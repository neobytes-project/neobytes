
Debian
====================
This directory contains files used to package neobytesd/neobytes-qt
for Debian-based Linux systems. If you compile neobytesd/neobytes-qt yourself, there are some useful files here.

## neobytes: URI support ##


neobytes-qt.desktop  (Gnome / Open Desktop)
To install:

	sudo desktop-file-install neobytes-qt.desktop
	sudo update-desktop-database

If you build yourself, you will either need to modify the paths in
the .desktop file or copy or symlink your neobytes-qt binary to `/usr/bin`
and the `../../share/pixmaps/neobytes128.png` to `/usr/share/pixmaps`

neobytes-qt.protocol (KDE)

