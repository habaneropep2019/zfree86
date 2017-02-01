/* $XFree86: xc/lib/font/include/fontmod.h,v 1.4 2006/03/16 16:49:48 dawes Exp $ */

#ifndef _FONTMOD_H_
#define _FONTMOD_H_

#ifndef _XF86MODULE_H
typedef struct module_desc *ModuleDescPtr;
#endif

typedef void (*InitFont)(void);

typedef struct font_module {
    InitFont		initFunc;
    char *		name;
    ModuleDescPtr	module;
} FontModule;

extern FontModule *FontModuleList;

#endif /* _FONTMOD_H_ */
