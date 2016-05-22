
                __^__                 __^__
                ( ___ )---------------( ___ )
                | / |  battery-stats  | \ |
                |___|                 |___|
                (_____)---------------(_____)

    Copyright (C) 2003 Karl E. Jørgensen <karl@jorgensen.com>
    Copyright (C) 2012 Kevin Funk <kevin@kfunk.org>
        Licensed under the GNU General Public License v2
	Copyright (C) 2015 F Zenke <fzenke@stanford.edu>
    Copyright (C) 2016 Petter Reinholdtsen <pere@hungry.com>


What is it?
===========
Battery-stats is a simple utility for collecting statistics about the
laptop battery charge.  Basically it will query ACPI at regular intervals
and write the results to a log file.

It also contains a simple plotting utility to show the battery charge over
time.

It does not (and will never) contain any graphical real-time "monitors" to
show the current state of the battery; lots of other utilities already do
this quite nicely.


Interpreting the Stats
======================
First of all, I have no delusions of being a battery expert.  This means
that I will not make any serious attempt at interpreting the stats.

What I know can be summed up as:

    When batteries age, they will take longer and longer to recharge, and
    will discharge quicker and quicker.

However, I suspect that the *shape* of the charge/discarge curves (and
the way the shape changes over several months) can tell something about the
battery health.  Ultimately, it should be possible to figure out when the
battery will wear out and need replacement.

Hopefully somebody with some electrochemical knowledge can fill in the
(large) gaps of my knowledge here - I promise to give credit where credit
is due.

Note that several sorts of bugs can result in incorrect statistics.  Such
bugs can be introduced in several ways:

*   There are different ways of measuring "how full" a battery is.
    Apparently, it can be measured by the charging current.  Such a
    measurement has to be "calibrated" with knowledge of the battery
    type (NiCad, LiIon etc), number of cells etc.

*   Buggy ACPI bios'es.

*   Buggy kernel ACPI.  This I have not yet encountered, but in theory
    it exists...   It is best to make sure that you actually have ACPI
    available from the kernel - and enabled at boot time.

*   Bugs in this software.  But that's really, really improbable ;-)


How this all started
====================
As a laptop user, I found myself with ageing batteries.  And my (buggy) APM
bios was far from accurate when it came to estimating how long I could run
before having to recharge.

I found other utilities out there that could help here - e.g.  ibam @
ibam.sourceforge.net.  These seem to work quite well with estimating how
long before the next recharge.  Short-term stuff.  But none of them could
tell me how the battery would perform over longer periods - weeks & months.
Even with this tool, I don't have much of an idea (!), but at least this
will make it possible for others to collect and interpret the statistics.

Besides, being a typical laptop user, I did not have a "feel" for how long
I *ought* to be able to go before a recharge - or how it had behaved in the
past.  I needed statistics, graphs, software and my name in lights.  So I
wrote this.


Getting it
==========
It is available for download at:
    https://github.com/petterreinholdtsen/battery-status

Compiling it
============
You will need:

* The sources
* CMake
* GNU make

Installing it
=============
Debian GNU/Linux users:  Just apt-get install the .deb.
Everybody else: Read on and do it the hard way.

The hard way is not that complicated:
-	Make sure that you have bash installed
-	Make sure that you have gnuplot installed (only needed if you
	actually want graphs)
-	$ cmake .; make install

To start the battery-stats-collector daemon at boot time, you will need to
set up an init script and the relevant links /etc/rc*.d directories.  This
is all ready out-of-the-box for the Debian distribution.

To access the graph plotter from the X desktop you will need to set up a
menu item or launcher to launch /usr/bin/battery-graph.  This is already
set up for the Debian distribution.

There are no provisions for stopping the log file from growing too much
(unless you are running ... yes! you guessed it: Debian GNU/Linux!).  Use
of logrotate is recommended.


Copyright
=========
Please see the COPYING file in this directory.


Bugs
====
None yet!

Please report any bugs you find to:
    https://github.com/petterreinholdtsen/battery-status

Bugs related to the debian package should be reported to:
    http://bugs.debian.org/battery-stats

Warranty
========
Sorry for the legalese blurb. Quick summary: "CMA" ...

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
    THE AUTHOR(S) OR ANY DISTRIBUTORS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
    FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
    DEALINGS IN THE SOFTWARE.


<!-- vim:set textwidth=75 autoindent: -->
