/*

Copyright (c) 1989  X Consortium

Permission is hereby granted, free of charge, to any person obtaining
a copy of this software and associated documentation files (the
"Software"), to deal in the Software without restriction, including
without limitation the rights to use, copy, modify, merge, publish,
distribute, sublicense, and/or sell copies of the Software, and to
permit persons to whom the Software is furnished to do so, subject to
the following conditions:

The above copyright notice and this permission notice shall be included
in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
IN NO EVENT SHALL THE X CONSORTIUM BE LIABLE FOR ANY CLAIM, DAMAGES OR
OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
OTHER DEALINGS IN THE SOFTWARE.

Except as contained in this notice, the name of the X Consortium shall
not be used in advertising or otherwise to promote the sale, use or
other dealings in this Software without prior written authorization
from the X Consortium.

*/
/* $XFree86: xc/programs/xcalc/actions.c,v 1.2 2005/03/25 02:22:59 dawes Exp $ */

/*
 * actions.c - externally available procedures for xcalc
 *
 * Author:  Donna Converse, MIT X Consortium
 */

#include <X11/Intrinsic.h>
#include <setjmp.h>
#include "xcalc.h"
#include "actions.h"
#include "xcmath.h"

#define XCALC_PRE_OP(keynum) if (pre_op(keynum)) return;

/*ARGSUSED*/
void
add(Widget w, XEvent *e, String *vector, Cardinal *count)
{
    XCALC_PRE_OP(kADD);
    rpn ? twof(kADD) : twoop(kADD);
    post_op();
}

/*ARGSUSED*/
void
back(Widget w, XEvent *e, String *vector, Cardinal *count)
{
    XCALC_PRE_OP(kBKSP);
    bkspf();
    post_op();
}

/*ARGSUSED*/
void
bell(Widget w, XEvent *e, String *vector, Cardinal *count)
{
    ringbell();
}

/*ARGSUSED*/
void
clearit(Widget w, XEvent *e, String *vector, Cardinal *count)
{
    XCALC_PRE_OP(kCLR);
    clearf();
    post_op();
}
   
/*ARGSUSED*/
void
cosine(Widget w, XEvent *e, String *vector, Cardinal *count)
{
    XCALC_PRE_OP(kCOS);
    oneop(kCOS);
    post_op();
}

/*ARGSUSED*/
void
decimal(Widget w, XEvent *e, String *vector, Cardinal *count)
{
    XCALC_PRE_OP(kDEC);
    decf();
    post_op();
}

/*ARGSUSED*/
void
degree(Widget w, XEvent *e, String *vector, Cardinal *count)
{
    XCALC_PRE_OP(kDRG);
    drgf();
    post_op();
}

/*ARGSUSED*/
void
digit(Widget w, XEvent *e, String *vector, Cardinal *count)
{
    switch (vector[0][0])
    {
      case '1':	XCALC_PRE_OP(kONE); numeric(kONE); break;
      case '2': XCALC_PRE_OP(kTWO); numeric(kTWO); break;
      case '3': XCALC_PRE_OP(kTHREE); numeric(kTHREE); break;
      case '4': XCALC_PRE_OP(kFOUR); numeric(kFOUR); break;
      case '5': XCALC_PRE_OP(kFIVE); numeric(kFIVE); break;
      case '6': XCALC_PRE_OP(kSIX); numeric(kSIX); break;
      case '7': XCALC_PRE_OP(kSEVEN); numeric(kSEVEN); break;
      case '8': XCALC_PRE_OP(kEIGHT); numeric(kEIGHT); break;
      case '9': XCALC_PRE_OP(kNINE); numeric(kNINE); break;
      case '0': XCALC_PRE_OP(kZERO); numeric(kZERO); break;
    }
    post_op();
}

/*ARGSUSED*/
void
divide(Widget w, XEvent *e, String *vector, Cardinal *count)
{
    XCALC_PRE_OP(kDIV);
    rpn  ? twof(kDIV) : twoop(kDIV);
    post_op();
}

/*ARGSUSED*/
void
e(Widget w, XEvent *e, String *vector, Cardinal *count)
{
    XCALC_PRE_OP(kE);
    oneop(kE);
    post_op();
}

/*ARGSUSED*/
void
enter(Widget w, XEvent *e, String *vector, Cardinal *count)
{
    XCALC_PRE_OP(kENTR);
    entrf();
    post_op();
}

/*ARGSUSED*/
void
epower(Widget w, XEvent *e, String *vector, Cardinal *count)
{
    XCALC_PRE_OP(kEXP);
    oneop(kEXP);
    post_op();
}

/*ARGSUSED*/
void
equal(Widget w, XEvent *e, String *vector, Cardinal *count)
{
    XCALC_PRE_OP(kEQU);
    equf();
    post_op();
}

/*ARGSUSED*/
void
exchange(Widget w, XEvent *e, String *vector, Cardinal *count)
{
    XCALC_PRE_OP(kEXC);
    oneop(kEXC);
    post_op();
}
   
/*ARGSUSED*/
void
factorial(Widget w, XEvent *e, String *vector, Cardinal *count)
{
    XCALC_PRE_OP(kFACT);
    oneop(kFACT);
    post_op();
}

/*ARGSUSED*/
void
inverse(Widget w, XEvent *e, String *vector, Cardinal *count)
{
    XCALC_PRE_OP(kINV);
    invf();
    post_op();
}
   
/*ARGSUSED*/
void
leftParen(Widget w, XEvent *e, String *vector, Cardinal *count)
{
    XCALC_PRE_OP(kLPAR);
    lparf();
    post_op();
}
   
/*ARGSUSED*/
void
logarithm(Widget w, XEvent *e, String *vector, Cardinal *count)
{
    XCALC_PRE_OP(kLOG);
    oneop(kLOG);
    post_op();
}

/*ARGSUSED*/
void
multiply(Widget w, XEvent *e, String *vector, Cardinal *count)
{
    XCALC_PRE_OP(kMUL);
    rpn ? twof(kMUL) : twoop(kMUL);
    post_op();
}
   
/*ARGSUSED*/
void
naturalLog(Widget w, XEvent *e, String *vector, Cardinal *count)
{
    XCALC_PRE_OP(kLN);
    oneop(kLN);
    post_op();
}
   
/*ARGSUSED*/
void
negate(Widget w, XEvent *e, String *vector, Cardinal *count)
{
    XCALC_PRE_OP(kNEG);
    negf();
    post_op();
}

/*ARGSUSED*/
void
nop(Widget w, XEvent *e, String *vector, Cardinal *count)
{
    ringbell();
}

/*ARGSUSED*/
void
off(Widget w, XEvent *e, String *vector, Cardinal *count)
{
    XCALC_PRE_OP(kOFF);
    offf();
    post_op();
}
   
/*ARGSUSED*/
void
pi(Widget w, XEvent *e, String *vector, Cardinal *count)
{
    XCALC_PRE_OP(kPI);
    oneop(kPI);
    post_op();
}
   
/*ARGSUSED*/
void
power(Widget w, XEvent *e, String *vector, Cardinal *count)
{
    XCALC_PRE_OP(kPOW);
    rpn ? twof(kPOW) : twoop(kPOW);
    post_op();
}

/*ARGSUSED*/
void
quit(Widget w, XEvent *e, String *vector, Cardinal *count)
{
    if (e->type == ClientMessage && e->xclient.data.l[0] != wm_delete_window)
	ringbell();
    else
	Quit();
}

/*ARGSUSED*/
void
recall(Widget w, XEvent *e, String *vector, Cardinal *count)
{
    XCALC_PRE_OP(kRCL);
    rpn ? memf(kRCL) : oneop(kRCL);
    post_op();
}
	
/*ARGSUSED*/
void
reciprocal(Widget w, XEvent *e, String *vector, Cardinal *count)
{
    XCALC_PRE_OP(kRECIP);
    oneop(kRECIP);
    post_op();
}
   
/*ARGSUSED*/
void
rightParen(Widget w, XEvent *e, String *vector, Cardinal *count)
{
    XCALC_PRE_OP(kRPAR);
    rparf();
    post_op();
}
   
/*ARGSUSED*/
void
roll(Widget w, XEvent *e, String *vector, Cardinal *count)
{
    XCALC_PRE_OP(kROLL);
    rollf();
    post_op();
}

/*ARGSUSED*/
void
scientific(Widget w, XEvent *e, String *vector, Cardinal *count)
{
    XCALC_PRE_OP(kEE);
    eef();
    post_op();
}

/*ARGSUSED*/
void
selection(Widget w, XEvent *e, String *vector, Cardinal *count)
{
    do_select(((XButtonReleasedEvent *)e)->time);
}

/*ARGSUSED*/
void
sine(Widget w, XEvent *e, String *vector, Cardinal *count)
{
    XCALC_PRE_OP(kSIN);
    oneop(kSIN);
    post_op();
}

/*ARGSUSED*/
void
square(Widget w, XEvent *e, String *vector, Cardinal *count)
{
    XCALC_PRE_OP(kSQR);
    oneop(kSQR);
    post_op();
}

/*ARGSUSED*/
void
squareRoot(Widget w, XEvent *e, String *vector, Cardinal *count)
{
    XCALC_PRE_OP(kSQRT);
    oneop(kSQRT);
    post_op();
}
   
/*ARGSUSED*/
void
store(Widget w, XEvent *e, String *vector, Cardinal *count)
{
    XCALC_PRE_OP(kSTO);
    rpn ? memf(kSTO) : oneop(kSTO);
    post_op();
}

/*ARGSUSED*/
void
subtract(Widget w, XEvent *e, String *vector, Cardinal *count)
{
    XCALC_PRE_OP(kSUB);
    rpn ? twof(kSUB) : twoop(kSUB);
    post_op();
}
   
/*ARGSUSED*/
void
sum(Widget w, XEvent *e, String *vector, Cardinal *count)
{
    XCALC_PRE_OP(kSUM);
    rpn ? memf(kSUM) : oneop(kSUM);
    post_op();
}
   
/*ARGSUSED*/
void
tangent(Widget w, XEvent *e, String *vector, Cardinal *count)
{
    XCALC_PRE_OP(kTAN);
    oneop(kTAN);
    post_op();
}
   
/*ARGSUSED*/
void
tenpower(Widget w, XEvent *e, String *vector, Cardinal *count)
{
    XCALC_PRE_OP(k10X);
    oneop(k10X);
    post_op();
}
   
/*ARGSUSED*/
void
XexchangeY(Widget w, XEvent *e, String *vector, Cardinal *count)
{
    XCALC_PRE_OP(kXXY);
    twof(kXXY);
    post_op();
}
