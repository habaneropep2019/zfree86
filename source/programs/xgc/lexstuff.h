/* $XFree86: xc/programs/xgc/lexstuff.h,v 1.3 2006/02/26 02:41:02 dawes Exp $ */

#ifdef NEED_YYIN
extern FILE *yyin;
#endif

extern void yyerror(const char *);
#if !defined(YYBISON) && !(defined(YYBYACC) && defined(__NetBSD__))
extern int yyparse(void);
#endif
extern int yylex(void);

