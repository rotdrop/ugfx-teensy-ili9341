#ifndef _GDISP_LLD_CONFIG_H
#define _GDISP_LLD_CONFIG_H

#if GFX_USE_GDISP

/*===========================================================================*/
/* Driver hardware support.                                                  */
/*===========================================================================*/

#define GDISP_PIN_RST 255
#define GDISP_PIN_LED 255

#define GDISP_HARDWARE_STREAM_WRITE GFXON
#define GDISP_HARDWARE_STREAM_READ  GFXON
#define GDISP_HARWWARE_DRAWPIXEL    GFXON
#define GDISP_HARDWARE_BITFILLS     GFXON
#define GDISP_HARDWARE_READAREA     GFXON
#define GDISP_HARDWARE_PIXELREAD    GFXON
#define GDISP_HARDWARE_FILLS        GFXON
#define GDISP_HARDWARE_CLEARS       GFXON
#define GDISP_HARDWARE_CONTROL      GFXON

#define GDISP_LLD_PIXELFORMAT       GDISP_PIXELFORMAT_RGB565

#endif	/* GFX_USE_GDISP */

#endif	/* _GDISP_LLD_CONFIG_H */
