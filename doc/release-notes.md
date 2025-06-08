Neobytes Core version 0.12.1 is now available from:

  <https://github.com/neobytes-project/neobytes/releases/>


Notable changes
===============

Example item
---------------------------------------

Example text.

0.12.1 Change log
=================

Detailed release notes follow. This overview includes changes that affect
behavior, not code moves, refactors and string updates. For convenience in locating
the code changes and accompanying discussion, both the pull request and
git merge commit are mentioned.

### RPC and REST

Asm script outputs replacements for OP_NOP2 and OP_NOP3
-------------------------------------------------------

OP_NOP2 has been renamed to OP_CHECKLOCKTIMEVERIFY by [BIP 
65](https://github.com/bitcoin/bips/blob/master/bip-0065.mediawiki)

OP_NOP3 has been renamed to OP_CHECKSEQUENCEVERIFY by [BIP 
112](https://github.com/bitcoin/bips/blob/master/bip-0112.mediawiki)

The following outputs are affected by this change:
- RPC `getrawtransaction` (in verbose mode)
- RPC `decoderawtransaction`
- RPC `decodescript`
- REST `/rest/tx/` (JSON format)
- REST `/rest/block/` (JSON format when including extended tx details)
- `bitcoin-tx -json`

### ZMQ

Each ZMQ notification now contains an up-counting sequence number that allows
listeners to detect lost notifications.
The sequence number is always the last element in a multi-part ZMQ notification and
therefore backward compatible.
Each message type has its own counter.
(https://github.com/bitcoin/bitcoin/pull/7762)

### Configuration and command-line options

### Block and transaction handling

### P2P protocol and network code

### Validation

### Build system

### Wallet

### GUI

### Tests and QA

### Miscellaneous

Credits
=======

Thanks to everyone who directly contributed to this release:


As well as everyone that helped translating on [Transifex](https://www.transifex.com/projects/p/bitcoin/).

Neobytes Core tree 0.12.x was a fork of Dash Core tree 0.12.

These release are considered obsolete. Old changelogs can be found here:

- [v0.12.0](release-notes/dash/release-notes-0.12.0.md) released ???/??/2015
- [v0.11.2](release-notes/dash/release-notes-0.11.2.md) released Mar/25/2015
- [v0.11.1](release-notes/dash/release-notes-0.11.1.md) released Feb/10/2015
- [v0.11.0](release-notes/dash/release-notes-0.11.0.md) released Jan/15/2015
- [v0.10.x](release-notes/dash/release-notes-0.10.0.md) released Sep/25/2014
- [v0.9.x](release-notes/dash/release-notes-0.9.0.md) released Mar/13/2014

