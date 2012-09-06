dnl Copyright (C) Antonio Radici <antonio@dyne.org>
dnl Simple autoconf template, released under the terms of GPLv2 as
dnl the rest of the original battery-stats program
dnl

# autoconf init
AC_INIT([battery-stats], [0.3.4], [battery-stats@dyne.org])

# automake init
AM_INIT_AUTOMAKE([-Wall foreign])

###### Check for progarms

# compiler
AC_PROG_CC

# gnuplot
AC_CHECK_PROGS([GNUPLOT], [gnuplot], [:])
if test "$GNUPLOT" = :; then
	AC_MSG_ERROR([This package needs gnuplot to operate properly])
fi


###### Check for libraries

# libapm and apmread
AC_CHECK_LIB([apm], [apm_read], [APMLIB=-lapm])
if test "$APMLIB" = :; then
	AC_MSG_ERROR([APM lib needs to be installed for this package to work properly])
fi
AC_SUBST([APMLIB])

# acpilib
AC_CHECK_LIB([acpi], [init_acpi_batt], [ACPILIB=-lacpi])
if test "$ACPILIB" = -lacpi; then
	AC_DEFINE([WANT_ACPI], [1], [Enabling ACPI support])
fi
AC_SUBST([ACPILIB])


#### Checks for headers files
AC_HEADER_STDC
AC_CHECK_HEADERS([stdlib.h string.h syslog.h unistd.h])
AC_CHECK_HEADERS([libacpi.h])

#### Checks for typedefs, structures, and compiler characteristics.
AC_C_CONST
AC_STRUCT_TM

#### Checks for library functions.
AC_FUNC_MALLOC
AC_CHECK_FUNCS([strerror strtol])


#### Output
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_FILES([Makefile src/Makefile man/Makefile src/battery-graph pixmaps/Makefile])
AC_OUTPUT
