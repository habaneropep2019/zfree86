#! /bin/nawk -f
# $XFree86: xc/doc/specs/rstart/fix.nawk,v 1.2 2006/01/09 14:56:37 dawes Exp $
#

BEGIN {
	ignore = 1;
}

/FORMFEED\[Page/	{
	sub("FORMFEED", "        ");
	print;
	print "";
	ignore = 1;
	next;
}

$0 == "" {
	if(ignore) next;
}

{
	ignore = 0;
	print;
}
