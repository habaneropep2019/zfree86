/* $XFree86: xc/lib/font/stubs/stubs.h,v 1.5 2005/10/14 15:16:06 tsi Exp $ */

/* This directory includes dummy entry for bdftopcf and mkfontdir */

#include <stdio.h>
#include "fntfilst.h"
#include <X11/fonts/font.h>


#ifndef True
#define True (-1)
#endif
#ifndef False
#define False (0)
#endif

extern FontPtr find_old_font ( FSID id );
extern int set_font_authorizations ( char **authorizations, 
				     int *authlen, 
				     ClientPtr client );

extern unsigned long GetTimeInMillis (void);

extern void ErrorF(const char *format, ...);
extern void FatalError(const char *format, ...);

/* end of file */
