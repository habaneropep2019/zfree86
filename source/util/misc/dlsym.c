/* $XFree86: xc/util/misc/dlsym.c,v 1.2 2006/01/09 15:01:58 dawes Exp $ */
/*
 * Stub interface to dynamic linker routines
 * that SunOS uses but didn't ship with 4.1.
 *
 * The C library routine wcstombs in SunOS 4.1 tries to dynamically
 * load some routines using the dlsym interface, described in dlsym(3x).
 * Unfortunately SunOS 4.1 does not include the necessary library, libdl.
 *
 * The R5 Xlib uses wcstombs.  If you link dynamcally, your program can
 * run even with the unresolved reference to dlsym.  However, if you
 * link statically, you will encounter this bug.  One workaround
 * is to include these stub routines when you link.
 */

void *dlopen()
{
    return 0;
}

void *dlsym()
{
    return 0;
}

int dlclose()
{
    return -1;
}
