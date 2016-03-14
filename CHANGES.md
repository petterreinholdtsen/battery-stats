0.4.0 (2016-03-14)
=====
* New developer.
* Collector rewritten as shell script, loosing some features along the way.

0.3.7 (unreleased)
=====
* Use CMake buildsystem, and other code base cleanups
* Make Gnuplot syntax used in graph-setup compatible with gnuplot-4.6
* Fix missing 'I' option in help output
* Make it possible to write to stdout instead just /var/log/...
* Convert all source files to UTF-8
* Improve --help output a lot

0.3.6 (2009-09-19)
=====
* New upstream release that integrates a patch to specify the battery number
  (see http://bugs.debian.org/cgi-bin/bugreport.cgi?bug=535815)

0.3.5 (2009-06-13)
=====
* Debian release

0.3.4 (2009-01-25)
=====
* Merged ACPI support from debian/patches
  - initial patch author: Michael Bunk <michael.bunk@gmail.com>
  - patch adapted to support autotools by Antonio Radici <antonio@dyne.org>
* Created a ./configure script using autotools to support the optional
  ACPI patch
* New gnuplot syntax supported, taken from debian/patches
* Corrected some manpages formatting error, taken from debian/patches
* New maintaner: Antonio Radici <antonio@dyne.org>
* Added an icon, gnome-ccperiph.png, it's taken from the debian
  package gnome-desktop-data, the icon is licensed with the GPL-2 and
  it's copyrighted by the GNOME group

0.3.3 (2003-03-21)
=====
* Converted part of the CVS changelog into this.  The debian/changelog
  is exclusively for use by debian-specific changes from now on.
* battery-log: Added (a slightly modified version of) patch from
  David Z Maze (debian bug #185623).
* battery-stats-collector.c:myversion : oops: This still thought it
  was version 0.2. It's now version 0.3.3...

0.3.2 (2003-02-21)
=====
* debian/: README, changelog, control, copyright, init.d, logrotate,
  menu, rules: Removed debian-specific files from trunc.  They now
  belong on the debian_version_xxx branches
* debian/control: Slight paraphrasing in Note:
* debian/changelog, debian/control: Bumped Standards-Version: to 3.5.6.1
* debian/README: No more mention of splitting into multiple packages -
  this is already mentioned in ../README
* README: Slightly more verbose now
* README: Bumped version number to 0.3.2
* debian/README: Now available on non-i386 too.
* debian/: changelog, menu: Menu entry is now immune to log rotation
* debian/changelog: No longer a debian-native package

0.3.1 (2003-01-16)
=====
* See debian/changelog for prehistoric change history.
