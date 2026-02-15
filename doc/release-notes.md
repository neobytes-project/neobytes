Neobytes Core version 0.12.2.2
==============================

Release is now available from:

  <https://github.com/neobytes-project/neobytes/releases/>

This is a new minor version release, bringing various bugfixes and other
improvements.

Please report bugs using the issue tracker at github:

  <https://github.com/neobytes-project/neobytes/issues>


Upgrading and downgrading
=========================

How to Upgrade
--------------

If you are running an older version, shut it down. Wait until it has completely
shut down (which might take a few minutes for older versions), then run the
installer (on Windows) or just copy over /Applications/Neobytes-Qt (on Mac) or
neobytesd/neobytes-qt (on Linux). Because of the per-UTXO fix (see below) there is a
one-time database upgrade operation, so expect a slightly longer startup time on
the first run.

Downgrade warning
-----------------

### Downgrade to a version < 0.12.2.2

Because release 0.12.2.2 includes the per-UTXO fix (see below) which changes the
structure of the internal database, this release is not fully backwards
compatible. You will have to reindex the database if you decide to use any
previous version.

This does not affect wallet forward or backward compatibility.


Notable changes
===============

Per-UTXO fix
------------

This fixes a potential vulnerability, so called 'Corebleed', which was
demonstrated this summer at the Вrеаkіng Віtсоіn Соnfеrеnсе іn Раrіs. The DoS
can cause nodes to allocate excessive amounts of memory, which leads them to a
halt. You can read more about the fix in the original Bitcoin Core pull request
https://github.com/bitcoin/bitcoin/pull/10195

To fix this issue in Neobytes Core however, we had to backport a lot of other
improvements from Bitcoin Core, see full list of backports in the detailed
change log below.

Additional indexes fix
----------------------

If you were using additional indexes like `addressindex`, `spentindex` or
`timestampindex` it's possible that they are not accurate. Please consider
reindexing the database by starting your node with `-reindex` command line
option. This is a one-time operation, the issue should be fixed now.

InstantSend fix
---------------

InstantSend should work with multisig addresses now.

PrivateSend fix
---------------

Some internal data structures were not cleared properly, which could lead
to a slightly higher memory consumption over a long period of time. This was
a minor issue which was not affecting mixing speed or user privacy in any way.

Removal of support for local masternodes
----------------------------------------

Keeping a wallet with 5000 NBY unlocked for 24/7 is definitely not a good idea
anymore. Because of this fact, it's also no longer reasonable to update and test
this feature, so it's completely removed now. If for some reason you were still
using it, please follow one of the guides and setup a remote masternode instead.

Dropping old (pre-12.2) peers
-----------------------------

Connections from peers with protocol lower than 70208 are no longer accepted.

Other improvements and bug fixes
--------------------------------

As a result of previous intensive refactoring and some additional fixes,
it should be possible to compile Neobytes Core with `--disable-wallet` option now.

This release also improves sync process and significantly lowers the time after
which `getblocktemplate` rpc becomes available on node start.

And as usual, various small bugs and typos were fixed and more refactoring was
done too.


0.12.2.2 Change log
===================

See detailed [change log](https://github.com/neobytes-project/neobytes/compare/v0.12.2.1...neobytes-project:v0.12.2.2).


Credits
=======

Thanks to everyone who directly contributed to this release:

- Alexander Block
- shade
- sidhujag
- thephez
- turbanoff
- Ilya Savinov
- UdjinM6
- Will Wray

As well as everyone that submitted issues and reviewed pull requests.


Older releases
==============

Neobytes Core tree 0.12.1.x was a fork of Dash Core tree 0.12.1

These release are considered obsolete. Old release notes can be found here:

- [v0.12.2](release-notes/release-notes-0.12.2.md) released Feb/??/2026
- [v0.12.1](release-notes/release-notes-0.12.1.md) released Jun/??/2021

