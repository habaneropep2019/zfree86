#! /bin/awk -f
# $XFree86: xc/doc/specs/rstart/fix.awk,v 1.2 2006/01/09 14:56:37 dawes Exp $
#

BEGIN {
	ignore = 1;
}

# following line starts /^L/
//	{
	print;
	ignore = 1;
	next;
}

/^$/ {
	if(ignore) next;
}

{
	ignore = 0;
	print;
}
