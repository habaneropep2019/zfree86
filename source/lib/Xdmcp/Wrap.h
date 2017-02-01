/*
 * header file for compatibility with something useful
 */

/* $XFree86: xc/lib/Xdmcp/Wrap.h,v 1.6 2006/01/09 14:59:09 dawes Exp $ */

typedef unsigned char auth_cblock[8];	/* block size */

typedef struct auth_ks_struct { auth_cblock _; } auth_wrapper_schedule[16];

extern void _XdmcpWrapperToOddParity (unsigned char *in, unsigned char *out);

#ifdef HASXDMAUTH
extern void _XdmcpAuthSetup (auth_cblock key, auth_wrapper_schedule schedule);
extern void _XdmcpAuthDoIt (auth_cblock input, auth_cblock output,
	auth_wrapper_schedule schedule, int edflag);
#endif
