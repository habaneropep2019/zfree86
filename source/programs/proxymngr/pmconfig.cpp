! $XFree86: xc/programs/proxymngr/pmconfig.cpp,v 1.2 2006/01/09 15:00:49 dawes Exp $
! proxy manager config file
!
! Each line has the format:
!    <serviceName> managed <startCommand>
!        or
!    <serviceName> unmanaged <proxyAddress>
!
lbx managed LBXPROXY
!
! substitute site-specific info
!xfwp unmanaged firewall:4444
