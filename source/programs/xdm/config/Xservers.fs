# $XFree86: xc/programs/xdm/config/Xservers.fs,v 1.2 2006/01/09 15:01:06 dawes Exp $
#
# Xservers file, fileserver prototype (any machine without a display)
#
# Put X terminals which don't support XDMCP in this file; you will
# want to leave those terminals on and connected to the network, else
# xdm will have a tougher time managing them.
#
# Each line should look like:
#
#	XterminalName:0 foreign
