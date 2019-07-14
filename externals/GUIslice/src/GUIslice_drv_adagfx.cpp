// =======================================================================
// GUIslice library (driver layer for Adafruit-GFX)
// - Calvin Hass
// - https://www.impulseadventure.com/elec/guislice-gui.html
// - https://github.com/ImpulseAdventure/GUIslice
// =======================================================================
//
// The MIT License
//
// Copyright 2016-2019 Calvin Hass
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
//
// =======================================================================
/// \file GUIslice_drv_adagfx.cpp


// Compiler guard for requested driver
#include "GUIslice_config.h" // Sets DRV_DISP_*
#if defined(DRV_DISP_ADAGFX) || defined(DRV_DISP_ADAGFX_AS)

// =======================================================================
// Driver Layer for Adafruit-GFX
// =======================================================================

// GUIslice library
#include "GUIslice_drv_adagfx.h"

#include <stdio.h>

// ------------------------------------------------------------------------
// Load display drivers
// ------------------------------------------------------------------------
#if defined(DRV_DISP_ADAGFX)

  // Almost all GFX-compatible libraries depend on Adafruit-GFX
  // There are a couple exceptions that do not require it
  #if !defined(DRV_DISP_ADAGFX_ILI9341_T3)
    #include <Adafruit_GFX.h>
    #include <gfxfont.h>
  #endif // ILI9341_T3

  // Now configure specific display driver for Adafruit-GFX
  #if defined(DRV_DISP_ADAGFX_ILI9341)
    #include <Adafruit_ILI9341.h>
    #include <SPI.h>
  #elif defined(DRV_DISP_ADAGFX_ILI9341_8BIT)
    #include <Adafruit_TFTLCD.h>
    #include <SPI.h>
  #elif defined(DRV_DISP_ADAGFX_ILI9341_T3)
    #include <ILI9341_t3.h>
    #include <SPI.h>
  #elif defined(DRV_DISP_ADAGFX_ILI9341_DUE_MB)
    #include <ILI9341_due.h>
    #include <SPI.h>
    #include <SystemFont5x7.h> // FIXME: Add support for user-defined fonts
  #elif defined(DRV_DISP_ADAGFX_SSD1306)
    #include <Adafruit_SSD1306.h>
    // TODO: Select either SPI or I2C. For now, assume SPI
    #include <SPI.h>
    #include <Wire.h>
  #elif defined(DRV_DISP_ADAGFX_ST7735)
    #include <Adafruit_ST7735.h>
    #include <SPI.h>
  #elif defined(DRV_DISP_ADAGFX_HX8347)
    #include <HX8347D_kbv.h>
    #include <SPI.h>
  #elif defined(DRV_DISP_ADAGFX_HX8357)
    #include <Adafruit_HX8357.h>
    // TODO: Select either SPI or I2C. For now, assume SPI
    #include <SPI.h>
  #elif defined(DRV_DISP_ADAGFX_PCD8544)
    #include <Adafruit_PCD8544.h>
    #include <SPI.h>
  #elif defined(DRV_DISP_ADAGFX_RA8875)
    #include <Adafruit_RA8875.h>
  #elif defined(DRV_DISP_ADAGFX_MCUFRIEND)
    #include <MCUFRIEND_kbv.h>

  #else
    #error "CONFIG: Need to enable a supported DRV_DISP_ADAGFX_* option in GUIslice_config_ard.h"
  #endif

  // Load any additional drivers
  #ifdef DRV_DISP_ADAGFX_SEESAW_18
    #define DRV_DISP_ADAGFX_SEESAW
    #include <Adafruit_seesaw.h>
    // Seesaw config specific to Adafruit 1.8" TFT shield
    #include <Adafruit_TFTShield18.h>
  #endif

#elif defined(DRV_DISP_ADAGFX_AS)
  #include <Adafruit_GFX_AS.h>

  // Now configure specific display driver for Adafruit-GFX-AS
  #if defined(DRV_DISP_ADAGFX_ILI9341_STM)
    #include <Adafruit_ILI9341_STM.h>
    #include <SPI.h>
  #else
    #error "CONFIG: Need to enable a supported DRV_DISP_ADAGFX_* option in GUIslice_config_ard.h"
  #endif

#endif

// ------------------------------------------------------------------------
// Load storage drivers
// - Support SD card interface
// ------------------------------------------------------------------------
#if (GSLC_SD_EN)
  #if (GSLC_SD_EN == 1)
    // Use Arduino built-in SD library
    // - Only supports HW SPI
    #include <SD.h>
  #elif (GSLC_SD_EN == 2)
    // Use greiman/SdFat library
    // - Supports SW SPI
    // - Recommend usage of SdFat library version 1.0.1
    // - To support SW SPI interface, need to make mod to SdFat lib:
    // -   Arduino\libraries\SdFat\src\SdFatConfig.h:
    // -     #define ENABLE_SOFTWARE_SPI_CLASS 1 // Change default from 0 to 1
    #include <SdFat.h>
    SdFatSoftSpi<12, 11, 13> SD; // FIXME: Add configurability
  #endif
#endif
 

// ------------------------------------------------------------------------
// Load touch drivers
// ------------------------------------------------------------------------
#if defined(DRV_TOUCH_ADA_STMPE610)
  #include <SPI.h>
  #include <Wire.h>
  #include "Adafruit_STMPE610.h"
#elif defined(DRV_TOUCH_ADA_FT6206)
  #include <Wire.h>
  #include "Adafruit_FT6206.h"
#elif defined(DRV_TOUCH_ADA_SIMPLE)
  #include <stdint.h>
  #include <TouchScreen.h>
#elif defined(DRV_TOUCH_XPT2046_STM)
  // NOTE: This file is located in the Arduino_STM32 library:
  //       Arduino_STM32/STM32F1/libraries/Serasidis_XPT2046_touch/src/XPT2046_touch.h
  #include <XPT2046_touch.h>
#elif defined(DRV_TOUCH_XPT2046_PS)
  #include <XPT2046_Touchscreen.h>
#elif defined(DRV_TOUCH_URTOUCH)
  #if defined(DRV_TOUCH_URTOUCH_OLD)
    #include <UTouch.h> // Select old version of URTouch
  #else
    #include <URTouch.h>
  #endif
#elif defined(DRV_TOUCH_HANDLER)
  #include <GUIslice_th.h>
#endif

// ------------------------------------------------------------------------


#ifdef __cplusplus
extern "C" {
#endif // __cplusplus



// ------------------------------------------------------------------------
#if defined(DRV_DISP_ADAGFX_ILI9341)
  #if (ADAGFX_SPI_HW) // Use hardware SPI or software SPI (with custom pins)
    const char* m_acDrvDisp = "ADA_ILI9341(SPI-HW)";
    Adafruit_ILI9341 m_disp = Adafruit_ILI9341(ADAGFX_PIN_CS, ADAGFX_PIN_DC, ADAGFX_PIN_RST);
  #else
    const char* m_acDrvDisp = "ADA_ILI9341(SPI-SW)";
    Adafruit_ILI9341 m_disp = Adafruit_ILI9341(ADAGFX_PIN_CS, ADAGFX_PIN_DC, ADAGFX_PIN_MOSI, ADAGFX_PIN_CLK, ADAGFX_PIN_RST, ADAGFX_PIN_MISO);
  #endif

// ------------------------------------------------------------------------
#elif defined(DRV_DISP_ADAGFX_ILI9341_8BIT)
  const char* m_acDrvDisp = "ADA_ILI9341_8b";
  Adafruit_TFTLCD m_disp = Adafruit_TFTLCD (ADAGFX_PIN_CS, ADAGFX_PIN_DC, ADAGFX_PIN_WR, ADAGFX_PIN_RD, ADAGFX_PIN_RST);

// ------------------------------------------------------------------------
#elif defined(DRV_DISP_ADAGFX_ILI9341_T3)
  #if (ADAGFX_SPI_HW)
    // Default hardware SPI pinout
    const char* m_acDrvDisp = "ADA_ILI9341_T3(SPI-HW)";
    ILI9341_t3 m_disp = ILI9341_t3(ADAGFX_PIN_CS, ADAGFX_PIN_DC, ADAGFX_PIN_RST);
  #else
    // Alternate hardware SPI pinout
    const char* m_acDrvDisp = "ADA_ILI9341_T3(SPI-HW-Alt)";
    ILI9341_t3 m_disp = ILI9341_t3 (ADAGFX_PIN_CS, ADAGFX_PIN_DC, ADAGFX_PIN_RST, ADAGFX_PIN_MOSI, ADAGFX_PIN_CLK, ADAGFX_PIN_MISO);
  #endif

// ------------------------------------------------------------------------
#elif defined(DRV_DISP_ADAGFX_ILI9341_STM)
  #if (ADAGFX_SPI_HW) // Use hardware SPI or software SPI (with custom pins)
    const char* m_acDrvDisp = "ADA_ILI9341_STM(SPI-HW)";
    //PIN_RST=-1 doesn't give same behavior as 2-param function variant, therefore use different functions
    #if ADAGFX_PIN_RST==-1
      Adafruit_ILI9341_STM m_disp = Adafruit_ILI9341_STM(ADAGFX_PIN_CS, ADAGFX_PIN_DC);
    #else
      Adafruit_ILI9341_STM m_disp = Adafruit_ILI9341_STM(ADAGFX_PIN_CS, ADAGFX_PIN_DC, ADAGFX_PIN_RST);
    #endif
  #else
    const char* m_acDrvDisp = "ADA_ILI9341_STM(SPI-SW)";
    Adafruit_ILI9341_STM m_disp = Adafruit_ILI9341_STM(ADAGFX_PIN_CS, ADAGFX_PIN_DC, ADAGFX_PIN_MOSI, ADAGFX_PIN_CLK, ADAGFX_PIN_RST, ADAGFX_PIN_MISO);
  #endif

// ------------------------------------------------------------------------
#elif defined(DRV_DISP_ADAGFX_ILI9341_DUE_MB)
  const char* m_acDrvDisp = "ADA_ILI9341_DUE_MB(SPI-HW)";
  ILI9341_due m_disp = ILI9341_due(ADAGFX_PIN_CS, ADAGFX_PIN_DC, ADAGFX_PIN_RST);

// ------------------------------------------------------------------------
#elif defined(DRV_DISP_ADAGFX_SSD1306)
  #if (ADAGFX_SPI_HW) // Use hardware SPI or software SPI (with custom pins)
    const char* m_acDrvDisp = "ADA_SSD1306(SPI-HW)";
    Adafruit_SSD1306 m_disp(ADAGFX_PIN_DC, ADAGFX_PIN_RST, ADAGFX_PIN_CS);
  #else
    const char* m_acDrvDisp = "ADA_SSD1306(SPI-SW)";
    Adafruit_SSD1306 m_disp(ADAGFX_PIN_MOSI, ADAGFX_PIN_CLK, ADAGFX_PIN_DC, ADAGFX_PIN_RST, ADAGFX_PIN_CS);
  #endif

// ------------------------------------------------------------------------
#elif defined(DRV_DISP_ADAGFX_ST7735)
  #if (ADAGFX_SPI_HW) // Use hardware SPI or software SPI (with custom pins)
    const char* m_acDrvDisp = "ADA_ST7735(SPI-HW)";
    Adafruit_ST7735 m_disp(ADAGFX_PIN_CS, ADAGFX_PIN_DC, ADAGFX_PIN_RST);
  #else
    const char* m_acDrvDisp = "ADA_ST7735(SPI-SW)";
    Adafruit_ST7735 m_disp(ADAGFX_PIN_CS, ADAGFX_PIN_DC, ADAGFX_PIN_MOSI, ADAGFX_PIN_CLK, ADAGFX_PIN_RST);
  #endif

  #ifdef DRV_DISP_ADAGFX_SEESAW_18
    Adafruit_TFTShield18 m_seesaw;
  #endif

// ------------------------------------------------------------------------
#elif defined(DRV_DISP_ADAGFX_HX8347)
  const char* m_acDrvDisp = "ADA_HX8347(SPI-HW)";
  HX8347D_kbv m_disp;

// ------------------------------------------------------------------------
#elif defined(DRV_DISP_ADAGFX_HX8357)
  #if (ADAGFX_SPI_HW) // Use hardware SPI or software SPI (with custom pins)
    const char* m_acDrvDisp = "ADA_HX8357(SPI-HW)";
    Adafruit_HX8357 m_disp(ADAGFX_PIN_CS, ADAGFX_PIN_DC, ADAGFX_PIN_RST);
  #else
    const char* m_acDrvDisp = "ADA_HX8357(SPI-SW)";
    Adafruit_HX8357 m_disp(ADAGFX_PIN_CS, ADAGFX_PIN_DC, ADAGFX_PIN_MOSI, ADAGFX_PIN_CLK, ADAGFX_PIN_RESET);
  #endif

// ------------------------------------------------------------------------
#elif defined(DRV_DISP_ADAGFX_PCD8544)
  #if (ADAGFX_SPI_HW) // Use hardware SPI or software SPI (with custom pins)
    const char* m_acDrvDisp = "ADA_PCD8544(SPI-HW)";
    Adafruit_PCD8544 m_disp(ADAGFX_PIN_DC, ADAGFX_PIN_CS, ADAGFX_PIN_RST);
  #else
    const char* m_acDrvDisp = "ADA_PCD8544(SPI-SW)";
    Adafruit_PCD8544 m_disp(ADAGFX_PIN_CLK, ADAGFX_PIN_MOSI, ADAGFX_PIN_DC, ADAGFX_PIN_CS, ADAGFX_PIN_RST);
  #endif

// ------------------------------------------------------------------------
#elif defined(DRV_DISP_ADAGFX_RA8875)
  const char* m_acDrvDisp = "ADA_RA8875(SPI-HW)";
  Adafruit_RA8875 m_disp(ADAGFX_PIN_CS, ADAGFX_PIN_RST);

// ------------------------------------------------------------------------
#elif defined(DRV_DISP_ADAGFX_MCUFRIEND)
  const char* m_acDrvDisp = "ADA_MCUFRIEND";
  MCUFRIEND_kbv m_disp;

// ------------------------------------------------------------------------
#endif // DRV_DISP_ADAGFX_*



// ------------------------------------------------------------------------
#if defined(DRV_TOUCH_ADA_STMPE610)
  #if (ADATOUCH_I2C_HW) // Use I2C
    const char* m_acDrvTouch = "STMPE610(I2C-HW)";
    Adafruit_STMPE610 m_touch = Adafruit_STMPE610();
  #elif (ADATOUCH_SPI_HW) // Use hardware SPI
    const char* m_acDrvTouch = "STMPE610(SPI-HW)";
    Adafruit_STMPE610 m_touch = Adafruit_STMPE610(ADATOUCH_PIN_CS);
  #elif (ADATOUCH_SPI_SW) // Use software SPI
    const char* m_acDrvTouch = "STMPE610(SPI-SW)";
    Adafruit_STMPE610 m_touch = Adafruit_STMPE610(ADATOUCH_PIN_CS, ADATOUCH_PIN_SDI, ADATOUCH_PIN_SDO, ADATOUCH_PIN_SCK);
  #else // No interface flag set
    #error "DRV_TOUCH_ADA_STMPE610 but no ADATOUCH_I2C_* or ADATOUCH_SPI_* set in config"
  #endif
// ------------------------------------------------------------------------
#elif defined(DRV_TOUCH_ADA_FT6206)
  const char* m_acDrvTouch = "FT6206(I2C)";
  // Always use I2C
  Adafruit_FT6206 m_touch = Adafruit_FT6206();
// ------------------------------------------------------------------------
#elif defined(DRV_TOUCH_ADA_SIMPLE)
  const char* m_acDrvTouch = "SIMPLE(Analog)";
  TouchScreen m_touch = TouchScreen(ADATOUCH_PIN_XP, ADATOUCH_PIN_YP, ADATOUCH_PIN_XM, ADATOUCH_PIN_YM, ADATOUCH_RX);
// ------------------------------------------------------------------------
#elif defined(DRV_TOUCH_ADA_RA8875)
  const char* m_acDrvTouch = "RA8875(internal)";
// ------------------------------------------------------------------------
#elif defined(DRV_TOUCH_XPT2046_STM)
  const char* m_acDrvTouch = "XPT2046_STM(SPI-HW)";
  // Create an SPI class for XPT2046 access
  XPT2046_DEFINE_DPICLASS;
  // XPT2046 driver from Arduino_STM32 by Serasidis (<XPT2046_touch.h>)
  XPT2046_touch m_touch(XPT2046_CS, XPT2046_spi); // Chip Select pin, SPI instance
// ------------------------------------------------------------------------
#elif defined(DRV_TOUCH_XPT2046_PS)
  const char* m_acDrvTouch = "XPT2046_PS(SPI-HW)";
  // Use SPI, no IRQs
  XPT2046_Touchscreen m_touch(XPT2046_CS); // Chip Select pin
// ------------------------------------------------------------------------
#elif defined(DRV_TOUCH_URTOUCH)
  #if defined(DRV_TOUCH_URTOUCH_OLD)
    const char* m_acDrvTouch = "URTOUCH_OLD";
    UTouch m_touch(DRV_TOUCH_URTOUCH_INIT);
  #else
    const char* m_acDrvTouch = "URTOUCH";
    URTouch m_touch(DRV_TOUCH_URTOUCH_INIT);
  #endif
// ------------------------------------------------------------------------
#elif defined(DRV_TOUCH_HANDLER)
  const char* m_acDrvTouch = "Handler";
// ------------------------------------------------------------------------
#elif defined(DRV_TOUCH_INPUT)
  const char* m_acDrvTouch = "INPUT";
// ------------------------------------------------------------------------
#elif defined(DRV_TOUCH_NONE)
  const char* m_acDrvTouch = "NONE";
// ------------------------------------------------------------------------
#endif // DRV_TOUCH_*



// =======================================================================
// Public APIs to GUIslice core library
// =======================================================================

// -----------------------------------------------------------------------
// Configuration Functions
// -----------------------------------------------------------------------

bool gslc_DrvInit(gslc_tsGui* pGui)
{

  // Report any debug info if enabled
  #if defined(DBG_DRIVER)
  // TODO
  #endif

  // Initialize any library-specific members
  if (pGui->pvDriver) {
    gslc_tsDriver*  pDriver = (gslc_tsDriver*)(pGui->pvDriver);

    pDriver->nColBkgnd = GSLC_COL_BLACK;

    // These displays can accept partial redraw as they retain the last
    // image in the controller graphics RAM
    pGui->bRedrawPartialEn = true;

    // Support any additional initialization prior to display init
    #ifdef DRV_DISP_ADAGFX_SEESAW
      // Special initialization for Adafruit Seesaw chip
      if (!m_seesaw.begin()) {
       GSLC_DEBUG_PRINT("ERROR: Adafruit seesaw not initialized", "");
      } else {
        #if !defined(INIT_MSG_DISABLE)
        GSLC_DEBUG_PRINT("- Adafruit seesaw OK\n", "");
        #endif
      }
      m_seesaw.setBacklight(TFTSHIELD_BACKLIGHT_OFF);
      m_seesaw.tftReset();
      m_seesaw.setBacklight(TFTSHIELD_BACKLIGHT_ON);
    #endif

    // Perform any display initialization
    #if defined(DRV_DISP_ADAGFX_ILI9341) || defined(DRV_DISP_ADAGFX_ILI9341_STM) || defined(DRV_DISP_ADAGFX_ILI9341_DUE_MB)

      #if (ADAGFX_SPI_SET) // Use extra SPI initialization (eg. on Teensy devices)
        // TODO: Consider check for GSLC_DEV_TEENSY
        // If ADAGFX_SPI_SET is enabled, then perform additional SPI initialization.
        // This may be required for certain pinouts with Teensy 3 devices.
        // If enabled, it must be done ahead of m_disp.begin()
        SPI.setMOSI(ADAGFX_PIN_MOSI);
        SPI.setSCK(ADAGFX_PIN_CLK);
      #endif

      m_disp.begin();
      m_disp.readcommand8(ILI9341_RDMODE);
      m_disp.readcommand8(ILI9341_RDMADCTL);
      m_disp.readcommand8(ILI9341_RDPIXFMT);
      m_disp.readcommand8(ILI9341_RDIMGFMT);
      m_disp.readcommand8(ILI9341_RDSELFDIAG);

    #elif defined(DRV_DISP_ADAGFX_ILI9341_8BIT)
      uint16_t identifier = m_disp.readID();
      m_disp.begin(identifier);

    #elif defined(DRV_DISP_ADAGFX_ILI9341_T3)
      m_disp.begin();

    #elif defined(DRV_DISP_ADAGFX_SSD1306)
      m_disp.begin(SSD1306_SWITCHCAPVCC);

    #elif defined(DRV_DISP_ADAGFX_ST7735)

      // ST7735 requires additional initialization depending on
      // display type. Enable the user to specify the
      // configuration via DRV_DISP_ADAGFX_ST7735_INIT.
      #ifndef DRV_DISP_ADAGFX_ST7735_INIT
        m_disp.initR(INITR_144GREENTAB);  // Default to Green Tab 1.44"
      #else
        m_disp.initR(DRV_DISP_ADAGFX_ST7735_INIT);
      #endif

    #elif defined(DRV_DISP_ADAGFX_HX8347)
      m_disp.begin();

    #elif defined(DRV_DISP_ADAGFX_HX8357)
      m_disp.begin(HX8357D);

    #elif defined(DRV_DISP_ADAGFX_PCD8544)
      m_disp.begin();
      //m_disp.setContrast(50); Set the contrast level

    #elif defined(DRV_DISP_ADAGFX_RA8875)
      // RA8875 requires additional initialization depending on
      // display type. Enable the user to specify the
      // configuration via DRV_DISP_ADAGFX_RA8875_INIT.
      bool bInitOk = true;
      #ifndef DRV_DISP_ADAGFX_RA8875_INIT
        bInitOk = m_disp.begin(RA8875_800x480);  // Default to 800x480
      #else
        bInitOk = m_disp.begin(DRV_DISP_ADAGFX_RA8875_INIT);
      #endif
      if (!bInitOk) {
        GSLC_DEBUG_PRINT("ERROR: RA8875 init failed\n", "");
        return false;
      }
      m_disp.displayOn(true);
      m_disp.GPIOX(true); // Enable TFT - display enable tied to GPIOX
      m_disp.PWM1config(true, RA8875_PWM_CLK_DIV1024); // PWM output for backlight
      m_disp.PWM1out(255);
      m_disp.graphicsMode(); // Go back to graphics mode

    #elif defined(DRV_DISP_ADAGFX_MCUFRIEND)
      uint16_t identifier = m_disp.readID();
      // Support override for MCUFRIEND ID auto-detection
      #if defined(DRV_DISP_ADAGFX_MCUFRIEND_FORCE)
        m_disp.begin(DRV_DISP_ADAGFX_MCUFRIEND_FORCE);
        #if !defined(INIT_MSG_DISABLE)
        GSLC_DEBUG_PRINT("- MCUfriend ID forced\n","");
        #endif
      #else
        m_disp.begin(identifier);
      #endif

    #endif

    // Now that we have initialized the display, we can assign
    // the rotation parameters and clipping region
    gslc_DrvRotate(pGui,GSLC_ROTATE);


    // Initialize SD card usage
    #if (GSLC_SD_EN)
    if (!SD.begin(ADAGFX_PIN_SDCS)) {
      GSLC_DEBUG_PRINT("ERROR: DrvInit() SD init failed\n",0);
      return false;
    }
    #endif

  }
  return true;
}


void gslc_DrvDestruct(gslc_tsGui* pGui)
{
}

const char* gslc_DrvGetNameDisp(gslc_tsGui* pGui)
{
  return m_acDrvDisp;
}

const char* gslc_DrvGetNameTouch(gslc_tsGui* pGui)
{
  return m_acDrvTouch;
}


// -----------------------------------------------------------------------
// Image/surface handling Functions
// -----------------------------------------------------------------------

void* gslc_DrvLoadImage(gslc_tsGui* pGui,gslc_tsImgRef sImgRef)
{
  // GUIslice adapter for Adafruit-GFX doesn't preload the
  // images into RAM (to keep RAM requirements low), so we
  // don't need to do any further processing here. Instead,
  // the loading is done during render.
  if (sImgRef.eImgFlags == GSLC_IMGREF_NONE) {
    return NULL;
  } else if ((sImgRef.eImgFlags & GSLC_IMGREF_SRC) == GSLC_IMGREF_SRC_FILE) {
    return NULL;  // No image preload done
  } else if ((sImgRef.eImgFlags & GSLC_IMGREF_SRC) == GSLC_IMGREF_SRC_SD) {
    return NULL;  // No image preload done
  } else if ((sImgRef.eImgFlags & GSLC_IMGREF_SRC) == GSLC_IMGREF_SRC_RAM) {
    return NULL;  // No image preload done
  } else if ((sImgRef.eImgFlags & GSLC_IMGREF_SRC) == GSLC_IMGREF_SRC_PROG) {
    return NULL;  // No image preload done
  }

  // Default
  return NULL;
}


bool gslc_DrvSetBkgndImage(gslc_tsGui* pGui,gslc_tsImgRef sImgRef)
{
  // Dispose of previous background
  if (pGui->sImgRefBkgnd.eImgFlags != GSLC_IMGREF_NONE) {
    gslc_DrvImageDestruct(pGui->sImgRefBkgnd.pvImgRaw);
    pGui->sImgRefBkgnd = gslc_ResetImage();
  }
  pGui->sImgRefBkgnd = sImgRef;
  pGui->sImgRefBkgnd.pvImgRaw = gslc_DrvLoadImage(pGui,sImgRef);
  if (pGui->sImgRefBkgnd.pvImgRaw == NULL) {
    GSLC_DEBUG2_PRINT("ERROR: DrvSetBkgndImage(%s) failed\n","");
    return false;
  }

  return true;
}


bool gslc_DrvSetBkgndColor(gslc_tsGui* pGui,gslc_tsColor nCol)
{
  if (pGui->pvDriver) {
    gslc_tsDriver*  pDriver = (gslc_tsDriver*)(pGui->pvDriver);
    pDriver->nColBkgnd = nCol;
  }
  return true;
}


bool gslc_DrvSetElemImageNorm(gslc_tsGui* pGui,gslc_tsElem* pElem,gslc_tsImgRef sImgRef)
{
  // This driver doesn't preload the image to memory,
  // so we just save the reference for loading upon render
  pElem->sImgRefNorm = sImgRef;
  return true; // TODO
}


bool gslc_DrvSetElemImageGlow(gslc_tsGui* pGui,gslc_tsElem* pElem,gslc_tsImgRef sImgRef)
{
  // This driver doesn't preload the image to memory,
  // so we just save the reference for loading upon render
  pElem->sImgRefGlow = sImgRef;
  return true; // TODO
}


void gslc_DrvImageDestruct(void* pvImg)
{
}

bool gslc_DrvSetClipRect(gslc_tsGui* pGui,gslc_tsRect* pRect)
{
  // NOTE: The clipping rect is currently saved in the
  // driver struct, but the drawing code does not currently
  // use it.
  gslc_tsDriver*  pDriver = (gslc_tsDriver*)(pGui->pvDriver);
  if (pRect == NULL) {
    // Default to entire display
    pDriver->rClipRect = {0,0,pGui->nDispW,pGui->nDispH};
  } else {
    pDriver->rClipRect = *pRect;
  }

  // TODO: For ILI9341, perhaps we can leverage m_disp.setAddrWindow(x0, y0, x1, y1)?
  return true;
}



// -----------------------------------------------------------------------
// Font handling Functions
// -----------------------------------------------------------------------

const void* gslc_DrvFontAdd(gslc_teFontRefType eFontRefType,const void* pvFontRef,uint16_t nFontSz)
{
  // Arduino mode currently only supports font definitions from memory
  if (eFontRefType != GSLC_FONTREF_PTR) {
    GSLC_DEBUG2_PRINT("ERROR: DrvFontAdd(%s) failed - Arduino only supports memory-based fonts\n","");
    return NULL;
  }
  // Return pointer to Adafruit-GFX GFXfont structure
  return pvFontRef;
}

void gslc_DrvFontsDestruct(gslc_tsGui* pGui)
{
  // Nothing to deallocate
}

bool gslc_DrvGetTxtSize(gslc_tsGui* pGui,gslc_tsFont* pFont,const char* pStr,gslc_teTxtFlags eTxtFlags,
        int16_t* pnTxtX,int16_t* pnTxtY,uint16_t* pnTxtSzW,uint16_t* pnTxtSzH)
{
  uint16_t  nTxtScale = 0;

#if defined(DRV_DISP_ADAGFX_ILI9341_T3)
  // Use PaulStoffregen/ILI9341_t3
  //
  // - IMPORTANT NOTE: Recent version of ILI9341_t3 library is required
  // - If you see a compilation error such as the following, then you
  //   need to update your ILI9341_t3 library to a more recent version:
  //     error: 'class ILI9341_t3' has no member named 'measureTextWidth'
  //     error: 'class ILI9341_t3' has no member named 'measureTextHeight'
  // - To update ILI9341_t3 to the latest, please follow the guidance here:
  //    https://github.com/ImpulseAdventure/GUIslice/wiki/Install-ILI9341_t3-for-Teensy

  // Fetch the string dimensions
  // - Note that the following APIs (measureTextHeight / measureTextWidth)
  //   were recently added to ILI9341_t3, so the latest version of
  //   the library from GitHub should be used.

  const ILI9341_t3_font_t* pT3Font = NULL;
  switch (pFont->eFontRefMode) {
  case GSLC_FONTREF_MODE_DEFAULT:
  default:
    // Default Adafruit-GFX font
    m_disp.setFontAdafruit();
    break;
  case GSLC_FONTREF_MODE_1:
    // T3 font
    pT3Font = (const ILI9341_t3_font_t*)(pFont->pvFont);
    m_disp.setFont(*pT3Font);
    break;
  }

  nTxtScale = pFont->nSize;
  m_disp.setTextSize(nTxtScale);

  // Fetch the font sizing
  *pnTxtSzW = m_disp.measureTextWidth(pStr,0);  // NOTE: If compile error, see note https://github.com/ImpulseAdventure/GUIslice/wiki/Install-ILI9341_t3-for-Teensy
  *pnTxtSzH = m_disp.measureTextHeight(pStr,0); // NOTE: If compile error, see note https://github.com/ImpulseAdventure/GUIslice/wiki/Install-ILI9341_t3-for-Teensy

  // Debug: report font sizing
  // GSLC_DEBUG2_PRINT("DBG:GetTxtSize: [%s] w=%d h=%d scale=%d\n",
  //   pStr,*pnTxtSzW,*pnTxtSzH,nTxtScale);

  // TODO: Support for extracting baseline info from Teensy fonts?
  *pnTxtX = 0;
  *pnTxtY = 0;

  return true;

#elif defined(DRV_DISP_ADAGFX_ILI9341_DUE_MB)
  // FIXME: Add support for user defined fonts
  m_disp.setFont(SystemFont5x7);

  nTxtScale = pFont->nSize;
  m_disp.setTextScale(nTxtScale);

  // Fetch the font sizing
  *pnTxtSzW = m_disp.getStringWidth(pStr);
  *pnTxtSzH = nTxtScale * m_disp.getFontHeight();

  // No baseline info
  *pnTxtX = 0;
  *pnTxtY = 0;

  return true;

#else

  nTxtScale = pFont->nSize;
  m_disp.setFont((const GFXfont *)pFont->pvFont);
  m_disp.setTextSize(nTxtScale);

  if ((eTxtFlags & GSLC_TXT_MEM) == GSLC_TXT_MEM_RAM) {
    // Fetch the text bounds
    m_disp.getTextBounds((char*)pStr,0,0,pnTxtX,pnTxtY,pnTxtSzW,pnTxtSzH);

  } else if ((eTxtFlags & GSLC_TXT_MEM) == GSLC_TXT_MEM_PROG) {
    #if (GSLC_USE_PROGMEM)
    uint16_t nTxtLen = strlen_P(pStr);
    char tempStr[nTxtLen+1];
    strncpy_P(tempStr,pStr,nTxtLen);
    tempStr[nTxtLen] = '\0';  // Force termination

    // Fetch the text bounds
    m_disp.getTextBounds(tempStr,0,0,pnTxtX,pnTxtY,pnTxtSzW,pnTxtSzH);
    #else
    // NOTE: Should not get here
    // - The text string has been marked as being stored in
    //   FLASH via PROGMEM (typically for Arduino) but
    //   the current device does not support the PROGMEM
    //   methodology.
    // - Degrade back to using SRAM directly

    // Fetch the text bounds
    m_disp.getTextBounds((char*)pStr,0,0,pnTxtX,pnTxtY,pnTxtSzW,pnTxtSzH);
    #endif
  }

  m_disp.setFont();
  return true;

#endif // DRV_DISP_*

}

bool gslc_DrvDrawTxt(gslc_tsGui* pGui,int16_t nTxtX,int16_t nTxtY,gslc_tsFont* pFont,const char* pStr,gslc_teTxtFlags eTxtFlags,gslc_tsColor colTxt, gslc_tsColor colBg=GSLC_COL_BLACK)
{
  uint16_t  nTxtScale = pFont->nSize;
  uint16_t  nColRaw = gslc_DrvAdaptColorToRaw(colTxt);
  char      ch;

  // Initialize the font and positioning
#if defined(DRV_DISP_ADAGFX_ILI9341_T3)
  const ILI9341_t3_font_t* pT3Font = NULL;
  switch (pFont->eFontRefMode) {
  case GSLC_FONTREF_MODE_DEFAULT:
  default:
    // Default Adafruit-GFX font
    m_disp.setFontAdafruit();
    break;
  case GSLC_FONTREF_MODE_1:
    // T3 font
    pT3Font = (const ILI9341_t3_font_t*)(pFont->pvFont);
    m_disp.setFont(*pT3Font);
    break;
  }
  m_disp.setTextColor(nColRaw);
  m_disp.setCursor(nTxtX,nTxtY);
  m_disp.setTextSize(nTxtScale);
#elif defined(DRV_DISP_ADAGFX_ILI9341_DUE_MB)
  // FIXME: Add support for user defined fonts
  m_disp.setFont(SystemFont5x7);
  m_disp.setTextColor(nColRaw);
  m_disp.cursorToXY(nTxtX,nTxtY);
  m_disp.setTextScale(nTxtScale);
#else
  m_disp.setFont((const GFXfont *)pFont->pvFont);
  m_disp.setTextColor(nColRaw);
  m_disp.setCursor(nTxtX,nTxtY);
  m_disp.setTextSize(nTxtScale);
#endif

  // Driver-specific overrides
  #if defined(DRV_DISP_ADAGFX_RA8875)
    // TODO: Add a mode to use the RA8875's internal ROM font
    // - Note that it is larger than the default Adafruit-GFX font
    //   so for now we won't use it. It is expected that it should
    //   be much faster.
    bool bInternal8875Font = false;

    if (bInternal8875Font) {
      uint16_t nColBgRaw = gslc_DrvAdaptColorToRaw(colBg);
      // Enter text mode when using RA8875 built-in fonts
      m_disp.textMode();
      nTxtScale = (nTxtScale > 0) ? nTxtScale : 0;
      // Adapt to RA8875 text scaling with 0-based notation
      m_disp.textEnlarge(nTxtScale - 1);
      m_disp.textColor(nColRaw, nColBgRaw);
      m_disp.textSetCursor(nTxtX, nTxtY);
    }
  #endif // DRV_DISP_*

  // Default to accessing RAM directly (GSLC_TXT_MEM_RAM)
  bool bProg = false;
  if ((eTxtFlags & GSLC_TXT_MEM) == GSLC_TXT_MEM_PROG) {
    bProg = true;
  }

  while (1) {
    // Fetch the next character
    if (!bProg) {
      // String in SRAM; can access buffer directly
      ch = *(pStr++);
    } else {
      // String in PROGMEM (flash); must access via pgm_* calls
      ch = pgm_read_byte(pStr++);
    }

    // Detect string terminator
    if (ch == 0) {
      break;
    }

    // Render the character
    #if defined(DRV_DISP_ADAGFX_RA8875)
      // When using custom fonts, we 
      if (bInternal8875Font) {
        // Use the default font, so call the RA8875 textWrite() function
        m_disp.textWrite((const char *)&ch, 1);
      } else {
        // Use Adafruit-GFX for rendering
        // - When using a custom Adafruit-GFX font with RA8875, we need to
        //   call the Adafruit-GFX base class write() function instead of
        //   the overloaded version in Adafruit_RA8875
        //   This is because Adafruit_RA8875 has overloaded the write() calls
        //   in a way that is not compatible with the Adafruit-GFX font rendering.
        m_disp.Adafruit_GFX::write(ch);
      }
    #else
      // Call Adafruit-GFX for rendering
      // NOTE: This should automatically advance the "cursor" (current text position)
      m_disp.print(ch);
    #endif

    // Handle multi-line text:
    // If we just output a newline, Adafruit-GFX will automatically advance
    // the Y cursor but reset the X cursor to 0. Therefore we need to
    // readjust the X cursor to our aligned bounding box.
    if (ch == '\n') {
      #if defined(DRV_DISP_ADAGFX_RA8875)
      int16_t nCurPosY = m_disp.getCursorY();
      if (bInternal8875Font) {
        // TODO: Is getCursorY() supported in RA8875 mode?
        m_disp.textSetCursor(nTxtX, nCurPosY);
      }
      #elif defined(DRV_DISP_ADAGFX_ILI9341_DUE_MB)
      nTxtY += m_disp.getFontHeight();
      m_disp.cursorToXY(nTxtX,nTxtY);
      #else
      int16_t nCurPosY = m_disp.getCursorY();
      m_disp.setCursor(nTxtX,nCurPosY);
      #endif
    }

  } // while(1)

  #if defined(DRV_DISP_ADAGFX_RA8875)
  if (bInternal8875Font) {
    // Return to RA8875 graphics mode
    m_disp.graphicsMode();
  }
  #endif // DRV_DISP_ADAGFX_RA8875

  // Restore the font
#if defined(DRV_DISP_ADAGFX_ILI9341_T3)
  // TODO
#elif defined(DRV_DISP_ADAGFX_ILI9341_DUE_MB)
  // TODO
#else
  m_disp.setFont();
#endif

  return true;
}

// -----------------------------------------------------------------------
// Screen Management Functions
// -----------------------------------------------------------------------

void gslc_DrvPageFlipNow(gslc_tsGui* pGui)
{
  #if defined(DRV_DISP_ADAGFX_SSD1306)
    // Show the display buffer on the hardware.
    // NOTE: You _must_ call display after making any drawing commands
    // to make them visible on the display hardware!
    m_disp.display();
    // TODO: Might need to call m_disp.clearDisplay() now?

  #else
    // Nothing to do as we're not double-buffered

  #endif
}


// -----------------------------------------------------------------------
// Graphics Primitives Functions
// -----------------------------------------------------------------------


bool gslc_DrvDrawPoint(gslc_tsGui* pGui,int16_t nX,int16_t nY,gslc_tsColor nCol)
{
#if (GSLC_CLIP_EN)
  // Perform clipping
  gslc_tsDriver* pDriver = (gslc_tsDriver*)(pGui->pvDriver);
  if (!gslc_ClipPt(&pDriver->rClipRect,nX,nY)) { return true; }
#endif

  uint16_t nColRaw = gslc_DrvAdaptColorToRaw(nCol);
  m_disp.drawPixel(nX,nY,nColRaw);
  return true;
}


bool gslc_DrvDrawPoints(gslc_tsGui* pGui,gslc_tsPt* asPt,uint16_t nNumPt,gslc_tsColor nCol)
{
  return false;
}

bool gslc_DrvDrawFillRect(gslc_tsGui* pGui,gslc_tsRect rRect,gslc_tsColor nCol)
{
#if (GSLC_CLIP_EN)
  // Perform clipping
  gslc_tsDriver* pDriver = (gslc_tsDriver*)(pGui->pvDriver);
  if (!gslc_ClipRect(&pDriver->rClipRect,&rRect)) { return true; }
#endif

  uint16_t nColRaw = gslc_DrvAdaptColorToRaw(nCol);
  m_disp.fillRect(rRect.x,rRect.y,rRect.w,rRect.h,nColRaw);
  return true;
}

bool gslc_DrvDrawFillRoundRect(gslc_tsGui* pGui,gslc_tsRect rRect,int16_t nRadius,gslc_tsColor nCol)
{
  // TODO: Support GSLC_CLIP_EN
  // - Would need to determine how to clip the rounded corners
  uint16_t nColRaw = gslc_DrvAdaptColorToRaw(nCol);
  m_disp.fillRoundRect(rRect.x,rRect.y,rRect.w,rRect.h,nRadius,nColRaw);
  return true;
}


bool gslc_DrvDrawFrameRect(gslc_tsGui* pGui,gslc_tsRect rRect,gslc_tsColor nCol)
{
  uint16_t nColRaw = gslc_DrvAdaptColorToRaw(nCol);
#if (GSLC_CLIP_EN)
  // Perform clipping
  // - TODO: Optimize the following, perhaps with new ClipLineHV()
  gslc_tsDriver* pDriver = (gslc_tsDriver*)(pGui->pvDriver);
  int16_t nX0, nY0, nX1, nY1;
  // Top
  nX0 = rRect.x;
  nY0 = rRect.y;
  nX1 = rRect.x + rRect.w - 1;
  nY1 = nY0;
  if (gslc_ClipLine(&pDriver->rClipRect, &nX0, &nY0, &nX1, &nY1)) { m_disp.drawLine(nX0, nY0, nX1, nY1, nColRaw); }
  // Bottom
  nX0 = rRect.x;
  nY0 = rRect.y + rRect.h - 1;
  nX1 = rRect.x + rRect.w - 1;
  nY1 = nY0;
  if (gslc_ClipLine(&pDriver->rClipRect, &nX0, &nY0, &nX1, &nY1)) { m_disp.drawLine(nX0, nY0, nX1, nY1, nColRaw); }
  // Left
  nX0 = rRect.x;
  nY0 = rRect.y;
  nX1 = nX0;
  nY1 = rRect.y + rRect.h - 1;
  if (gslc_ClipLine(&pDriver->rClipRect, &nX0, &nY0, &nX1, &nY1)) { m_disp.drawLine(nX0, nY0, nX1, nY1, nColRaw); }
  // Right
  nX0 = rRect.x + rRect.w - 1;
  nY0 = rRect.y;
  nX1 = nX0;
  nY1 = rRect.y + rRect.h - 1;
  if (gslc_ClipLine(&pDriver->rClipRect, &nX0, &nY0, &nX1, &nY1)) { m_disp.drawLine(nX0, nY0, nX1, nY1, nColRaw); }
#else
  m_disp.drawRect(rRect.x,rRect.y,rRect.w,rRect.h,nColRaw);
#endif
  return true;
}

bool gslc_DrvDrawFrameRoundRect(gslc_tsGui* pGui,gslc_tsRect rRect,int16_t nRadius,gslc_tsColor nCol)
{
  uint16_t nColRaw = gslc_DrvAdaptColorToRaw(nCol);
  // TODO: Support GSLC_CLIP_EN
  // - Would need to determine how to clip the rounded corners
  m_disp.drawRoundRect(rRect.x,rRect.y,rRect.w,rRect.h,nRadius,nColRaw);
  return true;
}



bool gslc_DrvDrawLine(gslc_tsGui* pGui,int16_t nX0,int16_t nY0,int16_t nX1,int16_t nY1,gslc_tsColor nCol)
{
#if (GSLC_CLIP_EN)
  gslc_tsDriver* pDriver = (gslc_tsDriver*)(pGui->pvDriver);
  if (!gslc_ClipLine(&pDriver->rClipRect,&nX0,&nY0,&nX1,&nY1)) { return true; }
#endif

  uint16_t nColRaw = gslc_DrvAdaptColorToRaw(nCol);
  m_disp.drawLine(nX0,nY0,nX1,nY1,nColRaw);
  return true;
}

bool gslc_DrvDrawFrameCircle(gslc_tsGui*,int16_t nMidX,int16_t nMidY,uint16_t nRadius,gslc_tsColor nCol)
{
#if (GSLC_CLIP_EN)
  // TODO
#endif

  uint16_t nColRaw = gslc_DrvAdaptColorToRaw(nCol);
  m_disp.drawCircle(nMidX,nMidY,nRadius,nColRaw);
  return true;
}

bool gslc_DrvDrawFillCircle(gslc_tsGui*,int16_t nMidX,int16_t nMidY,uint16_t nRadius,gslc_tsColor nCol)
{
#if (GSLC_CLIP_EN)
  // TODO
#endif

  uint16_t nColRaw = gslc_DrvAdaptColorToRaw(nCol);
  m_disp.fillCircle(nMidX,nMidY,nRadius,nColRaw);
  return true;
}


bool gslc_DrvDrawFrameTriangle(gslc_tsGui* pGui,int16_t nX0,int16_t nY0,
        int16_t nX1,int16_t nY1,int16_t nX2,int16_t nY2,gslc_tsColor nCol)
{
#if (GSLC_CLIP_EN)
  // TODO
#endif

  uint16_t nColRaw = gslc_DrvAdaptColorToRaw(nCol);
  m_disp.drawTriangle(nX0,nY0,nX1,nY1,nX2,nY2,nColRaw);
  return true;
}

bool gslc_DrvDrawFillTriangle(gslc_tsGui* pGui,int16_t nX0,int16_t nY0,
        int16_t nX1,int16_t nY1,int16_t nX2,int16_t nY2,gslc_tsColor nCol)
{
#if (GSLC_CLIP_EN)
  // TODO
#endif

  uint16_t nColRaw = gslc_DrvAdaptColorToRaw(nCol);
  m_disp.fillTriangle(nX0,nY0,nX1,nY1,nX2,nY2,nColRaw);
  return true;
}



// ----- REFERENCE CODE begin
// The following code was based upon the following reference code but modified to
// adapt for use in GUIslice.
//
//   URL:              https://github.com/adafruit/Adafruit-GFX-Library/blob/master/Adafruit_GFX.cpp
//   Original author:  Adafruit
//   Function:         drawBitmap()

// Draw a 1-bit image (bitmap) at the specified (x,y) position from the
// provided bitmap buffer using the foreground color defined in the
// header (unset bits are transparent).

// GUIslice modified the raw memory format to add a header:
// Image array format:
// - Width[15:8],  Width[7:0],
// - Height[15:8], Height[7:0],
// - ColorR[7:0],  ColorG[7:0],
// - ColorB[7:0],  0x00,
// - Monochrome bitmap follows...
//
void gslc_DrvDrawMonoFromMem(gslc_tsGui* pGui,int16_t nDstX, int16_t nDstY,
 const unsigned char *pBitmap,bool bProgMem)
 {
  const unsigned char*  bmap_base = pBitmap;
  int16_t         w,h;
  gslc_tsColor    nCol;

  // Read header
  w       = ( (bProgMem)? pgm_read_byte(bmap_base++) : *(bmap_base++) ) << 8;
  w      |= ( (bProgMem)? pgm_read_byte(bmap_base++) : *(bmap_base++) ) << 0;
  h       = ( (bProgMem)? pgm_read_byte(bmap_base++) : *(bmap_base++) ) << 8;
  h      |= ( (bProgMem)? pgm_read_byte(bmap_base++) : *(bmap_base++) ) << 0;
  nCol.r  =   (bProgMem)? pgm_read_byte(bmap_base++) : *(bmap_base++);
  nCol.g  =   (bProgMem)? pgm_read_byte(bmap_base++) : *(bmap_base++);
  nCol.b  =   (bProgMem)? pgm_read_byte(bmap_base++) : *(bmap_base++);
  bmap_base++;

  int16_t i, j, byteWidth = (w + 7) / 8;
  uint8_t nByte = 0;

  for(j=0; j<h; j++) {
    for(i=0; i<w; i++) {
      if(i & 7) nByte <<= 1;
      else {
        if (bProgMem) {
          nByte = pgm_read_byte(bmap_base + j * byteWidth + i / 8);
        } else {
          nByte = bmap_base[j * byteWidth + i / 8];
        }
      }
      if(nByte & 0x80) {
        gslc_DrvDrawPoint(pGui,nDstX+i,nDstY+j,nCol);
      }
    }
  }
}
// ----- REFERENCE CODE end

void gslc_DrvDrawBmp24FromMem(gslc_tsGui* pGui,int16_t nDstX, int16_t nDstY,const unsigned char* pBitmap,bool bProgMem)
{
  // AdaFruit GFX doesn't have a routine for this so we output pixel by pixel
  const int16_t* pImage = (const int16_t*)pBitmap;
  int16_t h, w;
  if (bProgMem) {
    h = pgm_read_word(pImage++);
    w = pgm_read_word(pImage++);
  } else {
    h = *(pImage++);
    w = *(pImage++);
  }
  #if defined(DBG_DRIVER)
  GSLC_DEBUG_PRINT("DBG: DrvDrawBmp24FromMem() w=%d h=%d\n", w, h);
  #endif
  int row, col;
  for (row=0; row<h; row++) { // For each scanline...
    for (col=0; col<w; col++) { // For each pixel...
      if (bProgMem) {
        //To read from Flash Memory, pgm_read_XXX is required.
        //Since image is stored as uint16_t, pgm_read_word is used as it uses 16bit address
        m_disp.drawPixel(nDstX+col, nDstY+row, pgm_read_word(pImage++));
      } else {
        m_disp.drawPixel(nDstX+col, nDstY+row, *(pImage++));
      }
    } // end pixel
  }
}

#if (GSLC_SD_EN)
// ----- REFERENCE CODE begin
// The following code was based upon the following reference code but modified to
// adapt for use in GUIslice.
//
//   URL:              https://github.com/adafruit/Adafruit_ILI9341/blob/master/examples/spitftbitmap/spitftbitmap.ino
//   Original author:  Adafruit
//   Function:         bmpDraw()

// These read 16- and 32-bit types from the SD card file.
// BMP data is stored little-endian, Arduino is little-endian too.
// May need to reverse subscript order if porting elsewhere.
uint16_t gslc_DrvRead16SD(File &f) {
  uint16_t result;
  ((uint8_t *)&result)[0] = f.read(); // LSB
  ((uint8_t *)&result)[1] = f.read(); // MSB
  return result;
}

uint32_t gslc_DrvRead32SD(File &f) {
  uint32_t result;
  ((uint8_t *)&result)[0] = f.read(); // LSB
  ((uint8_t *)&result)[1] = f.read();
  ((uint8_t *)&result)[2] = f.read();
  ((uint8_t *)&result)[3] = f.read(); // MSB
  return result;
}

void gslc_DrvDrawBmp24FromSD(gslc_tsGui* pGui,const char *filename, uint16_t x, uint16_t y)
{
  File     bmpFile;
  int      bmpWidth, bmpHeight;   // W+H in pixels
  uint8_t  bmpDepth;              // Bit depth (currently must be 24)
  uint32_t bmpImageoffset;        // Start of image data in file
  uint32_t rowSize;               // Not always = bmpWidth; may have padding
  uint8_t  sdbuffer[3*GSLC_SD_BUFFPIXEL]; // pixel buffer (R+G+B per pixel)
  uint8_t  buffidx = sizeof(sdbuffer); // Current position in sdbuffer
  boolean  goodBmp = false;       // Set to true on valid header parse
  boolean  flip    = true;        // BMP is stored bottom-to-top
  int      w, h, row, col;
  uint8_t  r, g, b;
  uint32_t pos = 0, startTime = millis();
  (void)startTime; // Unused

  if((x >= pGui->nDispW) || (y >= pGui->nDispH)) return;

  //Serial.println();
  //Serial.print("Loading image '");
  //Serial.print(filename);
  //Serial.println('\'');

  // Open requested file on SD card
  if ((bmpFile = SD.open(filename)) == 0) {
    GSLC_DEBUG2_PRINT("ERROR: DrvDrawBmp24FromSD() file not found [%s]",filename);
    return;
  }
  // Parse BMP header
  if(gslc_DrvRead16SD(bmpFile) == 0x4D42) { // BMP signature
    uint32_t nFileSize = gslc_DrvRead32SD(bmpFile);
    (void)nFileSize; // Unused
    //Serial.print("File size: "); Serial.println(nFileSize);
    (void)gslc_DrvRead32SD(bmpFile); // Read & ignore creator bytes
    bmpImageoffset = gslc_DrvRead32SD(bmpFile); // Start of image data
    //Serial.print("Image Offset: "); Serial.println(bmpImageoffset, DEC);
    // Read DIB header
    uint32_t nHdrSize = gslc_DrvRead32SD(bmpFile);
    (void)nHdrSize; // Unused
    //Serial.print("Header size: "); Serial.println(nHdrSize);
    bmpWidth  = gslc_DrvRead32SD(bmpFile);
    bmpHeight = gslc_DrvRead32SD(bmpFile);
    if(gslc_DrvRead16SD(bmpFile) == 1) { // # planes -- must be '1'
      bmpDepth = gslc_DrvRead16SD(bmpFile); // bits per pixel
      //Serial.print("Bit Depth: "); Serial.println(bmpDepth);
      if((bmpDepth == 24) && (gslc_DrvRead32SD(bmpFile) == 0)) { // 0 = uncompressed
        goodBmp = true; // Supported BMP format -- proceed!
        //Serial.print("Image size: ");
        //Serial.print(bmpWidth);
        //Serial.print('x');
        //Serial.println(bmpHeight);

        // BMP rows are padded (if needed) to 4-byte boundary
        rowSize = (bmpWidth * 3 + 3) & ~3;

        // If bmpHeight is negative, image is in top-down order.
        // This is not canon but has been observed in the wild.
        if(bmpHeight < 0) {
          bmpHeight = -bmpHeight;
          flip      = false;
        }

        // Crop area to be loaded
        w = bmpWidth;
        h = bmpHeight;
        if((x+w-1) >= pGui->nDispW) w = pGui->nDispW  - x;
        if((y+h-1) >= pGui->nDispH) h = pGui->nDispH - y;

        // Set TFT address window to clipped image bounds
        //xxx tft.setAddrWindow(x, y, x+w-1, y+h-1);

        for (row=0; row<h; row++) { // For each scanline...

          // Seek to start of scan line.  It might seem labor-
          // intensive to be doing this on every line, but this
          // method covers a lot of gritty details like cropping
          // and scanline padding.  Also, the seek only takes
          // place if the file position actually needs to change
          // (avoids a lot of cluster math in SD library).
          if(flip) // Bitmap is stored bottom-to-top order (normal BMP)
            pos = bmpImageoffset + (bmpHeight - 1 - row) * rowSize;
          else     // Bitmap is stored top-to-bottom
            pos = bmpImageoffset + row * rowSize;
          if(bmpFile.position() != pos) { // Need seek?
            bmpFile.seek(pos);
            buffidx = sizeof(sdbuffer); // Force buffer reload
          }

          for (col=0; col<w; col++) { // For each pixel...
            // Time to read more pixel data?
            if (buffidx >= sizeof(sdbuffer)) { // Indeed
              bmpFile.read(sdbuffer, sizeof(sdbuffer));
              buffidx = 0; // Set index to beginning
            }

            // Convert pixel from BMP to TFT format, push to display
            b = sdbuffer[buffidx++];
            g = sdbuffer[buffidx++];
            r = sdbuffer[buffidx++];
            //xxx tft.pushColor(tft.Color565(r,g,b));
            gslc_tsColor nCol = (gslc_tsColor){r,g,b};
            gslc_tsColor nColTrans = (gslc_tsColor){GSLC_BMP_TRANS_RGB};
            bool bDrawBit = true;
            if (GSLC_BMP_TRANS_EN) {
              if ((nCol.r == nColTrans.r) && (nCol.g == nColTrans.g) && (nCol.b == nColTrans.b)) {
                bDrawBit = false;
              }
            }
            if (bDrawBit) {
              gslc_DrvDrawPoint(pGui,x+col,y+row,nCol);
            }

          } // end pixel
        } // end scanline
        //Serial.print("Loaded in ");
        //Serial.print(millis() - startTime);
        //Serial.println(" ms");
      } // end goodBmp
    }
  }
  bmpFile.close();
  if(!goodBmp) {
    GSLC_DEBUG2_PRINT("ERROR: DrvDrawBmp24FromSD() BMP format unknown [%s]",filename);
  }
}
// ----- REFERENCE CODE end
#endif // GSLC_SD_EN


bool gslc_DrvDrawImage(gslc_tsGui* pGui,int16_t nDstX,int16_t nDstY,gslc_tsImgRef sImgRef)
{
  #if defined(DBG_DRIVER)
  char addr[6];
  GSLC_DEBUG_PRINT("DBG: DrvDrawImage() with ImgBuf address=","");
  sprintf(addr,"%04X",(unsigned int)sImgRef.pImgBuf);
  GSLC_DEBUG_PRINT("%s\n",addr);
  #endif

  // GUIslice adapter library for Adafruit-GFX does not pre-load
  // image data into memory before calling DrvDrawImage(), so
  // we to handle the loading now (when rendering).
  if (sImgRef.eImgFlags == GSLC_IMGREF_NONE) {
    return true;  // Nothing to do

  } else if ((sImgRef.eImgFlags & GSLC_IMGREF_SRC) == GSLC_IMGREF_SRC_FILE) {
    return false; // Not supported

  } else if ((sImgRef.eImgFlags & GSLC_IMGREF_SRC) == GSLC_IMGREF_SRC_RAM) {
    if ((sImgRef.eImgFlags & GSLC_IMGREF_FMT) == GSLC_IMGREF_FMT_RAW1) {
      // Draw a monochrome bitmap from SRAM
      // - Dimensions and output color are defined in arrray header
      gslc_DrvDrawMonoFromMem(pGui,nDstX,nDstY,sImgRef.pImgBuf,false);
      return true;
    } else if ((sImgRef.eImgFlags & GSLC_IMGREF_FMT) == GSLC_IMGREF_FMT_BMP24) {
      // 24-bit Bitmap in ram
      gslc_DrvDrawBmp24FromMem(pGui,nDstX,nDstY,sImgRef.pImgBuf,false);
      return true;
    } else {
      return false; // TODO: not yet supported
    }
#if (GSLC_USE_PROGMEM)
  } else if ((sImgRef.eImgFlags & GSLC_IMGREF_SRC) == GSLC_IMGREF_SRC_PROG) {
    // TODO: Probably need to fix this to work with PROGMEM,
    //       but check (GSLC_USE_PROGMEM) first
    if ((sImgRef.eImgFlags & GSLC_IMGREF_FMT) == GSLC_IMGREF_FMT_RAW1) {
      // Draw a monochrome bitmap from program memory
      // - Dimensions and output color are defined in array header
      gslc_DrvDrawMonoFromMem(pGui,nDstX,nDstY,sImgRef.pImgBuf,true);
      return true;
    } else if ((sImgRef.eImgFlags & GSLC_IMGREF_FMT) == GSLC_IMGREF_FMT_BMP24) {
      // 24-bit Bitmap in flash
      gslc_DrvDrawBmp24FromMem(pGui,nDstX,nDstY,sImgRef.pImgBuf,true);
      return true;
    } else {
      return false; // TODO: not yet supported
    }
#endif
  } else if ((sImgRef.eImgFlags & GSLC_IMGREF_SRC) == GSLC_IMGREF_SRC_SD) {
    // Load image from SD media
    #if (GSLC_SD_EN)
      if ((sImgRef.eImgFlags & GSLC_IMGREF_FMT) == GSLC_IMGREF_FMT_BMP24) {
        // 24-bit Bitmap
        gslc_DrvDrawBmp24FromSD(pGui,sImgRef.pFname,nDstX,nDstY);
        return true;
      } else {
        // Unsupported format
        return false;
      }
    #else
      // SD card access not enabled
      return false;
    #endif

  } else {
    // Unsupported source
    GSLC_DEBUG2_PRINT("DBG: DrvDrawImage() unsupported source eImgFlags=%d\n", sImgRef.eImgFlags);
    return false;
  }
}


void gslc_DrvDrawBkgnd(gslc_tsGui* pGui)
{
  if (pGui->pvDriver) {
    gslc_tsDriver*  pDriver = (gslc_tsDriver*)(pGui->pvDriver);

    // Check to see if an image has been assigned to the background
    if (pGui->sImgRefBkgnd.eImgFlags == GSLC_IMGREF_NONE) {
      // No image assigned, so assume flat color background
      // TODO: Create a new eImgFlags enum to signal that the
      //       background should be a flat color instead of
      //       an image.

      // NOTE: We don't call m_disp.fillScreen() here as
      //       that API doesn't support clipping. Since
      //       we may be redrawing the page with a clipping
      //       region enabled, it is important that we don't
      //       redraw the entire screen.
      gslc_tsRect rRect = (gslc_tsRect) { 0, 0, pGui->nDispW, pGui->nDispH };
      gslc_DrvDrawFillRect(pGui, rRect, pDriver->nColBkgnd);
    } else {
      // An image should be loaded
      // TODO: For now, re-use the DrvDrawImage(). Later, consider
      //       extending to support different background drawing
      //       capabilities such as stretching and tiling of background
      //       image.
      gslc_DrvDrawImage(pGui,0,0,pGui->sImgRefBkgnd);
    }
  }
}


// -----------------------------------------------------------------------
// Touch Functions (via display driver)
// -----------------------------------------------------------------------


bool gslc_DrvInitTouch(gslc_tsGui* pGui,const char* acDev) {
  if (pGui == NULL) {
    GSLC_DEBUG2_PRINT("ERROR: DrvInitTouch(%s) called with NULL ptr\n","");
    return false;
  }
  // TODO
  // Perform any driver-specific touchscreen init here
  return true;
}


bool gslc_DrvGetTouch(gslc_tsGui* pGui,int16_t* pnX,int16_t* pnY,uint16_t* pnPress,gslc_teInputRawEvent* peInputEvent,int16_t* pnInputVal)
{
  // TODO
  return false;
}

// ------------------------------------------------------------------------
// Touch Functions (via external touch driver)
// ------------------------------------------------------------------------


#if defined(DRV_TOUCH_ADA_SIMPLE)

  // --------------------------------------------------------------------------
  // Enable Adafruit_TouchScreen workarounds
  // --------------------------------------------------------------------------

  // NOTE: The Adafruit_TouchScreen is not natively compatible with certain
  //       devices (eg. ESP32) and also doesn't safeguard against pin-sharing
  //       conflicts. For these and some other issues, the following workarounds
  //       are optionally enabled.

  // Enable workaround for ambiguity in Adafruit_TouchScreen pressure readings
  // - See https://github.com/ImpulseAdventure/GUIslice/issues/96
  #define FIX_4WIRE_Z // Comment out to disable

  // Enable workaround for Adafruit_TouchScreen getPoint() altering
  // the pin state and not restoring it. Without working around this,
  // the touch handler may interfere with displays that share pins.
  #define FIX_4WIRE_PIN_STATE // Comment out to disable

  // --------------------------------------------------------------------------

  // Disable certain workarounds for Adafruit_TouchScreen in STM32 mode
  // as we haven't implemented the equivalent pin save/restore code yet.
  #if defined(ARDUINO_ARCH_STM32) || defined(__STM32F1__)
    #undef FIX_4WIRE_PIN_STATE
  #endif

  #if defined(FIX_4WIRE_PIN_STATE)
  // NOTE: The Adafruit_TouchScreen library alters the state of several
  //       pins during the course of reading the touch coordinates and
  //       pressure. Unfortunately, it does not restore the prior state
  //       which can impact other processes such as graphics drivers which
  //       may share the same pins. The following routines are responsible
  //       for saving and restoring the pin state and will wrap the
  //       touch polling logic. If a future release of the Adafruit_TouchScreen
  //       library addresses this issue, this wrapper logic can be removed.
  //       For further reference, please refer to Issue #96.

  /// Structure used to retain a port state (mode and level)
  /// so that it can be restored later.
  struct gslc_tsPinState
  {
    int     nMode;     // OUTPUT, INPUT, INPUT_PULLUP
    bool    bIsHigh;   // Is an output and HIGH?
  };

  /// Return the current pinMode() for a pin
  int gslc_TDrvGetPinMode(uint8_t nPin)
  {
    if (nPin >= NUM_DIGITAL_PINS) {
      return (-1);
    }
    uint8_t nBit            = digitalPinToBitMask(nPin);
    uint8_t nPort           = digitalPinToPort(nPin);

    // Determine if port is an output
    volatile uint8_t *nReg  = portModeRegister(nPort);
    if (*nReg & nBit) {
      return (OUTPUT);
    }

    // Determine if port is an input and whether pullup is active
    volatile  uint8_t *nOut = portOutputRegister(nPort);
    return ((*nOut & nBit) ? INPUT_PULLUP : INPUT);
  }

  /// Fetch the current pin mode and level
  inline void gslc_TDrvSavePinState(int nPin, gslc_tsPinState &sPinState)
  {
    sPinState.nMode = gslc_TDrvGetPinMode(nPin);
    sPinState.bIsHigh = digitalRead(nPin);
  }

  /// Restore the pin mode and level
  inline void gslc_TDrvRestorePinState(int nPin,gslc_tsPinState sPinState)
  {
    pinMode(nPin,sPinState.nMode);
    if (sPinState.nMode == OUTPUT) digitalWrite(nPin,sPinState.bIsHigh);
  }

  #endif // FIX_4WIRE_PIN_STATE

  // --------------------------------------------------------------------------

#endif // DRV_TOUCH_ADA_SIMPLE


#if defined(DRV_TOUCH_TYPE_EXTERNAL)

bool gslc_TDrvInitTouch(gslc_tsGui* pGui,const char* acDev) {

  // Capture default calibration settings for resistive displays
  #if defined(DRV_TOUCH_TYPE_RES)
    pGui->nTouchCalXMin = ADATOUCH_X_MIN;
    pGui->nTouchCalXMax = ADATOUCH_X_MAX;
    pGui->nTouchCalYMin = ADATOUCH_Y_MIN;
    pGui->nTouchCalYMax = ADATOUCH_Y_MAX;
  #endif // DRV_TOUCH_TYPE_RES

  // Support touch controllers with swapped X & Y
  #if defined(ADATOUCH_REMAP_YX)
    // Capture swap setting from config file
    pGui->bTouchRemapYX = ADATOUCH_REMAP_YX;
  #else
    // For backward compatibility with older config files
    // that have not defined this config option
    pGui->bTouchRemapYX = false;
  #endif

  #if defined(DRV_TOUCH_ADA_STMPE610)
    #if (ADATOUCH_I2C_HW)
    if (!m_touch.begin(ADATOUCH_I2C_ADDR)) {
    #else
    if (!m_touch.begin()) {
    #endif
      GSLC_DEBUG2_PRINT("ERROR: TDrvInitTouch() failed to init STMPE610\n",0);
      return false;
    } else {
      return true;
    }
  #elif defined(DRV_TOUCH_ADA_FT6206)
    if (!m_touch.begin(ADATOUCH_SENSITIVITY)) {
      GSLC_DEBUG2_PRINT("ERROR: TDrvInitTouch() failed to init FT6206\n",0);
      return false;
    } else {
      return true;
    }
  #elif defined(DRV_TOUCH_ADA_SIMPLE)
    return true;
  #elif defined(DRV_TOUCH_XPT2046_STM)
    m_touch.begin();
    return true;
  #elif defined(DRV_TOUCH_XPT2046_PS)
    m_touch.begin();
    // Since this XPT2046 library supports "touch rotation", and defaults
    // to landscape orientation, rotate to traditional portrait orientation
    // for consistency with other handlers.
    //
    // Unfortunately, this API (from 2018/01/04) is not available in the
    // latest tagged release of XPT2046 in the Library Manager. Therefore,
    // we can't use this API and instead need to hardcode the mapping
    // during the DrvGetTouch() function.
    //m_touch.setRotation(0);
    return true;
  #elif defined(DRV_TOUCH_ADA_RA8875)
    m_disp.touchEnable(true);
    return true;
  #elif defined(DRV_TOUCH_URTOUCH)
    m_touch.InitTouch();
    m_touch.setPrecision(PREC_MEDIUM);
    // Disable touch remapping since URTouch handles it
    gslc_SetTouchRemapEn(pGui, false);
    return true;
  #elif defined(DRV_TOUCH_INPUT)
    // Nothing more to initialize for GPIO input control mode
    return true;
  #elif defined(DRV_TOUCH_HANDLER)
    return true;
  #else
    // ERROR: Unsupported driver mode
    GSLC_DEBUG_PRINT("ERROR: TDrvInitTouch() driver not supported yet\n",0);
    return false;
  #endif

}

bool gslc_TDrvGetTouch(gslc_tsGui* pGui,int16_t* pnX,int16_t* pnY,uint16_t* pnPress,gslc_teInputRawEvent* peInputEvent,int16_t* pnInputVal)
{

  #if defined(DRV_TOUCH_NONE)
    return false;
  #endif

  // As the STMPE610 hardware driver doesn't appear to return
  // an indication of "touch released" with a coordinate, we
  // must detect the release transition here and send the last
  // known coordinate but with pressure=0. To do this, we are
  // allocating a static variable to maintain the last touch
  // coordinate.
  // TODO: This code can be reworked / simplified
  static int16_t  m_nLastRawX     = 0;
  static int16_t  m_nLastRawY     = 0;
  static uint16_t m_nLastRawPress = 0;
  static bool     m_bLastTouched  = false;

  bool bValid = false;  // Indicate a touch event to GUIslice core?

  // Define maximum bounds for display in native orientation
  int nDispOutMaxX,nDispOutMaxY;
  nDispOutMaxX = pGui->nDisp0W-1;
  nDispOutMaxY = pGui->nDisp0H-1;

  // ----------------------------------------------------------------
  #if defined(DRV_TOUCH_ADA_STMPE610)

  uint16_t  nRawX,nRawY;
  uint8_t   nRawPress;

  if (m_touch.touched()) {

    if (m_touch.bufferEmpty()) {
      // Nothing to do
    } else {
      while (!m_touch.bufferEmpty()) {
        // Continued press; update next reading
        // TODO: Is there a risk that the touch hardware could
        //       maintain a non-empty state for an extended period of time?
        m_touch.readData(&nRawX,&nRawY,&nRawPress);
        m_nLastRawX = nRawX;
        m_nLastRawY = nRawY;
        m_nLastRawPress = nRawPress;
        m_bLastTouched = true;
        bValid = true;
      }

    }
    // Clear interrupts
    m_touch.writeRegister8(STMPE_INT_STA, 0xFF);

  } else {
    if (!m_bLastTouched) {
      // Wasn't touched before; do nothing
    } else {
      // Touch release
      // Indicate old coordinate but with pressure=0
      m_nLastRawPress = 0;
      m_bLastTouched = false;
      bValid = true;
    }
    // Flush the FIFO
    while (!m_touch.bufferEmpty()) {
      m_touch.readData(&nRawX,&nRawY,&nRawPress);
    }
  }

  // ----------------------------------------------------------------
  #elif defined(DRV_TOUCH_ADA_FT6206)

  if (m_touch.touched()) {
    TS_Point ptTouch = m_touch.getPoint();
    // FT6206 coordinates appear to have flipped both axes vs other controllers
    // - Confirmed by comments in Adafruit_FT6206 library example code:
    //   "rotate coordinate system. flip it around to match the screen."
    m_nLastRawX = nDispOutMaxX-ptTouch.x;
    m_nLastRawY = nDispOutMaxY-ptTouch.y;
    m_nLastRawPress = 255;  // Select arbitrary non-zero value
    m_bLastTouched = true;
    bValid = true;

  } else {
    if (!m_bLastTouched) {
      // Wasn't touched before; do nothing
    } else {
      // Touch release
      // Indicate old coordinate but with pressure=0
      m_nLastRawPress = 0;
      m_bLastTouched = false;
      bValid = true;
    }
  }

  // ----------------------------------------------------------------
  #elif defined(DRV_TOUCH_ADA_SIMPLE)

  uint16_t  nRawX,nRawY;
  int16_t   nRawPress;

  #if defined(FIX_4WIRE_PIN_STATE)
    // Saved pin state
    gslc_tsPinState   sPinStateXP, sPinStateXM, sPinStateYP, sPinStateYM;

    // As Adafruit_TouchScreen polling will alter the pin state and some
    // of these pins may be shared with the display, we need to save and
    // then later restore the pin state.
    gslc_TDrvSavePinState(ADATOUCH_PIN_XP, sPinStateXP);
    gslc_TDrvSavePinState(ADATOUCH_PIN_XM, sPinStateXM);
    gslc_TDrvSavePinState(ADATOUCH_PIN_YP, sPinStateYP);
    gslc_TDrvSavePinState(ADATOUCH_PIN_YM, sPinStateYM);
  #endif // FIX_4WIRE_PIN_STATE
  
  // Perform the polling of touch coordinate & pressure
  TSPoint p = m_touch.getPoint();

  // Select reasonable touch pressure threshold range.
  // Note that the Adafruit_TouchScreen library appears to
  // return the following:
  // - 0:     If no touch (results from integer overflow, div/0)
  // - 0:     If touch active but filtered due to noise
  // - small: If touch active and hard
  // - large: If touch active and soft
  // Note that the "pressure" (z) value is inverted in interpretation
  if ((p.z > ADATOUCH_PRESS_MIN) && (p.z < ADATOUCH_PRESS_MAX)) {
    nRawX = p.x;
    nRawY = p.y;
    nRawPress = p.z;
    m_nLastRawX = nRawX;
    m_nLastRawY = nRawY;
    m_nLastRawPress = nRawPress;
    m_bLastTouched = true;
    bValid = true;
  } else {
    if (!m_bLastTouched) {
      // Wasn't touched before; do nothing
    } else {

      #if !defined(FIX_4WIRE_Z) // Original behavior without touch pressure workaround

      // Indicate old coordinate but with pressure=0
      m_nLastRawPress = 0;
      m_bLastTouched = false;
      bValid = true;
      #ifdef DBG_TOUCH
      GSLC_DEBUG_PRINT("DBG: Touch End  =%u Raw[%d,%d] *****\n",
          m_nLastRawPress,m_nLastRawX,m_nLastRawY);
      #endif

      #else // Apply touch pressure workaround

      // Unfortunately, the Adafruit_TouchScreen has a few issues that
      // make it hard to deal with reliably. The most difficult problem
      // involves the ambiguous return state from getTouch().
      // Without handling this in a special way, we might see spurious
      // touch-release events.
      //
      // Upon entering this clause, we can infer Adafruit_TouchScreen returned z=0
      // - This either means:
      //   a) Touch was released (z is 0 due to integer overflow, div/0)
      //   b) Touch still active but filtered due to noisy read
      //
      // Because of case (b) returning the same signature as case (a), we
      // need to take an additional step to differentiate the two cases
      // otherwise we might interpret spurious "touch release" events.
      //
      // In order to differentiate these cases, we can call the Adafruit
      // getPressure() API since it does not include the filtering for (b).
      // Therefore, if we see that the pressure is non-zero, and less than
      // the max pressure threshold, we can re-interpret our original reading
      // as (b), wherein we would still want to treat as a touch pressed event.

      // Read the touch pressure
      // Note that we will need to restore the pin status later
      // once we are done with our polling.
      uint16_t nPressCur = m_touch.pressure();

      if ((nPressCur > ADATOUCH_PRESS_MIN) && (nPressCur < ADATOUCH_PRESS_MAX)) {
        // The unfiltered result is that the display is still pressed
        // Therefore we are likely in case (b) and should return our
        // last saved result (with touch pressure still active)
        bValid = true;
        #ifdef DBG_TOUCH
        // Give indication that workaround applied: continue press
        GSLC_DEBUG_PRINT("DBG: Touch Cont =%u Raw[%d,%d]\n",
            m_nLastRawPress,m_nLastRawX,m_nLastRawY);
        #endif
      } else {
        // The unfiltered result is that the display is not pressed
        // Therefore we are likely in case (a) and should force
        // the touch pressure to be deactivated

        // Indicate old coordinate but with pressure=0
        m_nLastRawPress = 0;
        m_bLastTouched = false;
        bValid = true;
        #ifdef DBG_TOUCH
        GSLC_DEBUG_PRINT("DBG: Touch End  =%u Raw[%d,%d] *****\n",
            m_nLastRawPress,m_nLastRawX,m_nLastRawY);
        #endif
      } // nPressCur
      #endif // FIX_4WIRE_Z

      // TODO: Implement touch debouncing

    } // m_bLastTouched
  }

  #if defined(FIX_4WIRE_PIN_STATE)
    // Now that we have completed our polling into Adafruit_TouchScreen,
    // we need to restore the original pin state.
    gslc_TDrvRestorePinState(ADATOUCH_PIN_XP, sPinStateXP);
    gslc_TDrvRestorePinState(ADATOUCH_PIN_XM, sPinStateXM);
    gslc_TDrvRestorePinState(ADATOUCH_PIN_YP, sPinStateYP);
    gslc_TDrvRestorePinState(ADATOUCH_PIN_YM, sPinStateYM);
  #endif // FIX_4WIRE_PIN_STATE

  // ----------------------------------------------------------------
  #elif defined(DRV_TOUCH_XPT2046_STM)
    // NOTE: XPT2046_STM returns pressure (z) values with a reversed
    //       convention versus other touch libraries (ie. a small
    //       non-zero z value means light touch, whereas a large
    //       value means a hard / wide touch).

    uint16_t  nRawX,nRawY; //XPT2046 returns values up to 4095
    uint16_t  nRawPress;   //XPT2046 returns values up to 4095

    TS_Point p = m_touch.getPoint();

    if ((p.z > ADATOUCH_PRESS_MIN) && (p.z < ADATOUCH_PRESS_MAX)) {
      nRawX = p.x;
      nRawY = p.y;
      nRawPress = p.z;
      m_nLastRawX = nRawX;
      m_nLastRawY = nRawY;
      m_nLastRawPress = nRawPress;
      m_bLastTouched = true;
      bValid = true;
    }
    else {
      if (!m_bLastTouched) {
        // Wasn't touched before; do nothing
      }
      else {
        // Touch release
        // Indicate old coordinate but with pressure=0
        m_nLastRawPress = 0;
        m_bLastTouched = false;
        bValid = true;
      }
    }

  // ----------------------------------------------------------------
  #elif defined(DRV_TOUCH_XPT2046_PS)
    uint16_t  nRawX,nRawY; //XPT2046 returns values up to 4095
    uint16_t  nRawPress;   //XPT2046 returns values up to 4095

    TS_Point p = m_touch.getPoint();

    if ((p.z > ADATOUCH_PRESS_MIN) && (p.z < ADATOUCH_PRESS_MAX)) {
      // PaulStoffregen/XPT2046 appears to use a different orientation
      // than other libraries. Therefore, we will remap it here
      // to match the default portrait orientation.
      nRawX = 4095-p.y;
      nRawY = p.x;

      nRawPress = p.z;
      m_nLastRawX = nRawX;
      m_nLastRawY = nRawY;
      m_nLastRawPress = nRawPress;
      m_bLastTouched = true;
      bValid = true;
    }
    else {
      if (!m_bLastTouched) {
        // Wasn't touched before; do nothing
      }
      else {
        // Touch release
        // Indicate old coordinate but with pressure=0
        m_nLastRawPress = 0;
        m_bLastTouched = false;
        bValid = true;
      }
    }

  // ----------------------------------------------------------------
  #elif defined(DRV_TOUCH_ADA_RA8875)
    uint16_t  nRawX,nRawY;

    // Use Adafruit_RA8875 display driver for touch
    // Note that it doesn't support a "pressure" reading
    if (m_disp.touched()) {
      m_disp.touchRead(&nRawX,&nRawY);

      m_nLastRawX = nRawX;
      m_nLastRawY = nRawY;
      m_nLastRawPress = 255;  // Select arbitrary non-zero value
      m_bLastTouched = true;
      bValid = true;

      // The Adafruit_RA8875 touched() implementation relies on reading
      // the status of the Touch Panel interrupt register bit. The touchRead()
      // call clears the status of the interrupt. It appears that the
      // interrupt requires moderate time to propagate (so that it can be
      // available for the next call to touched). Therefore, a 1ms delay
      // is inserted here. Note that a similar delay can be found in
      // the Adafruit example code.
      delay(1);

    } else {
      if (!m_bLastTouched) {
        // Wasn't touched before; do nothing
      } else {
        // Touch release
        // Indicate old coordinate but with pressure=0
        m_nLastRawPress = 0;
        m_bLastTouched = false;
        bValid = true;
      }
    }

  // ----------------------------------------------------------------
  #elif defined(DRV_TOUCH_HANDLER)

    uint16_t  nRawX,nRawY;
    uint16_t  nRawPress; 
  
    TouchHandler *pTH = gslc_getTouchHandler();
    THPoint p(0,0,0);
    //if no TouchHandler was defined use (0,0,0)
    if (pTH!=NULL)
        p = pTH->getPoint();
    
    if (p.z > 0) {
      nRawX=p.x;
      nRawY=p.y;
      nRawPress=p.z;
      m_nLastRawX = nRawX;
      m_nLastRawY = nRawY;
      m_nLastRawPress = nRawPress;
      m_bLastTouched = true;
      bValid = true;
      //Serial.print("pTH= ");Serial.print(p.x);Serial.print(" ");Serial.print(p.y);Serial.print(" ");Serial.println(p.z);
    }
    else {
      if (!m_bLastTouched) {
        // Wasn't touched before; do nothing
      } else {
        // Touch release
        // Indicate old coordinate but with pressure=0
        m_nLastRawPress = 0;
        m_bLastTouched = false;
        bValid = true;
      }
    }

  // ----------------------------------------------------------------
  #elif defined(DRV_TOUCH_URTOUCH)

    // Note that we rely on URTouch's calibration
    // - This is detected by URTouch / URTouch_Calibration
    // - The calibration settings are stored in URTouch/URTouchCD.h

    int16_t   nRawX,nRawY;
    uint16_t  nRawPress = 0;
    bool bTouchOk = true;

    if (!m_touch.dataAvailable()) {
      bTouchOk = false;
    }

    if (bTouchOk) {
      m_touch.read();
      nRawX = m_touch.getX();
      nRawY = m_touch.getY();
      if ((nRawX == -1) || (nRawY == -1)) {
        bTouchOk = false;
      }
    }

    if (bTouchOk) {
      nRawPress = 255; // Dummy non-zero value
      m_nLastRawX = nRawX;
      m_nLastRawY = nRawY;
      m_nLastRawPress = nRawPress;
      m_bLastTouched = true;
      bValid = true;
    } else {
      if (!m_bLastTouched) {
        // Wasn't touched before; do nothing
      }
      else {
        // Touch release
        // Indicate old coordinate but with pressure=0
        m_nLastRawPress = 0;
        m_bLastTouched = false;
        bValid = true;
      }
    }

  // ----------------------------------------------------------------
  #elif defined(DRV_TOUCH_INPUT)
    // No more to do for GPIO-only mode since gslc_Update() already
    // looks for GPIO inputs before calling TDrvGetTouch().
    // bValid will default to false


  // Assign defaults
  *pnX = 0;
  *pnY = 0;
  *pnPress = 0;

  *peInputEvent = GSLC_INPUT_NONE;
  *pnInputVal = 0;

  #ifdef DRV_DISP_ADAGFX_SEESAW
    // Keep track of last value to support simple debouncing
    static uint32_t nButtonsLast = 0xFFFFFFFF;     // Saved last value (static to preserve b/w calls)
    uint32_t nButtonsCur = m_seesaw.readButtons(); // Current value (note active low)
    if ((nButtonsLast & TFTSHIELD_BUTTON_UP) && !(nButtonsCur & TFTSHIELD_BUTTON_UP)) {
      *peInputEvent = GSLC_INPUT_PIN_ASSERT;
      *pnInputVal = GSLC_PIN_BTN_UP;
    } else if ((nButtonsLast & TFTSHIELD_BUTTON_DOWN) && !(nButtonsCur & TFTSHIELD_BUTTON_DOWN)) {
      *peInputEvent = GSLC_INPUT_PIN_ASSERT;
      *pnInputVal = GSLC_PIN_BTN_DOWN;
    } else if ((nButtonsLast & TFTSHIELD_BUTTON_LEFT) && !(nButtonsCur & TFTSHIELD_BUTTON_LEFT)) {
      *peInputEvent = GSLC_INPUT_PIN_ASSERT;
      *pnInputVal = GSLC_PIN_BTN_LEFT;
    } else if ((nButtonsLast & TFTSHIELD_BUTTON_RIGHT) && !(nButtonsCur & TFTSHIELD_BUTTON_RIGHT)) {
      *peInputEvent = GSLC_INPUT_PIN_ASSERT;
      *pnInputVal = GSLC_PIN_BTN_RIGHT;
    } else if ((nButtonsLast & TFTSHIELD_BUTTON_IN) && !(nButtonsCur & TFTSHIELD_BUTTON_IN)) {
      *peInputEvent = GSLC_INPUT_PIN_ASSERT;
      *pnInputVal = GSLC_PIN_BTN_SEL;
    }
    // Save button state so that transitions can be detected
    // during the next pass.
    nButtonsLast = nButtonsCur;
  #endif


  // If we reached here, then we had a button event
  return true;

  // ----------------------------------------------------------------
  #endif // DRV_TOUCH_*


  // If an event was detected, signal it back to GUIslice
  if (bValid) {

    int nRawX,nRawY;
    int nInputX,nInputY;
    int nOutputX,nOutputY;

    // Input assignment
    nRawX = m_nLastRawX;
    nRawY = m_nLastRawY;

    // Handle any hardware swapping in native orientation
    // This is done prior to any flip/swap as a result of
    // rotation away from the native orientation.
    // In most cases, the following is not used, but there
    // may be touch modules that have swapped their X&Y convention.
    if (pGui->bTouchRemapYX) {
      nRawX = m_nLastRawY;
      nRawY = m_nLastRawX;
    }

    nInputX = nRawX;
    nInputY = nRawY;

    // For resistive displays, perform constraint and scaling
    #if defined(DRV_TOUCH_TYPE_RES)
      if (pGui->bTouchRemapEn) {
        // Perform scaling from input to output
        // - Calibration done in native orientation (GSLC_ROTATE=0)
        // - Input to map() is done with raw unswapped X,Y
        // - map() and constrain() done with native dimensions and
        //   native calibration
        // - Swap & Flip done to output of map/constrain according
        //   to GSLC_ROTATE
        //
        #if defined(DBG_TOUCH)
          GSLC_DEBUG_PRINT("DBG: remapX: (%d,%d,%d,%d,%d)\n", nInputX, pGui->nTouchCalXMin, pGui->nTouchCalXMax, 0, nDispOutMaxX);
          GSLC_DEBUG_PRINT("DBG: remapY: (%d,%d,%d,%d,%d)\n", nInputY, pGui->nTouchCalYMin, pGui->nTouchCalYMax, 0, nDispOutMaxY);
        #endif
        nOutputX = map(nInputX, pGui->nTouchCalXMin, pGui->nTouchCalXMax, 0, nDispOutMaxX);
        nOutputY = map(nInputY, pGui->nTouchCalYMin, pGui->nTouchCalYMax, 0, nDispOutMaxY);
        // Perform constraining to OUTPUT boundaries
        nOutputX = constrain(nOutputX, 0, nDispOutMaxX);
        nOutputY = constrain(nOutputY, 0, nDispOutMaxY);
      } else {
        // No scaling from input to output
        nOutputX = nInputX;
        nOutputY = nInputY;
      }
    #else
      // No scaling from input to output
      nOutputX = nInputX;
      nOutputY = nInputY;
    #endif  // DRV_TOUCH_TYPE_RES
  
    #ifdef DBG_TOUCH
    GSLC_DEBUG_PRINT("DBG: PreRotate: x=%u y=%u\n", nOutputX, nOutputY);
    #if defined(DRV_TOUCH_TYPE_RES)
      GSLC_DEBUG_PRINT("DBG: RotateCfg: remap=%u nSwapXY=%u nFlipX=%u nFlipY=%u\n",
        pGui->bTouchRemapEn,pGui->nSwapXY,pGui->nFlipX,pGui->nFlipY);
    #endif // DRV_TOUCH_TYPE_RES
    #endif // DBG_TOUCH

    // Perform remapping due to current orientation
    if (pGui->bTouchRemapEn) {
      // Perform any requested swapping of input axes
      if (pGui->nSwapXY) {
        int16_t nOutputXTmp = nOutputX;
        nOutputX = nOutputY;
        nOutputY = nOutputXTmp;
        // Perform any requested output axis flipping
        // TODO: Collapse these cases
        if (pGui->nFlipX) {
          nOutputX = nDispOutMaxY - nOutputX;
        }
        if (pGui->nFlipY) {
          nOutputY = nDispOutMaxX - nOutputY;
        }
      } else {
        // Perform any requested output axis flipping
        if (pGui->nFlipX) {
          nOutputX = nDispOutMaxX - nOutputX;
        }
        if (pGui->nFlipY) {
          nOutputY = nDispOutMaxY - nOutputY;
        }
      }
    }

    // Final assignment
    *pnX          = nOutputX;
    *pnY          = nOutputY;
    *pnPress      = m_nLastRawPress;
    *peInputEvent = GSLC_INPUT_TOUCH;
    *pnInputVal   = 0;

    // Print output for debug
    #ifdef DBG_TOUCH
    GSLC_DEBUG_PRINT("DBG: Touch Press=%u Raw[%d,%d] Out[%d,%d]\n",
        m_nLastRawPress,m_nLastRawX,m_nLastRawY,nOutputX,nOutputY);
    #endif

    // Return with indication of new value
    return true;
  }

  // No new value
  return false;
}

#endif // DRV_TOUCH_*


// -----------------------------------------------------------------------
// Dynamic Screen rotation and Touch axes swap/flip functions
// -----------------------------------------------------------------------

/// Change display rotation and any associated touch orientation
bool gslc_DrvRotate(gslc_tsGui* pGui, uint8_t nRotation)
{
  bool bChange = true;
  bool bSupportRotation = true;

  // Determine if the new orientation has swapped axes
  // versus the native orientation (0)
  bool bSwap = false;
  if ((nRotation == 1) || (nRotation == 3)) {
    bSwap = true;
  }
  (void)bSwap; // May be Unused in some driver modes

  // Did the orientation change?
  if (nRotation == pGui->nRotation) {
    // Orientation did not change -- indicate this by returning
    // false so that we can avoid a redraw
    bChange = false;
  }

  // Update the GUI rotation member
  pGui->nRotation = nRotation;

  // Inform the display to adjust the orientation and
  // update the saved display dimensions
  #if defined(DRV_DISP_ADAGFX_ILI9341) || defined(DRV_DISP_ADAGFX_ILI9341_STM) || defined(DRV_DISP_ADAGFX_ILI9341_T3)
    pGui->nDisp0W = ILI9341_TFTWIDTH;
    pGui->nDisp0H = ILI9341_TFTHEIGHT;
    m_disp.setRotation(pGui->nRotation);
    if (!bSwap) {
      pGui->nDispW = ILI9341_TFTWIDTH;
      pGui->nDispH = ILI9341_TFTHEIGHT;
    } else {
      pGui->nDispW = ILI9341_TFTHEIGHT;
      pGui->nDispH = ILI9341_TFTWIDTH;
    }

  #elif defined(DRV_DISP_ADAGFX_ILI9341_8BIT)
    pGui->nDisp0W = ILI9341_TFTWIDTH;
    pGui->nDisp0H = ILI9341_TFTHEIGHT;
    m_disp.setRotation(pGui->nRotation);
    if (!bSwap) {
      pGui->nDispW = ILI9341_TFTWIDTH;
      pGui->nDispH = ILI9341_TFTHEIGHT;
    } else {
      pGui->nDispW = ILI9341_TFTHEIGHT;
      pGui->nDispH = ILI9341_TFTWIDTH;
    }

  #elif defined(DRV_DISP_ADAGFX_ILI9341_DUE_MB)
    pGui->nDisp0W = ILI9341_TFTWIDTH;
    pGui->nDisp0H = ILI9341_TFTHEIGHT;
    m_disp.setRotation((iliRotation)pGui->nRotation);
    if (!bSwap) {
      pGui->nDispW = ILI9341_TFTWIDTH;
      pGui->nDispH = ILI9341_TFTHEIGHT;
    } else {
      pGui->nDispW = ILI9341_TFTHEIGHT;
      pGui->nDispH = ILI9341_TFTWIDTH;
    }

  #elif defined(DRV_DISP_ADAGFX_SSD1306)
    // No support for rotation in SSD1306 library
    pGui->nDisp0W = SSD1306_LCDWIDTH;
    pGui->nDisp0H = SSD1306_LCDHEIGHT;
    pGui->nDispW = SSD1306_LCDWIDTH;
    pGui->nDispH = SSD1306_LCDHEIGHT;

  #elif defined(DRV_DISP_ADAGFX_ST7735)
    // TODO: To support ST7789, init() is called with the display dimensions
    //       instead of initR() with the initialization enumeration.
    m_disp.setRotation(0);
    pGui->nDisp0W = m_disp.width();
    pGui->nDisp0H = m_disp.height();
    m_disp.setRotation(pGui->nRotation);
    pGui->nDispW = m_disp.width();
    pGui->nDispH = m_disp.height();

  #elif defined(DRV_DISP_ADAGFX_HX8347)
    m_disp.setRotation(0);
    pGui->nDisp0W = m_disp.width();
    pGui->nDisp0H = m_disp.height();
    m_disp.setRotation(pGui->nRotation);
    pGui->nDispW = m_disp.width();
    pGui->nDispH = m_disp.height();

  #elif defined(DRV_DISP_ADAGFX_HX8357)
    m_disp.setRotation(0);
    pGui->nDisp0W = HX8357_TFTWIDTH;
    pGui->nDisp0H = HX8357_TFTHEIGHT;
    m_disp.setRotation(pGui->nRotation);
    if (!bSwap) {
      pGui->nDispW = HX8357_TFTWIDTH;
      pGui->nDispH = HX8357_TFTHEIGHT;
    } else {
      pGui->nDispW = HX8357_TFTHEIGHT;
      pGui->nDispH = HX8357_TFTWIDTH;
    }

  #elif defined(DRV_DISP_ADAGFX_PCD8544)
    pGui->nDisp0W = LCDWIDTH;
    pGui->nDisp0H = LCDHEIGHT;
    // No support for rotation in PCD8544 library
    pGui->nDispW = LCDWIDTH;
    pGui->nDispH = LCDHEIGHT;

  #elif defined(DRV_DISP_ADAGFX_RA8875)
    // No support for rotation in Adafruit_RA8875 library
    bSupportRotation = false;
    pGui->nDisp0W = m_disp.width();
    pGui->nDisp0H = m_disp.height();
    pGui->nDispW = m_disp.width();
    pGui->nDispH = m_disp.height();

  #elif defined(DRV_DISP_ADAGFX_MCUFRIEND)
    m_disp.setRotation(0);
    pGui->nDisp0W = m_disp.width();
    pGui->nDisp0H = m_disp.height();
    m_disp.setRotation(pGui->nRotation);
    pGui->nDispW = m_disp.width();
    pGui->nDispH = m_disp.height();

  #else
    // Report error for unsupported display mode
    // - If we don't trap this condition, the GUI dimensions will be incorrect
    #error "ERROR: DRV_DISP_* mode not supported in DrvRotate initialization"

  #endif

  // Update the clipping region
  gslc_tsRect rClipRect = { 0,0,pGui->nDispW,pGui->nDispH };
  gslc_DrvSetClipRect(pGui, &rClipRect);

  if (!bSupportRotation) {
    // No support for rotation, so override rotation indicator to 0
    // This will also ensure that nSwapXY / nFlipX / nFlipY all remain 0
    pGui->nRotation = 0;
    // Ensure no redraw forced due to change in rotation value
    bChange = false;
  }

  // Now update the touch remapping
  #if !defined(DRV_TOUCH_NONE)
    pGui->nSwapXY = TOUCH_ROTATION_SWAPXY(pGui->nRotation);
    pGui->nFlipX = TOUCH_ROTATION_FLIPX(pGui->nRotation);
    pGui->nFlipY = TOUCH_ROTATION_FLIPY(pGui->nRotation);
  #endif // !DRV_TOUCH_NONE

  // Mark the current page ask requiring redraw
  // if the rotation value changed
  if (bChange) {
    gslc_PageRedrawSet( pGui, true );
  }

  return true;
}


// =======================================================================
// Private Functions
// =======================================================================


// Convert from RGB struct to native screen format
// TODO: Use 32bit return type?
uint16_t gslc_DrvAdaptColorToRaw(gslc_tsColor nCol)
{
  uint16_t nColRaw = 0;

  #if defined(DRV_DISP_ADAGFX_SSD1306) || defined(DRV_DISP_ADAGFX_PCD8544)
    // Monochrome
    if ((nCol.r == 0) && (nCol.g == 0) && (nCol.b == 0)) { // GSLC_COL_BLACK
      nColRaw = 0;  // BLACK
    } else {
      nColRaw = 1;  // WHITE
    }

  #else
    // Default to RGB565
    nColRaw |= (((nCol.r & 0xF8) >> 3) << 11); // Mask: 1111 1000 0000 0000
    nColRaw |= (((nCol.g & 0xFC) >> 2) <<  5); // Mask: 0000 0111 1110 0000
    nColRaw |= (((nCol.b & 0xF8) >> 3) <<  0); // Mask: 0000 0000 0001 1111

  #endif // DRV_DISP_ADAGFX_*

  return nColRaw;
}

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // Compiler guard for requested driver