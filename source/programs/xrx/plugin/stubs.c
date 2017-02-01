/* $XFree86: xc/programs/xrx/plugin/stubs.c,v 1.2 2006/01/09 15:01:45 dawes Exp $ */
/* -*- Mode: C; tab-width: 4; -*- */
/*******************************************************************************
 * Simple LiveConnect Sample Plugin
 * Copyright (c) 1996 Netscape Communications. All rights reserved.
 ******************************************************************************/

/*
** Ok, so we don't usually include .c files (only .h files) but we're
** doing it here to avoid some fancy make rules. First pull in the common
** glue code:
*/
#ifdef XP_UNIX
#include "common/npunix.c"
#endif
