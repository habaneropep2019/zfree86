#! /bin/sh
# makeg - run "make" with options necessary to make a debuggable executable
# $XFree86: xc/config/util/makeg.sh,v 1.2 2006/01/09 14:56:16 dawes Exp $

# set GDB=1 in your environment if using gdb on Solaris 2.

make="${MAKE-make}"
flags="CDEBUGFLAGS=-g CXXDEBUGFLAGS=-g"

# gdb on Solaris needs the stabs included in the executable
test "${GDB+yes}" = yes && flags="$flags -xs"

exec "$make" $flags LDSTRIPFLAGS= ${1+"$@"}
