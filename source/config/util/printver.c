
/*
 * A simple program to make it possible to print the XFree86 version and
 * date info as defined in xf86Version.h and xf86Date.h, as well as
 * XFree86CustomVersion as defined in site.def very early in the
 * build process.
 */

/* $XFree86: xc/config/util/printver.c,v 1.4 2006/06/28 03:20:26 dawes Exp $ */

#include <stdio.h>
#include <stdlib.h>
#include "xf86Version.h"
#include "xf86Date.h"
#include "site.h"

int
main(int argc, char *argv[])
{
#ifdef XF86_VERSION_MAJOR
	printf(" version %d.%d.%d", XF86_VERSION_MAJOR, XF86_VERSION_MINOR,
		XF86_VERSION_PATCH);
	if (XF86_VERSION_SNAP != 0)
		printf(".%d", XF86_VERSION_SNAP);
#ifdef XF86_DATE
	printf(" (%s)", XF86_DATE);
#endif
#endif
#ifdef XFree86CustomVersion
	printf(" (%s)", XFree86CustomVersion);
#endif
	exit(0);
}

