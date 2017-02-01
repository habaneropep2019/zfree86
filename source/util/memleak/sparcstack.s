# $XFree86: xc/util/memleak/sparcstack.s,v 1.2 2006/01/09 15:01:57 dawes Exp $
	.seg	"text"
	.proc	16
	.globl	_getStackPointer
_getStackPointer:
	retl
	mov	%sp,%o0
	.globl	_getFramePointer
_getFramePointer:
	retl
	mov	%fp,%o0
