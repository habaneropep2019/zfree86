! $XFree86: xc/util/memleak/sparcsolstack.s,v 1.2 2006/01/09 15:01:57 dawes Exp $
	.seg	"text"
	.proc	16
	.globl	getStackPointer
getStackPointer:
	retl
	mov	%sp,%o0
	.globl	getFramePointer
getFramePointer:
	retl
	mov	%fp,%o0
