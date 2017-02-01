/*
 * cut and paste from gc 4.4 by Hans-J. Boehm and Alan J. Demers
 *
 * Cutter and Paster:  Jeffrey Hsu
 */

/* $XFree86: xc/util/memleak/stackbottom.c,v 3.3 2006/03/02 03:00:40 dawes Exp $ */

#include <signal.h>
#include "stackbottom.h"

#ifndef bool
typedef int bool;
#endif

#define VOLATILE volatile

# ifndef STACK_GROWS_UP
#   define STACK_GROWS_DOWN
# endif

typedef unsigned long word;

#define TRUE 1

#if defined(__alpha) || defined(__alpha__)
extern __start
#   define HEURISTIC2_LIMIT ((ptr_t)((word)(&__start) & ~(getpagesize()-1)))
#endif

static void
GC_noop(void)
{
}

/* Some tools to implement HEURISTIC2	*/
#define MIN_PAGE_SIZE 256	/* Smallest conceivable page size, bytes */
#include <setjmp.h>
static jmp_buf GC_jmp_buf;

/*ARGSUSED*/
static void
GC_fault_handler(int sig)
{
    longjmp(GC_jmp_buf, 1);
}

typedef void (*handler) (int);

/* Return the first nonaddressible location > p (up) or 	*/
/* the smallest location q s.t. [q,p] is addressible (!up).	*/
static ptr_t
GC_find_limit(ptr_t p, bool up)
{
    static VOLATILE ptr_t result;

    /* Needs to be static, since otherwise it may not be        */
    /* preserved across the longjmp.  Can safely be             */
    /* static since it's only called once, with the             */
    /* allocation lock held.                                    */

    static handler old_segv_handler, old_bus_handler;

    /* See above for static declaration.                        */

    old_segv_handler = signal(SIGSEGV, GC_fault_handler);
#ifdef SIGBUS
    old_bus_handler = signal(SIGBUS, GC_fault_handler);
#endif
    if (setjmp(GC_jmp_buf) == 0) {
	result = (ptr_t) (((word) (p)) & ~(MIN_PAGE_SIZE - 1));
	for (;;) {
	    if (up) {
		result += MIN_PAGE_SIZE;
	    } else {
		result -= MIN_PAGE_SIZE;
	    }
	    GC_noop();
	}
    }
    (void)signal(SIGSEGV, old_segv_handler);
#ifdef SIGBUS
    (void)signal(SIGBUS, old_bus_handler);
#endif
    if (!up) {
	result += MIN_PAGE_SIZE;
    }
    return result;
}

ptr_t
GC_get_stack_base()
{
    word dummy;
    static ptr_t result;

    if (!result) {

#ifdef STACK_GROWS_DOWN
	result = GC_find_limit((ptr_t) (&dummy), TRUE);
#ifdef HEURISTIC2_LIMIT
	if (result > HEURISTIC2_LIMIT && (ptr_t) (&dummy) < HEURISTIC2_LIMIT) {
	    result = HEURISTIC2_LIMIT;
	}
#endif
#else
	result = GC_find_limit((ptr_t) (&dummy), FALSE);
#ifdef HEURISTIC2_LIMIT
	if (result < HEURISTIC2_LIMIT && (ptr_t) (&dummy) > HEURISTIC2_LIMIT) {
	    result = HEURISTIC2_LIMIT;
	}
#endif
#endif
    }

    return result;
}
