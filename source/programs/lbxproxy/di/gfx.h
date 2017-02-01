/* $XFree86: xc/programs/lbxproxy/di/gfx.h,v 1.2 2005/03/28 02:51:13 dawes Exp $ */

#ifndef _GFX_H_
#define _GFX_H_

extern int  ProcLBXChangeWindowAttributes(ClientPtr client),
	    ProcLBXGetWindowAttributes(ClientPtr client),
	    ProcLBXGetGeometry(ClientPtr client),
            ProcLBXAllocColorCells(ClientPtr client),
            ProcLBXAllocColorPlanes(ClientPtr client),
	    ProcLBXCopyArea(ClientPtr client),
	    ProcLBXCopyPlane(ClientPtr client),
            ProcLBXPolyPoint(ClientPtr client),
            ProcLBXPolyLine(ClientPtr client),
            ProcLBXPolySegment(ClientPtr client),
            ProcLBXPolyRectangle(ClientPtr client),
            ProcLBXPolyArc(ClientPtr client),
            ProcLBXFillPoly(ClientPtr client),
            ProcLBXPolyFillRectangle(ClientPtr client),
            ProcLBXPolyFillArc(ClientPtr client),
	    ProcLBXPolyText(ClientPtr client),
	    ProcLBXImageText(ClientPtr client),
	    ProcLBXGetImage(ClientPtr client),
	    ProcLBXPutImage(ClientPtr client);

/* tables */
extern int (*InitialVector[3]) (ClientPtr client);

#endif /* _GFX_H_ */
