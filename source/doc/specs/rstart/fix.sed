#! /bin/sed -f
#
# $XFree86: xc/doc/specs/rstart/fix.sed,v 1.2 2006/01/09 14:56:37 dawes Exp $
#
s/o+/./g
s/|-/+/g
s/.//g
/FORMFEED\[Page/{
s/FORMFEED/        /
a\

}
