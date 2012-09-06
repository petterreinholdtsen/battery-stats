# this file is part of the battery-stats package.
# Copyright (C) 2009 Antonio Radici <antonio@dyne.org>
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

# the content of this makefile is derived from the old Makefile of
# battery-stats 0.3.3 written by Karl E. Jørgensen

sbin_PROGRAMS = battery-stats-collector
battery_stats_collector_SOURCES = battery-stats-collector.c
battery_stats_collector_LDADD = $(APMLIB) $(ACPILIB)

dist_bin_SCRIPTS = battery-graph battery-log
dist_pkgdata_DATA = graph-setup

dist_sysconf_DATA = battery-stats.conf
