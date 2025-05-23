// Copyright (c) 2014-2017 The Dash Core developers
// Copyright (c) 2021-2022 The Neobytes Core developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef GOVERNANCE_OBJECT_H
#define GOVERNANCE_OBJECT_H

//#define ENABLE_NEOBYTES_DEBUG

#include "cachemultimap.h"
#include "governance-exceptions.h"
#include "governance-vote.h"
#include "governance-votedb.h"
#include "key.h"
#include "net.h"
#include "sync.h"
#include "util.h"

#include <univalue.h>

class CGovernanceManager;
class CGovernanceTriggerManager;
class CGovernanceObject;
class CGovernanceVote;

static const int MAX_GOVERNANCE_OBJECT_DATA_SIZE = 16 * 1024;
static const int MIN_GOVERNANCE_PEER_PROTO_VERSION = 70204;
static const int GOVERNANCE_FILTER_PROTO_VERSION = 70206;

static const double GOVERNANCE_FILTER_FP_RATE = 0.001;

static const int GOVERNANCE_OBJECT_UNKNOWN = 0;
static const int GOVERNANCE_OBJECT_PROPOSAL = 1;
static const int GOVERNANCE_OBJECT_TRIGGER = 2;
static const int GOVERNANCE_OBJECT_WATCHDOG = 3;

static const CAmount GOVERNANCE_PROPOSAL_FEE_TX = (5.0*COIN);

static const int64_t GOVERNANCE_FEE_CONFIRMATIONS = 6;
static const int64_t GOVERNANCE_UPDATE_MIN = 60*60;
static const int64_t GOVERNANCE_DELETION_DELAY = 10*60;
static const int64_t GOVERNANCE_ORPHAN_EXPIRATION_TIME = 10*60;
static const int64_t GOVERNANCE_WATCHDOG_EXPIRATION_TIME = 2*60*60;

static const int GOVERNANCE_TRIGGER_EXPIRATION_BLOCKS = 576;

// FOR SEEN MAP ARRAYS - GOVERNANCE OBJECTS AND VOTES
static const int SEEN_OBJECT_IS_VALID = 0;
static const int SEEN_OBJECT_ERROR_INVALID = 1;
static const int SEEN_OBJECT_ERROR_IMMATURE = 2;
static const int SEEN_OBJECT_EXECUTED = 3; //used for triggers
static const int SEEN_OBJECT_UNKNOWN = 4; // the default

typedef std::pair<CGovernanceVote, int64_t> vote_time_pair_t;

inline bool operator<(const vote_time_pair_t& p1, const vote_time_pair_t& p2)
{
    return (p1.first < p2.first);
}

struct vote_instance_t {

    vote_outcome_enum_t eOutcome;
    int64_t nTime;
    int64_t nCreationTime;

    vote_instance_t(vote_outcome_enum_t eOutcomeIn = VOTE_OUTCOME_NONE, int64_t nTimeIn = 0, int64_t nCreationTimeIn = 0)
        : eOutcome(eOutcomeIn),
          nTime(nTimeIn),
          nCreationTime(nCreationTimeIn)
    {}

    ADD_SERIALIZE_METHODS;

    template <typename Stream, typename Operation>
    inline void SerializationOp(Stream& s, Operation ser_action, int nType, int nVersion)
    {
        int nOutcome = int(eOutcome);
        READWRITE(nOutcome);
        READWRITE(nTime);
        READWRITE(nCreationTime);
        if(ser_action.ForRead()) {
            eOutcome = vote_outcome_enum_t(nOutcome);
        }
    }
};

typedef std::map<int,vote_instance_t> vote_instance_m_t;

typedef vote_instance_m_t::iterator vote_instance_m_it;

typedef vote_instance_m_t::const_iterator vote_instance_m_cit;

struct vote_rec_t {
    vote_instance_m_t mapInstances;

    ADD_SERIALIZE_METHODS;

     template <typename Stream, typename Operation>
     inline void SerializationOp(Stream& s, Operation ser_action, int nType, int nVersion)
     {
         READWRITE(mapInstances);
     }
};

/**
* Governance Object
*
*/

class CGovernanceObject
{
    friend class CGovernanceManager;

    friend class CGovernanceTriggerManager;

public: // Types
    typedef std::map<int, vote_rec_t> vote_m_t;

    typedef vote_m_t::iterator vote_m_it;

    typedef vote_m_t::const_iterator vote_m_cit;

    typedef CacheMultiMap<CTxIn, vote_time_pair_t> vote_mcache_t;

private:
    /// critical section to protect the inner data structures
    mutable CCriticalSection cs;

    /// Object typecode
    int nObjectType;

    /// parent object, 0 is root
    uint256 nHashParent;

    /// object revision in the system
    int nRevision;

    /// time this object was created
    int64_t nTime;

    /// time this object was marked for deletion
    int64_t nDeletionTime;

    /// fee-tx
    uint256 nCollateralHash;

    /// Data field - can be used for anything
    std::string strData;

    /// Masternode info for signed objects
    CTxIn vinMasternode;
    std::vector<unsigned char> vchSig;

    /// is valid by blockchain
    bool fCachedLocalValidity;
    std::string strLocalValidityError;

    // VARIOUS FLAGS FOR OBJECT / SET VIA MASTERNODE VOTING

    /// true == minimum network support has been reached for this object to be funded (doesn't mean it will for sure though)
    bool fCachedFunding;

    /// true == minimum network has been reached flagging this object as a valid and understood goverance object (e.g, the serialized data is correct format, etc)
    bool fCachedValid;

    /// true == minimum network support has been reached saying this object should be deleted from the system entirely
    bool fCachedDelete;

    /** true == minimum network support has been reached flagging this object as endorsed by an elected representative body
     * (e.g. business review board / technecial review board /etc)
     */
    bool fCachedEndorsed;

    /// object was updated and cached values should be updated soon
    bool fDirtyCache;

    /// Object is no longer of interest
    bool fExpired;

    /// Failed to parse object data
    bool fUnparsable;

    vote_m_t mapCurrentMNVotes;

    /// Limited map of votes orphaned by MN
    vote_mcache_t mapOrphanVotes;

    CGovernanceObjectVoteFile fileVotes;

public:
    CGovernanceObject();

    CGovernanceObject(uint256 nHashParentIn, int nRevisionIn, int64_t nTime, uint256 nCollateralHashIn, std::string strDataIn);

    CGovernanceObject(const CGovernanceObject& other);

    void swap(CGovernanceObject& first, CGovernanceObject& second); // nothrow

    // Public Getter methods

    int64_t GetCreationTime() const {
        return nTime;
    }

    int64_t GetDeletionTime() const {
        return nDeletionTime;
    }

    int GetObjectType() const {
        return nObjectType;
    }

    const uint256& GetCollateralHash() const {
        return nCollateralHash;
    }

    const CTxIn& GetMasternodeVin() const {
        return vinMasternode;
    }

    bool IsSetCachedFunding() const {
        return fCachedFunding;
    }

    bool IsSetCachedValid() const {
        return fCachedValid;
    }

    bool IsSetCachedDelete() const {
        return fCachedDelete;
    }

    bool IsSetCachedEndorsed() const {
        return fCachedEndorsed;
    }

    bool IsSetDirtyCache() const {
        return fDirtyCache;
    }

    bool IsSetExpired() const {
        return fExpired;
    }

    void InvalidateVoteCache() {
        fDirtyCache = true;
    }

    CGovernanceObjectVoteFile& GetVoteFile() {
        return fileVotes;
    }

    // Signature related functions

    void SetMasternodeInfo(const CTxIn& vin);
    bool Sign(CKey& keyMasternode, CPubKey& pubKeyMasternode);
    bool CheckSignature(CPubKey& pubKeyMasternode);

    std::string GetSignatureMessage() const;

    // CORE OBJECT FUNCTIONS

    bool IsValidLocally(std::string& strError, bool fCheckCollateral);

    bool IsValidLocally(std::string& strError, bool& fMissingMasternode, bool fCheckCollateral);

    /// Check the collateral transaction for the budget proposal/finalized budget
    bool IsCollateralValid(std::string& strError);

    void UpdateLocalValidity();

    void UpdateSentinelVariables();

    int GetObjectSubtype();

    CAmount GetMinCollateralFee();

    UniValue GetJSONObject();

    void Relay();

    uint256 GetHash() const;

    // GET VOTE COUNT FOR SIGNAL

    int CountMatchingVotes(vote_signal_enum_t eVoteSignalIn, vote_outcome_enum_t eVoteOutcomeIn) const;

    int GetAbsoluteYesCount(vote_signal_enum_t eVoteSignalIn) const;
    int GetAbsoluteNoCount(vote_signal_enum_t eVoteSignalIn) const;
    int GetYesCount(vote_signal_enum_t eVoteSignalIn) const;
    int GetNoCount(vote_signal_enum_t eVoteSignalIn) const;
    int GetAbstainCount(vote_signal_enum_t eVoteSignalIn) const;

    bool GetCurrentMNVotes(const CTxIn& mnCollateralOutpoint, vote_rec_t& voteRecord);

    // FUNCTIONS FOR DEALING WITH DATA STRING

    std::string GetDataAsHex();
    std::string GetDataAsString();

    // SERIALIZER

    ADD_SERIALIZE_METHODS;

    template <typename Stream, typename Operation>
    inline void SerializationOp(Stream& s, Operation ser_action, int nType, int nVersion)
    {
        // SERIALIZE DATA FOR SAVING/LOADING OR NETWORK FUNCTIONS

        READWRITE(nHashParent);
        READWRITE(nRevision);
        READWRITE(nTime);
        READWRITE(nCollateralHash);
        READWRITE(LIMITED_STRING(strData, MAX_GOVERNANCE_OBJECT_DATA_SIZE));
        READWRITE(nObjectType);
        READWRITE(vinMasternode);
        READWRITE(vchSig);
        if(nType & SER_DISK) {
            // Only include these for the disk file format
            LogPrint("gobject", "CGovernanceObject::SerializationOp Reading/writing votes from/to disk\n");
            READWRITE(nDeletionTime);
            READWRITE(fExpired);
            READWRITE(mapCurrentMNVotes);
            READWRITE(fileVotes);
            LogPrint("gobject", "CGovernanceObject::SerializationOp hash = %s, vote count = %d\n", GetHash().ToString(), fileVotes.GetVoteCount());
        }

        // AFTER DESERIALIZATION OCCURS, CACHED VARIABLES MUST BE CALCULATED MANUALLY
    }

    CGovernanceObject& operator=(CGovernanceObject from)
    {
        swap(*this, from);
        return *this;
    }

private:
    // FUNCTIONS FOR DEALING WITH DATA STRING
    void LoadData();
    void GetData(UniValue& objResult);

    bool ProcessVote(CNode* pfrom,
                     const CGovernanceVote& vote,
                     CGovernanceException& exception);

    void RebuildVoteMap();

    /// Called when MN's which have voted on this object have been removed
    void ClearMasternodeVotes();

    void CheckOrphanVotes();

};


#endif
