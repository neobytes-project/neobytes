Neobytes Core version 0.12.2
============================

Release is now available from:

  <https://github.com/neobytes-project/neobytes/releases/>

This is a new major version release, bringing new features and other improvements.

Please report bugs using the issue tracker at github:

  <https://github.com/neobytes-project/neobytes/issues>

Upgrading and downgrading
=========================

How to Upgrade
--------------

If you are running an older version, shut it down. Wait until it has completely
shut down (which might take a few minutes for older versions), then run the
installer (on Windows) or just copy over /Applications/Neobytes-Qt (on Mac) or
neobytesd/neobytes-qt (on Linux).

Downgrade warning
-----------------
### Downgrade to a version < 0.12.2

Because release 0.12.2 includes DIP0001 (2 MB block size hardfork) plus
a transaction fee reduction and a fix for masternode rank calculation algo
(which activation also depends on DIP0001) this release will not be
backwards compatible after DIP0001 lock in/activation happens.

This does not affect wallet forward or backward compatibility.

Notable changes
===============

DIP0001
-------

We outline an initial scaling mechanism for Neobytes. After deployment and activation, Neobytes will be able to handle double the transactions it can currently handle. Together with the faster block times, Neobytes we will be prepared to handle eight times the traffic of Bitcoin.

https://github.com/neobytes-project/dips/blob/master/dip-0001.md


Fee reduction
-------------

All transaction fees are reduced 10x (from 10K per Kb to 1K per Kb), including fees for InstantSend (from 0.001 NBY per input to 0.0001 per input)

InstantSend fix
---------------

The potential vulnerability found by Matt Robertson and Alexander Block was fixed in d7a8489f3 (#1620).

RPC changes
-----------

There are few changes in existing RPC in this release:
- There is no more `bcconfirmations` field in RPC output and `confirmations` shows blockchain only confirmations by default now. You can change this behaviour by switching new `addlockconf` param to `true`. There is a new rpc field `instantlock` which indicates whether a given transaction is locked via InstantSend. For more info and examples please see https://github.com/neobytes-project/neobytes/doc/instantsend.md;
- `gobject list` and `gobject diff` accept `funding`, `delete` and `endorsed` filtering options now, in addition to `valid` and `all` currently available;
- `vin` field in `masternode` commands is renamed to `outpoint` and shows data in short format now;
- `getblocktemplate` output is extended with versionbits-related information;
- Output of wallet-related commands `validateaddress` is extended with optional `hdkeypath` and `hdchainid` fields.

There are few new RPC commands also:
- `masternodelist info` shows additional information about sentinel for each masternode in the list;
- `masternodelist pubkey` shows pubkey corresponding to masternodeprivkey for each masternode in the list;
- `gobject check` allows to check proposal data for correctness before preparing/submitting the proposal, `gobject prepare` and `gobject submit` should also perform additional validation now though;
- `setnetworkactive` allows to turn all network activity on and off;
- `dumphdinfo` displays some information about HD wallet (if available).

Command-line options
--------------------

New: `assumevalid`, `blocksonly`, `reindex-chainstate`

Experimental: `usehd`, `mnemonic`, `mnemonicpassphrase`, `hdseed`

See `Help -> Command-line options` in Qt wallet or `neobytesd --help` for more info.

PrivateSend improvements
------------------------

Algorithm for selecting inputs was slightly changed. This should allow user to get some mixed funds much faster.

Lots of backports, refactoring and bug fixes
--------------------------------------------

We backported some performance improvements from Bitcoin Core and aligned our codebase with their source a little bit better. We still do not have all the improvements so this work is going to be continued in next releases.

A lot of refactoring and other fixes should make code more reliable and easier to review now.

Experimental HD wallet
----------------------

This release includes experimental implementation of BIP39/BIP44 compatible HD wallet. Wallet type (HD or non-HD) is selected when wallet is created via `usehd` command-line option, default is `0` which means that a regular non-deterministic wallet is going to be used. If you decide to use HD wallet, you can also specify BIP39 mnemonic and mnemonic passphrase (see `mnemonic` and `mnemonicpassphrase` command-line options) but you can do so only on initial wallet creation and can't change these afterwards. If you don't specify them, mnemonic is going to be generated randomly and mnemonic passphrase is going to be just a blank string.

**WARNING:** The way it's currently implemented is NOT safe and is NOT recommended to use on mainnet. Wallet is created unencrypted with mnemonic stored inside, so even if you encrypt it later there will be a short period of time when mnemonic is stored in plain text. This issue will be addressed in future releases.

0.12.2 Change log
=================

### Backports:
- Align with btc 0.12
- Fix for neobytes-qt issue with startup and multiple monitors.
- Force to use C++11 mode for compilation
- Make strWalletFile const
- Access WorkQueue::running only within the cs lock.
- trivial: fix bloom filter init to isEmpty = true
- Avoid ugly exception in log on unknown inv type
- Don't return the address of a P2SH of a P2SH
- Generate auth cookie in hex instead of base64
- Do not shadow LOCK's criticalblock variable for LOCK inside LOCK
- Remove unnecessary LOCK(cs_main) in getrawpmempool
- Remove duplicate bantablemodel.h include
- Fix for build on Ubuntu 14.04 with system libraries
- Improve EncodeBase58/DecodeBase58 performance
- auto_ptr → unique_ptr
- Boost 1.61.0
- Boost 1.63.0
- Minimal fix to slow prevector tests as stopgap measure
- build: fix qt5.7 build under macOS
- Increase minimum debug.log size to 10MB after shrink.
- Backport Bitcoin PRs #6589, #7180 and remaining part of #7181: enable per-command byte counters in `CNode`
- Increase test coverage for addrman and addrinfo
- Eliminate unnecessary call to CheckBlock
- Backport Bincoin PR#7348: MOVE ONLY: move rpc* to rpc/ + same for Neobytes-specific rpc
- Backport Bitcoin PR#7349: Build against system UniValue when available
- Backport Bitcoin PR#7350: Banlist updates
- Backport Bitcoin PR#7458: [Net] peers.dat, banlist.dat recreated when missing
- Backport Bitcoin PR#7696: Fix de-serialization bug where AddrMan is corrupted after exception
- Backport Bitcoin PR#7749: Enforce expected outbound services
- Backport Bitcoin PR#7917: Optimize reindex
- Remove non-determinism which is breaking net_tests #8069
- fix race that could fail to persist a ban
- Backport Bitcoin PR#7906: net: prerequisites for p2p encapsulation changes
- Backport Bitcoin PR#8084: Add recently accepted blocks and txn to AttemptToEvictConnection
- Backport Bitcoin PR#8113: Rework addnode behaviour
- Remove bad chain alert partition check
- Added feeler connections increasing good addrs in the tried table.
- Backport Bitcoin PR#7942: locking for Misbehave() and other cs_main locking fixes
- Backport Bitcoin PR#8085: p2p: Begin encapsulation
- Backport Bitcoin PR#8049: Expose information on whether transaction relay is enabled in `getnetwork`
- backport 9008: Remove assert(nMaxInbound > 0)
- Backport Bitcoin PR#8708: net: have CConnman handle message sending
- net: only delete CConnman if it's been created
- Backport Bitcoin PR#8865: Decouple peer-processing-logic from block-connection-logic
- Backport Bitcoin PR#8969: Decouple peer-processing-logic from block-connection-logic (#2)
- Backport Bitcoin PR#9075: Decouple peer-processing-logic from block-connection-logic (#3)
- Backport Bitcoin PR#9183: Final Preparation for main.cpp Split
- Backport Bitcoin PR#8822: net: Consistent checksum handling
- Backport Bitcoin PR#9260: Mrs Peacock in The Library with The Candlestick (killed main.{h,cpp})
- Backport Bitcoin PR#9289: net: drop boost::thread_group
- Backport Bitcoin PR#9609: net: fix remaining net assertions
- Backport Bitcoin PR#9441: Net: Massive speedup. Net locks overhaul
- Backport "assumed valid blocks" feature from Bitcoin 0.13
- Partially backport Bitcoin PR#9626: Clean up a few CConnman cs_vNodes/CNode things
- Do not add random inbound peers to addrman.
- net: No longer send local address in addrMe
- Backport Bitcoin PR#7868: net: Split DNS resolving functionality out of net structures
- Backport Bitcoin PR#8128: Net: Turn net structures into dumb storage classes
- Backport Bitcoin Qt/Gui changes up to 0.14.x part 1
- Backport Bitcoin Qt/Gui changes up to 0.14.x part 2
- Backport Bitcoin Qt/Gui changes up to 0.14.x part 3
- Merge #8944: Remove bogus assert on number of oubound connections.
- Merge #10231: [Qt] Reduce a significant cs_main lock freeze

### PrivateSend:
- mix inputs with highest number of rounds first
- Few fixes for PrivateSend
- Refactor PS
- PrivateSend: dont waste keys from keypool on failure in CreateDenominated
- fix calculation of (unconfirmed) anonymizable balance
- split CPrivateSend
- Expire confirmed DSTXes after ~1h since confirmation
- fix MakeCollateralAmounts
- fix a bug in CommitFinalTransaction
- fix number of blocks to wait after successful mixing tx
- Fix losing keys on PrivateSend
- Make sure mixing masternode follows bip69 before signing final mixing tx
- speedup MakeCollateralAmounts by skiping denominated inputs early
- Keep track of wallet UTXOs and use them for PS balances and rounds calculations
- do not calculate stuff that are not going to be visible in simple PS UI anyway

### InstantSend:
- Safety check in CInstantSend::SyncTransaction
- Fix potential deadlock in CInstantSend::UpdateLockedTransaction
- fix instantsendtoaddress param convertion
- Fix: Reject invalid instantsend transaction
- InstandSend overhaul
- fix SPORK_5_INSTANTSEND_MAX_VALUE validation in CWallet::CreateTransaction
- Fix masternode score/rank calculations
- fix instantsend-related RPC output
- bump MIN_INSTANTSEND_PROTO_VERSION to 70208
- Fix edge case for IS (skip inputs that are too large)
- remove InstantSend votes for failed lock attemts after some timeout
-  update setAskFor on TXLOCKVOTE
- fix bug introduced in #1695

### Governance:
- Few changes for governance rpc:
- sentinel uses status of funding votes
- Validate proposals on prepare and submit
- Replace watchdogs with ping
- Fixed issues with propagation of governance objects
- Fix issues with mapSeenGovernanceObjects
- change invalid version string constant
- Fix vulnerability with mapMasternodeOrphanObjects
- New rpc call "masternodelist info"
- add 6 to strAllowedChars
- fixed potential deadlock in CSuperblockManager::IsSuperblockTriggered
- fix potential deadlock (PR#1536 fix)
- fix potential deadlock in CGovernanceManager::ProcessVote
- fix potential deadlock in CMasternodeMan::CheckMnbAndUpdateMasternodeList
- Fix MasternodeRateCheck
- fix off-by-1 in CSuperblock::GetPaymentsLimit
- Relay govobj and govvote to every compatible peer, not only to the one with the same version
- allow up to 40 chars in proposal name
- start_epoch, end_epoch and payment_amount should be numbers, not strings

### Network/Sync:
- fix sync reset which is triggered erroneously during reindex
- track asset sync time
- do not use masternode connections in feeler logic
- Make sure mixing messages are relayed/accepted properly
- fix CDSNotificationInterface::UpdatedBlockTip signature
- Sync overhaul
- limit UpdatedBlockTip in IBD
- Relay tx in sendrawtransaction according to its inv.type
- net: Consistently use GetTimeMicros() for inactivity checks
- Use GetAdjustedTime instead of GetTime when dealing with network-wide timestamps
- Fix sync issues
- Fix duplicate headers download in initial sync
- Use connman passed to ThreadSendAlert() instead of g_connman global.
- Eliminate g_connman use in spork module.
- Eliminate g_connman use in instantx module.
- Move some (spamy) CMasternodeSync log messages to new `mnsync` log category
- Eliminate remaining uses of g_connman in Neobytes-specific code.
- Wait for full sync in functional tests that use getblocktemplate.
- fix sync
- Fix unlocked access to vNodes.size()
- Remove cs_main from ThreadMnbRequestConnections
- Fix bug: nCachedBlockHeight was not updated on start
- Add more logging for MN votes and MNs missing votes
- Fix sync reset on lack of activity
- Fix mnp relay bug

### GUI:
- Full path in "failed to load cache" warnings
- Qt: bug fixes and enhancement to traffic graph widget
- Icon Cutoff Fix 
- Fix windows installer script, should handle `neobytes:` uri correctly now
- Update startup shortcuts
- fix TrafficGraphData bandwidth calculation
- Fix empty tooltip during sync under specific conditions
- [GUI] Change look of modaloverlay 
- Fix compilation with qt < 5.2
- Translations201710 - en, de, fi, fr, ru, vi
- Translations 201710 part2
- Add hires version of `light` theme for Hi-DPI screens

### DIP0001:
- DIP0001 implementation
- Dip0001-related adjustments (inc. constants)
- fix fDIP0001* flags initialization
- fix DIP0001 implementation
- fix: update DIP0001 related stuff even in IBD
- fix: The idea behind fDIP0001LockedInAtTip was to indicate that it WAS locked in, not that it IS locked in

### Wallet:
- HD wallet
- Minor Warning Fixed
- Disable HD wallet by default
- Lower tx fees 10x
- Ensure Neobytes wallets < 0.12.2 can't open HD wallets
- fix fallback fee

### RPC:
- add `masternodelist pubkey` to rpc
- more "vin" -> "outpoint" in masternode rpc output
- fix `masternode current` rpc
- remove send addresses from listreceivedbyaddress output
- fix Examples section of the RPC output for listreceivedbyaccount, listreceivedbyaccount and sendfrom commands
- RPC help formatting updates
- Revert "fix `masternode current` rpc (#1640)"
- partially revert "[Trivial] RPC help formatting updates #1670"


### Docs:
- Doc: fix broken formatting in markdown #headers
- Added clarifications in INSTALL readme for newcomers
- Documentation: Add spork message / details to protocol-documentation.md
- Documentation: Update undocumented messages in protocol-documentation.md
- Update `instantsend.md` according to PR#1628 changes
- Update build-osx.md formatting
- 12.2 release notes

### Other (noticeable) refactoring and fixes:
- Refactor: CDarkSendSigner
- Implement BIP69 outside of CTxIn/CTxOut
- fix deadlock
- fixed potential deadlock in CMasternodePing::SimpleCheck
- Masternode classes: Remove repeated/un-needed code and data
- add/use GetUTXO\[Coins/Confirmations\] helpers instead of GetInputAge\[IX\]
- drop pCurrentBlockIndex and use cached block height instead (nCachedBlockHeight)
- drop masternode index
- slightly refactor CDSNotificationInterface
- safe version of GetMasternodeByRank
- Refactor masternode management
- Remove some recursive locks

### Other (technical) commits:
- bump to 0.12.2.0
- Merge remote-tracking branch 'remotes/origin/master' into v0.12.2.x
- Fixed pow (test and algo)
- c++11: don't throw from the reverselock destructor
- Rename bitcoinconsensus library to neobytesconsensus.
- Fix the same header included twice.
- fix travis ci mac build
- fix BIP34 starting blocks for mainnet/testnet
- adjust/fix some log and error messages
- Neobytesify bitcoin unix executables
- Don't try to create empty datadir before the real path is known
- Force self-recheck on CActiveMasternode::ManageStateRemote()
- various trivial cleanup fixes
- include atomic
- Revert "fixed regtest+ds issues"
- workaround for travis
- Pass reference when calling HasPayeeWithVotes
- typo: "Writting" -> "Writing"
- build: silence gcc7's implicit fallthrough warning
- bump MIN_MASTERNODE_PAYMENT_PROTO_VERSION_2 and PROTOCOL_VERSION to 70208
- fix BIP68 granularity and mask
- Revert "fix BIP68 granularity and mask (#1641)"
- Fork testnet to test 12.2 migration
- fork testnet again to re-test dip0001 because of 2 bugs found in 1st attempt
- update nMinimumChainWork and defaultAssumeValid for testnet
- fix `setnetworkactive` (typo)
- update nCollateralMinConfBlockHash for local (hot) masternode on mn start
- Fix/optimize images
- fix trafficgraphdatatests for qt4

Credits
=======

Thanks to everyone who directly contributed to this release:

- Alex Werner
- Alexander Block
- Allan Doensen
- Bob Feldbauer
- chaeplin
- crowning-
- diego-ab
- Gavin Westwood
- gladcow
- Holger Schinzel
- Ilya Savinov
- Kamuela Franco
- krychlicki
- Nathan Marley
- Oleg Girko
- QuantumExplorer
- sorin-postelnicu
- Spencer Lievens
- taw00
- TheLazieR Yip
- thephez
- Tim Flynn
- UdjinM6
- Will Wray

As well as Neobytes Core Developers and everyone that submitted issues or helped translating on [Transifex](https://explore.transifex.com/neobytes-project/).


Older releases
==============

Neobytes Core tree 0.12.x was a fork of Dash Core tree 0.12.

These release are considered obsolete. Old changelogs can be found here:

- [v0.12.1](release-notes/neobytes/release-notes-0.12.1.md) released ???/??/2024

