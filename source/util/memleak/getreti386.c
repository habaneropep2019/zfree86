/*
 * some bits copied from mprof by Ben Zorn
 *
 * Copyright (c) 1995 Jeffrey Hsu
 */

/* $XFree86: xc/util/memleak/getreti386.c,v 3.7 2006/04/02 00:16:52 dawes Exp $ */

#define get_current_fp(first_local)	((unsigned)&(first_local) + 4)
#define prev_fp_from_fp(fp)		*((unsigned *) fp)
#define ret_addr_from_fp(fp)		*((unsigned *) (fp+4))

#define MIN_ADDRESS		0x1000

#include "getstack.h"

static unsigned long
junk(int *foo)
{
    return (unsigned long)foo + 4;
}

void
getStackTrace(unsigned long *results, int max)
{

    int first_local;
    unsigned long fp, ret_addr;

    first_local = 0;
    fp = junk(&first_local);
    fp = get_current_fp(first_local);
    ret_addr = ret_addr_from_fp(fp);

    while (ret_addr > MIN_ADDRESS && max-- > 1) {
	*results++ = ret_addr;
	if (fp < (unsigned long)&first_local)
	    break;
	fp = prev_fp_from_fp(fp);
	if (!fp)
	    break;
	if (fp < (unsigned long)&first_local)
	    break;
	ret_addr = ret_addr_from_fp(fp);
    }
    *results++ = 0;

}
