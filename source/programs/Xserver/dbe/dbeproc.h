/* $XFree86: xc/programs/Xserver/dbe/dbeproc.h,v 1.3 2007/01/04 02:48:12 tsi Exp $ */

#ifndef DBEPROC_H
#define DBEPROC_H 1

#include "extnsionst.h"

#ifndef DBE_EXT_INIT_ONLY

#include "dbestruct.h"

typedef Bool (*DbeInitFuncPtr)(ScreenPtr pScreen,
			       DbeScreenPrivPtr pDbeScreenPriv);

/* DbeValidateBuffer declaration moved to dix.h */

void DbeRegisterFunction(ScreenPtr pScreen, DbeInitFuncPtr funct);

#endif /* DBE_EXT_INIT_ONLY */

void DbeExtensionInit(INITARGS);

#endif /* DBEPROC_H */
