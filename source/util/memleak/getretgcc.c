/*
 * Use gcc's __builtin_frame_address() function to get the stack trace.
 */

/* $XFree86: xc/util/memleak/getretgcc.c,v 1.2 2006/04/08 17:53:40 dawes Exp $ */

#include "getstack.h"

#define MIN_ADDR 0x1000

struct gccframe {
	struct gccframe *next;
	void *ret;
};

void
getStackTrace(unsigned long *results, int max)
{
    struct gccframe *frame;

    frame = (struct gccframe *)__builtin_frame_address(0);
#if defined(__powerpc__)
    if (frame)
	frame = frame->next;
#endif
    while ((unsigned long)frame > MIN_ADDR && max-- > 1) {
	*results = (unsigned long)frame->ret;
	if (*results < MIN_ADDR)
		break;
	results++;
	frame = frame->next;
    }
    *results = 0;
}

