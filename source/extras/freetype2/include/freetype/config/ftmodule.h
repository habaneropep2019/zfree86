/* $XFree86: xc/extras/freetype2/include/freetype/config/ftmodule.h,v 1.2 2007/05/18 18:01:57 tsi Exp $ */

FT_USE_MODULE(autohint_module_class)
FT_USE_MODULE(cff_driver_class)
#ifndef FONTMODULE
FT_USE_MODULE(t1cid_driver_class)
#endif
FT_USE_MODULE(pcf_driver_class)
FT_USE_MODULE(bdf_driver_class)
FT_USE_MODULE(psaux_module_class)
FT_USE_MODULE(psnames_module_class)
FT_USE_MODULE(pshinter_module_class)
FT_USE_MODULE(ft_raster1_renderer_class)
FT_USE_MODULE(sfnt_module_class)
#ifndef FONTMODULE
FT_USE_MODULE(ft_smooth_renderer_class)
FT_USE_MODULE(ft_smooth_lcd_renderer_class)
FT_USE_MODULE(ft_smooth_lcdv_renderer_class)
#endif
FT_USE_MODULE(tt_driver_class)
FT_USE_MODULE(t1_driver_class)
#ifndef FONTMODULE
FT_USE_MODULE(t42_driver_class)
FT_USE_MODULE(pfr_driver_class)
FT_USE_MODULE(winfnt_driver_class)
#endif

