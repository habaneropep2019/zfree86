/* $XFree86: xc/util/memleak/stackbottom.h,v 1.1 2006/03/02 03:00:40 dawes Exp $ */

typedef char *ptr_t;	/* A generic pointer to which we can add        */
			/* byte displacements.                          */
			/* Preferably identical to caddr_t, if it       */
			/* exists.                                      */

extern ptr_t GC_get_stack_base(void);

