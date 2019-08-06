/*
 * This file is subject to the terms of the GFX License. If a copy of
 * the license was not distributed with this file, you can obtain one at:
 *
 *              http://ugfx.io/license.html
 */

#include "gfx.h"
#include "ILI9341-driver.hh"

#if GFX_USE_GDISP

#if defined(GDISP_SCREEN_HEIGHT) || defined(GDISP_SCREEN_HEIGHT)
#if GFX_COMPILER_WARNING_TYPE == GFX_COMPILER_WARNING_DIRECT
#warning "GDISP: This low level driver does not support setting a screen size. It is being ignored."
#elif GFX_COMPILER_WARNING_TYPE == GFX_COMPILER_WARNING_MACRO
COMPILER_WARNING("GDISP: This low level driver does not support setting a screen size. It is being ignored.")
#endif
#undef GDISP_SCREEN_WIDTH
#undef GDISP_SCREEN_HEIGHT
#endif

#define GDISP_DRIVER_VMT			GDISPVMT_ILI9341
#include "gdisp_lld_config.h"
#include <ugfx/src/gdisp/gdisp_driver.h>

/*===========================================================================*/
/* Driver local definitions.                                                 */
/*===========================================================================*/

#ifndef GDISP_SCREEN_HEIGHT
#define GDISP_SCREEN_HEIGHT		320
#endif
#ifndef GDISP_SCREEN_WIDTH
#define GDISP_SCREEN_WIDTH		240
#endif
#ifndef GDISP_INITIAL_CONTRAST
#define GDISP_INITIAL_CONTRAST	50
#endif
#ifndef GDISP_INITIAL_BACKLIGHT
#define GDISP_INITIAL_BACKLIGHT	100
#endif

/*===========================================================================*/
/* Driver exported functions.                                                */
/*===========================================================================*/

static ILI9341Driver& getDriver(GDisplay *g)
{
  return *(ILI9341Driver *)g->board;
}

#define GDISP_PIN_CS  10
#define GDISP_PIN_DC   9

/**
 * @brief   Initialize the driver.
 * @return	gTrue if successful.
 * @param[in]	g					The driver structure
 * @param[out]	g->g				The driver must fill in the GDISPControl structure
 */
LLDSPEC gBool gdisp_lld_init(GDisplay *g)
{
  // No private area for this controller
  g->priv = nullptr;

  if (GDISP_DRIVER_VMT == nullptr) {
    return false;
  }

  // Initialise the board interface
  switch(g->controllerdisplay) {
  case 0:
    // Setup display 1
    int8_t rst=-1, led=-1;
#ifdef GDISP_PIN_RST
    rst = GDISP_PIN_RST;
#endif
#ifdef GDISP_PIN_LED
    led = GDISP_PIN_LED;
#endif
    auto& board = *new ILI9341Driver(GDISP_PIN_CS, GDISP_PIN_DC, rst, led);//, uint8_t _MOSI=11, uint8_t _SCLK=13, uint8_t _MISO=12);
    board.begin();
    g->board = &board;
    break;
  }

  return gTrue;
}

#if GDISP_HARDWARE_STREAM_WRITE
/**
 * @brief   Start a streamed write operation
 * @pre		GDISP_HARDWARE_STREAM_WRITE is GFXON
 *
 * @param[in]	g				The driver structure
 *
 * @note		g->p.x,g->p.y	The window position
 * @note		g->p.cx,g->p.cy	The window size
 *
 * @note		The parameter variables must not be altered by the driver.
 * @note		Streaming operations that wrap the defined window have
 * 				undefined results.
 * @note		This must be followed by a call to @p gdisp_lld_write_pos() if GDISP_HARDWARE_STREAM_POS is GFXON.
 */
LLDSPEC	void gdisp_lld_write_start(GDisplay *g)
{
  getDriver(g).startWrite(g->p.x, g->p.y, g->p.x + g->p.cx - 1, g->p.y + g->p.cy - 1);
}

/**
 * @brief   Send a pixel to the current streaming position and then increment that position
 * @pre		GDISP_HARDWARE_STREAM_WRITE is GFXON
 *
 * @param[in]	g				The driver structure
 *
 * @note		g->p.color		The color to display at the curent position
 * @note		The parameter variables must not be altered by the driver.
 */
LLDSPEC	void gdisp_lld_write_color(GDisplay *g)
{
  getDriver(g).pushColor(gdispColor2Native(g->p.color));
}

/**
 * @brief   End the current streaming write operation
 * @pre		GDISP_HARDWARE_STREAM_WRITE is GFXON
 *
 * @param[in]	g				The driver structure
 *
 * @note		The parameter variables must not be altered by the driver.
 */
LLDSPEC	void gdisp_lld_write_stop(GDisplay *g)
{
  getDriver(g).endWrite();
}

# if GDISP_HARDWARE_STREAM_POS
/**
 * @brief   Change the current position within the current streaming window
 * @pre		GDISP_HARDWARE_STREAM_POS is GFXON and GDISP_HARDWARE_STREAM_WRITE is GFXON
 *
 * @param[in]	g				The driver structure
 * @param[in]	g->p.x,g->p.y	The new position (which will always be within the existing stream window)
 *
 * @note		The parameter variables must not be altered by the driver.
 */
// cH: FIXME, think this cannot work with ILI9341
LLDSPEC	void gdisp_lld_write_pos(GDisplay *g);
# endif
#endif

#if GDISP_HARDWARE_STREAM_READ
/**
 * @brief   Start a streamed read operation
 * @pre		GDISP_HARDWARE_STREAM_READ is GFXON
 *
 * @param[in]	g				The driver structure
 * @param[in]	g->p.x,g->p.y	The window position
 * @param[in]	g->p.cx,g->p.cy	The window size
 *
 * @note		The parameter variables must not be altered by the driver.
 * @note		Streaming operations that wrap the defined window have
 * 				undefined results.
 */
LLDSPEC	void gdisp_lld_read_start(GDisplay *g)
{
  getDriver(g).startRead(g->p.x, g->p.y, g->p.x + g->p.cx - 1, g->p.y + g->p.cy - 1);
}

/**
 * @brief   Read a pixel from the current streaming position and then increment that position
 * @return	The color at the current position
 * @pre		GDISP_HARDWARE_STREAM_READ is GFXON
 *
 * @param[in]	g				The driver structure
 *
 * @note		The parameter variables must not be altered by the driver.
 */
LLDSPEC	gColor gdisp_lld_read_color(GDisplay *g)
{
  return getDriver(g).readColor();
}

/**
 * @brief   End the current streaming operation
 * @pre		GDISP_HARDWARE_STREAM_READ is GFXON
 *
 * @param[in]	g				The driver structure
 *
 * @note		The parameter variables must not be altered by the driver.
 */
LLDSPEC	void gdisp_lld_read_stop(GDisplay *g)
{
  getDriver(g).endRead();
}
#endif

#if GDISP_HARDWARE_DRAWPIXEL
/**
 * @brief   Draw a pixel
 * @pre		GDISP_HARDWARE_DRAWPIXEL is GFXON
 *
 * @param[in]	g				The driver structure
 * @param[in]	g->p.x,g->p.y	The pixel position
 * @param[in]	g->p.color		The color to set
 *
 * @note		The parameter variables must not be altered by the driver.
 */
LLDSPEC	void gdisp_lld_draw_pixel(GDisplay *g)
{
  getDriver(g).drawPixel(g->p.x, g->p.y, g->p.color);
}
#endif

#if GDISP_HARDWARE_CLEARS
/**
 * @brief   Clear the screen using the defined color
 * @pre		GDISP_HARDWARE_CLEARS is GFXON
 *
 * @param[in]	g				The driver structure
 * @param[in]	g->p.color		The color to set
 *
 * @note		The parameter variables must not be altered by the driver.
 */
LLDSPEC	void gdisp_lld_clear(GDisplay *g)
{
  getDriver(g).fillScreen(g->p.color);
}
#endif

#if GDISP_HARDWARE_FILLS
/**
 * @brief   Fill an area with a single color
 * @pre		GDISP_HARDWARE_FILLS is GFXON
 *
 * @param[in]	g				The driver structure
 * @param[in]	g->p.x,g->p.y	The area position
 * @param[in]	g->p.cx,g->p.cy	The area size
 * @param[in]	g->p.color		The color to set
 *
 * @note		The parameter variables must not be altered by the driver.
 */
LLDSPEC	void gdisp_lld_fill_area(GDisplay *g)
{
  getDriver(g).fillRect(g->p.x, g->p.y, g->p.cx, g->p.cy, g->p.color);
}
#endif

#if GDISP_HARDWARE_BITFILLS
/**
 * @brief   Fill an area using a bitmap
 * @pre		GDISP_HARDWARE_BITFILLS is GFXON
 *
 * @param[in]	g				The driver structure
 * @param[in]	g->p.x,g->p.y	The area position
 * @param[in]	g->p.cx,g->p.cy	The area size
 * @param[in]	g->p.x1,g->p.y1	The starting position in the bitmap
 * @param[in]	g->p.x2			The width of a bitmap line
 * @param[in]	g->p.ptr		The pointer to the bitmap
 *
 * @note		The parameter variables must not be altered by the driver.
 */
LLDSPEC	void gdisp_lld_blit_area(GDisplay *g)
{
  auto buffer = (gPixel*)g->p.ptr + g->p.x1 + g->p.y1 * g->p.x2;
  getDriver(g).blitRect(g->p.x, g->p.y, g->p.cx, g->p.cy, buffer, g->p.x2);
}
#endif

#if GDISP_HARDWARE_READAREA
/**
 * @brief   Read an area into a bitmap
 * @pre		GDISP_HARDWARE_READAREA is GFXON
 *
 * @param[in]	g				The driver structure
 * @param[in]	g->p.x,g->p.y	The area position
 * @param[in]	g->p.cx,g->p.cy	The area size
 * @param[in]	g->p.ptr	The pointer to the bitmap
 *
 * @note		The parameter variables must not be altered by the driver.
 */
LLDSPEC	void gdisp_lld_read_area(GDisplay *g)
{
  getDriver(g).readRect(g->p.x, g->p.y, g->p.cx, g->p.cy, (uint16_t *)g->p.ptr);
}
#endif

#if GDISP_HARDWARE_PIXELREAD
/**
 * @brief   Read a pixel from the display
 * @return	The color at the defined position
 * @pre		GDISP_HARDWARE_PIXELREAD is GFXON (and the application needs it)
 *
 * @param[in]	g				The driver structure
 * @param[in]	g->p.x,g->p.y	The pixel position
 *
 * @note		The parameter variables must not be altered by the driver.
 */
LLDSPEC	gColor gdisp_lld_get_pixel_color(GDisplay *g)
{
  return getDriver(g).readPixel(g->p.x, g->p.y);
}

#endif

#if (GDISP_HARDWARE_SCROLL && GDISP_NEED_SCROLL)
/**
 * @brief   Scroll an area of the screen
 * @pre		GDISP_HARDWARE_SCROLL is GFXON (and the application needs it)
 *
 * @param[in]	g				The driver structure
 * @param[in]	g->p.x,g->p.y	The area position
 * @param[in]	g->p.cx,g->p.cy	The area size
 * @param[in]	g->p.y1			The number of lines to scroll (positive or negative)
 *
 * @note		The parameter variables must not be altered by the driver.
 * @note		This can be easily implemented if the hardware supports
 * 				display area to display area copying.
 * @note		Clearing the exposed area on the scroll operation is not
 * 				needed as the high level code handles this.
 */
LLDSPEC	void gdisp_lld_vertical_scroll(GDisplay *g);
#endif

#if (GDISP_HARDWARE_QUERY && GDISP_NEED_QUERY)
/**
 * @brief   Query some feature of the hardware
 * @return	The information requested (typecast as void *)
 * @pre		GDISP_HARDWARE_QUERY is GFXON (and the application needs it)
 *
 * @param[in]	g				The driver structure
 * @param[in]	g->p.x			What to query
 *
 * @note		The parameter variables must not be altered by the driver.
 */
LLDSPEC	void *gdisp_lld_query(GDisplay *g);				// Uses p.x (=what);
#endif

#if (GDISP_HARDWARE_CLIP && (GDISP_NEED_CLIP || GDISP_NEED_VALIDATION))
/**
 * @brief   Set the hardware clipping area
 * @pre		GDISP_HARDWARE_CLIP is GFXON (and the application needs it)
 *
 * @param[in]	g				The driver structure
 * @param[in]	g->p.x,g->p.y	The area position
 * @param[in]	g->p.cx,g->p.cy	The area size
 *
 * @note		The parameter variables must not be altered by the driver.
 */
LLDSPEC	void gdisp_lld_set_clip(GDisplay *g);
#endif

#if (GDISP_HARDWARE_CONTROL && GDISP_NEED_CONTROL)
/**
 * @brief   Control some feature of the hardware
 * @pre		GDISP_HARDWARE_CONTROL is GFXON (and the application needs it)
 *
 * @param[in]	g				The driver structure
 * @param[in]	g->p.x			The operation to perform
 * @param[in]	g->p.ptr		The operation parameter
 *
 * @note		The parameter variables must not be altered by the driver.
 */
LLDSPEC void gdisp_lld_control(GDisplay *g) {
  switch(g->p.x) {
  case GDISP_CONTROL_POWER: {
    gPowermode requestedMode = (gPowermode)(uint32_t)g->p.ptr;
    if (g->g.Powermode == requestedMode)
      return;
    switch(requestedMode) {
    case gPowerOff:
    case gPowerSleep:
    case gPowerDeepSleep:
      getDriver(g).sleep(true);
      break;
    case gPowerOn:
      getDriver(g).sleep(false);
      break;
    default:
      return;
    }
    g->g.Powermode = requestedMode;
    return;
  }
  case GDISP_CONTROL_ORIENTATION: {
    gOrientation requestedOrientation = (gOrientation)(uint32_t)g->p.ptr;
    if (g->g.Orientation == requestedOrientation)
      return;
    switch(requestedOrientation) {
    case gOrientation0:
      getDriver(g).setRotation(0);
      g->g.Height = GDISP_SCREEN_HEIGHT;
      g->g.Width = GDISP_SCREEN_WIDTH;
      break;
    case gOrientation90:
      getDriver(g).setRotation(1);
      g->g.Height = GDISP_SCREEN_WIDTH;
      g->g.Width = GDISP_SCREEN_HEIGHT;
      break;
    case gOrientation180:
      getDriver(g).setRotation(2);
      g->g.Height = GDISP_SCREEN_HEIGHT;
      g->g.Width = GDISP_SCREEN_WIDTH;
      break;
    case gOrientation270:
      getDriver(g).setRotation(2);
      g->g.Height = GDISP_SCREEN_WIDTH;
      g->g.Width = GDISP_SCREEN_HEIGHT;
      break;
    default:
      return;
    }
    g->g.Orientation = requestedOrientation;
    return;
  }
  case GDISP_CONTROL_BACKLIGHT: {
    uint8_t requestedBacklight = (uint8_t)(uint32_t)g->p.ptr;
    if (requestedBacklight > 100) {
      requestedBacklight = 100;
    }
    getDriver(g).setBacklight(requestedBacklight);
    g->g.Backlight = requestedBacklight;
    return;
  }

    //case GDISP_CONTROL_CONTRAST:
  default:
    return;
  }
}
#endif

#if 0
Required Routines:

    gdisp_lld_init() - Initialise the controller and display
    gdisp_lld_write_start() - Start a windowed write operation
    gdisp_lld_write_color() - Send one pixel to the current window at the current position
    gdisp_lld_write_stop() - Stop a windowed write operation

Optional Routines:

    gdisp_lld_write_pos() - Set the current position within the write window (increases drawing efficiency)
    gdisp_lld_read_start() - Start a windowed read operation
    gdisp_lld_read_color() - Read one pixel from the current window at the current position
    gdisp_lld_read_stop() - Stop a windowed read operation
    gdisp_lld_set_clip() - Set a hardware clipping region. All writes are clipped to this area (regardless of the current window)
    gdisp_lld_control() - Handle backlight, contrast, screen orientation and any driver specific control commands
    gdisp_lld_query() - Query some driver specific value
    Any Point and Block Model routines as described below.

  Required Routines:

    gdisp_lld_init() - Initialise the controller and display
    gdisp_lld_draw_pixel() - Set a single pixel

Optional Routines:

    gdisp_lld_fill_area() - Fill a block with a color
    gdisp_lld_blit_area() - Fill a block from an array of pixels
    gdisp_lld_vertical_scroll() - Scroll up or down a windowed area of the display
    gdisp_lld_get_pixel_color() - Get the color of a single pixel
    gdisp_lld_set_clip() - Set a hardware clipping region. All writes are clipped to this area.
    gdisp_lld_control() - Handle backlight, contrast, screen orientation and any driver specific control commands
    gdisp_lld_query() - Query some driver specific value
#endif

#endif /* GFX_USE_GDISP */

// Local Variables: ***
// c-basic-offset: 2 ***
// indent-tabs-mode: nil ***
// End: ***
