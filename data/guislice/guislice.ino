// FILE: guislice.ino
//
// GUIslice Builder Generated File
//
// ARDUINO NOTES:
// - GUIslice_config.h must be edited to match the pinout connections
//   between the Arduino CPU and the display controller (see ADAGFX_PIN_*).
//

#include "GUIslice.h"
#include "GUIslice_ex.h"
#include "GUIslice_drv.h"

#include <Adafruit_GFX.h>

// Note that font files are located within the Adafruit-GFX library folder:
//<Fonts !Start!>
//<Fonts !End!>

// ------------------------------------------------
// Defines for resources
// ------------------------------------------------
//<Resources !Start!>
//<Resources !End!>

// ------------------------------------------------
// Enumerations for pages, elements, fonts, images
// ------------------------------------------------
//<Enum !Start!>
enum {E_PG_MAIN,E_PG2,E_PG3};
enum {E_BTN2,E_BTN3,E_BTN4,E_BTN5
      ,E_BTN6,E_BTN7,E_PROGRESS1,E_PROGRESS2
      ,E_SCROLLBAR2,E_TXTBOX2};
enum {E_FONT_TXT1,E_FONT_TXT2};
//<Enum !End!>

// ------------------------------------------------
// Instantiate the GUI
// ------------------------------------------------

// Define the maximum number of elements per page
//<ElementDefines !Start!>
#define MAX_PAGE                3
#define MAX_FONT                2
#define MAX_ELEM_PG_MAIN 5
#define MAX_ELEM_PG_MAIN_RAM MAX_ELEM_PG_MAIN
#define MAX_ELEM_PG2 6
#define MAX_ELEM_PG2_RAM MAX_ELEM_PG2
#define MAX_ELEM_PG3 3
#define MAX_ELEM_PG3_RAM MAX_ELEM_PG3
//<ElementDefines !End!>

// GUI Elements
gslc_tsGui                      m_gui;
gslc_tsDriver                   m_drv;
gslc_tsFont                     m_asFont[MAX_FONT];
gslc_tsPage                     m_asPage[MAX_PAGE];

//<GUI_Extra_Elements !Start!>
gslc_tsElem                 m_asPage1Elem[MAX_ELEM_PG_MAIN_RAM];
gslc_tsElemRef              m_asPage1ElemRef[MAX_ELEM_PG_MAIN];
gslc_tsElem                 m_asPage2Elem[MAX_ELEM_PG2_RAM];
gslc_tsElemRef              m_asPage2ElemRef[MAX_ELEM_PG2];
gslc_tsElem                 m_asPage3Elem[MAX_ELEM_PG3_RAM];
gslc_tsElemRef              m_asPage3ElemRef[MAX_ELEM_PG3];
gslc_tsXTextbox                 m_sTextbox2;
char                            m_acTextboxBuf2[1408]; // NRows=32 NCols=44
gslc_tsXGauge                   m_sXGauge[2];
gslc_tsXSlider                  m_sXSlider[1];

#define MAX_STR                 100

//<GUI_Extra_Elements !End!>

// ------------------------------------------------
// Save some element references for update loop access
// ------------------------------------------------
//<Save_References !Start!>
gslc_tsElemRef*  m_pElemProgress1  = NULL;
gslc_tsElemRef*  m_pElemProgress2  = NULL;
gslc_tsElemRef*  m_pElemTextbox2   = NULL;
//<Save_References !End!>

// ------------------------------------------------
// Program Globals
// ------------------------------------------------

// Define debug message function
static int16_t DebugOut(char ch) { if (ch == (char)'\n') Serial.println(""); else Serial.write(ch); return 0; }

// ------------------------------------------------
// Callback Methods
// ------------------------------------------------
// Common Button callback
bool CbBtnCommon(void* pvGui,void *pvElemRef,gslc_teTouch eTouch,int16_t nX,int16_t nY)
{
  gslc_tsElemRef* pElemRef = (gslc_tsElemRef*)(pvElemRef);
  gslc_tsElem* pElem = pElemRef->pElem;

  if ( eTouch == GSLC_TOUCH_UP_IN ) {
    // From the element's ID we can determine which button was pressed.
    Serial.print("Button ");
    Serial.println(pElem->nId, DEC);
    switch (pElem->nId) {
//<Button Enums !Start!>
      case E_BTN4:
        gslc_SetPageCur(&m_gui,2);
        break;
      case E_BTN5:
        gslc_SetPageCur(&m_gui,1);
        break;
      case E_BTN2:
        gslc_SetPageCur(&m_gui,2);
        break;
      case E_BTN3:
        gslc_SetPageCur(&m_gui,0);
        break;
      case E_BTN6:
        gslc_SetPageCur(&m_gui,1);
        break;
      case E_BTN7:
        gslc_SetPageCur(&m_gui,0);
        break;
//<Button Enums !End!>
      default:
        break;
    }
  }
  return true;
}
//<Draw Callback !Start!>
//<Draw Callback !End!>

// Callback function for when a slider's position has been updated
bool CbSlidePos(void* pvGui,void* pvElemRef,int16_t nPos)
{
  gslc_tsGui*     pGui      = (gslc_tsGui*)(pvGui);
  gslc_tsElemRef* pElemRef  = (gslc_tsElemRef*)(pvElemRef);
  gslc_tsElem*    pElem     = pElemRef->pElem;
  int16_t         nVal;

  // From the element's ID we can determine which slider was updated.
  switch (pElem->nId) {
//<Slider Enums !Start!>
    case E_SCROLLBAR2:
      // Fetch the slider position
      nVal = gslc_ElemXSliderGetPos(pGui,pElemRef);
      //TODO- Replace with slider handling code
      break;
//<Slider Enums !End!>
    default:
      break;
  }

  return true;
}
//<Tick Callback !Start!>
//<Tick Callback !End!>

// ------------------------------------------------
// Create page elements
// ------------------------------------------------
bool InitGUI()
{
  gslc_tsElemRef* pElemRef = NULL;

//<InitGUI !Start!>
  gslc_PageAdd(&m_gui,E_PG_MAIN,m_asPage1Elem,MAX_ELEM_PG_MAIN_RAM,m_asPage1ElemRef,MAX_ELEM_PG_MAIN);
  gslc_PageAdd(&m_gui,E_PG2,m_asPage2Elem,MAX_ELEM_PG2_RAM,m_asPage2ElemRef,MAX_ELEM_PG2);
  gslc_PageAdd(&m_gui,E_PG3,m_asPage3Elem,MAX_ELEM_PG3_RAM,m_asPage3ElemRef,MAX_ELEM_PG3);
  
  // Background flat color
  gslc_SetBkgndColor(&m_gui,GSLC_COL_BLACK);

  // -----------------------------------
  // PAGE: E_PG_MAIN
  
  // Create GSLC_ID_AUTO text label
  pElemRef = gslc_ElemCreateTxt(&m_gui,GSLC_ID_AUTO,E_PG_MAIN,(gslc_tsRect){10,10,60,20},
    (char*)"Page 1",0,E_FONT_TXT2);
  
  // create E_BTN4 button with text label
  pElemRef = gslc_ElemCreateBtnTxt(&m_gui,E_BTN4,E_PG_MAIN,
    (gslc_tsRect){10,200,30,30},(char*)"<<",0,E_FONT_TXT2,&CbBtnCommon);
  gslc_ElemSetFrameEn(&m_gui,pElemRef,true);
  
  // create E_BTN5 button with text label
  pElemRef = gslc_ElemCreateBtnTxt(&m_gui,E_BTN5,E_PG_MAIN,
    (gslc_tsRect){280,200,30,30},(char*)">>",0,E_FONT_TXT2,&CbBtnCommon);
  gslc_ElemSetFrameEn(&m_gui,pElemRef,true);

  // Create progress bar E_PROGRESS1 
  pElemRef = gslc_ElemXGaugeCreate(&m_gui,E_PROGRESS1,E_PG_MAIN,&m_sXGauge[0],
    (gslc_tsRect){10,50,100,100},0,100,0,GSLC_COL_GREEN,false);
  gslc_ElemXGaugeSetStyle(&m_gui,pElemRef, GSLCX_GAUGE_STYLE_RADIAL);
  gslc_ElemXGaugeSetIndicator(&m_gui,pElemRef,GSLC_COL_GREEN,45,5,false);

  // Create progress bar E_PROGRESS2 
  pElemRef = gslc_ElemXGaugeCreate(&m_gui,E_PROGRESS2,E_PG_MAIN,&m_sXGauge[1],
    (gslc_tsRect){210,50,100,100},0,100,0,GSLC_COL_GREEN,false);
  gslc_ElemXGaugeSetStyle(&m_gui,pElemRef, GSLCX_GAUGE_STYLE_RADIAL);
  gslc_ElemXGaugeSetIndicator(&m_gui,pElemRef,GSLC_COL_GREEN,45,5,false);

  // -----------------------------------
  // PAGE: E_PG2
  
  // create E_BTN2 button with text label
  pElemRef = gslc_ElemCreateBtnTxt(&m_gui,E_BTN2,E_PG2,
    (gslc_tsRect){280,200,30,30},(char*)">>",0,E_FONT_TXT2,&CbBtnCommon);
  gslc_ElemSetFrameEn(&m_gui,pElemRef,true);
  
  // create E_BTN3 button with text label
  pElemRef = gslc_ElemCreateBtnTxt(&m_gui,E_BTN3,E_PG2,
    (gslc_tsRect){10,200,30,30},(char*)"<<",0,E_FONT_TXT2,&CbBtnCommon);
  gslc_ElemSetFrameEn(&m_gui,pElemRef,true);
  
  // Create GSLC_ID_AUTO text label
  pElemRef = gslc_ElemCreateTxt(&m_gui,GSLC_ID_AUTO,E_PG2,(gslc_tsRect){10,10,74,20},
    (char*)"Page 2",0,E_FONT_TXT2);
   
  // Create wrapping box for textbox E_TXTBOX2 and scrollbar
  pElemRef = gslc_ElemCreateBox(&m_gui,GSLC_ID_AUTO,E_PG2,(gslc_tsRect){10,40,300,150});
  gslc_ElemSetCol(&m_gui,pElemRef,GSLC_COL_GRAY,GSLC_COL_BLACK,GSLC_COL_BLACK);
  
  // Create textbox
  pElemRef = gslc_ElemXTextboxCreate(&m_gui,E_TXTBOX2,E_PG2,&m_sTextbox2,
    (gslc_tsRect){10+2,40+4,300-23,150-5},E_FONT_TXT1,(char*)&m_acTextboxBuf2,32,44);
  gslc_ElemXTextboxWrapSet(&m_gui,pElemRef,true);
  gslc_ElemSetCol(&m_gui,pElemRef,GSLC_COL_GRAY,GSLC_COL_BLACK,GSLC_COL_BLACK);

  // Create vertical scrollbar for textbox
  pElemRef = gslc_ElemXSliderCreate(&m_gui,E_SCROLLBAR2,E_PG2,&m_sXSlider[0],
          (gslc_tsRect){10+300-21,40+4,20,150-8},0,100,0,5,true);
  gslc_ElemSetCol(&m_gui,pElemRef,GSLC_COL_GRAY,GSLC_COL_BLACK,GSLC_COL_BLACK);
  gslc_ElemXSliderSetPosFunc(&m_gui,pElemRef,&CbSlidePos);

  // -----------------------------------
  // PAGE: E_PG3
  
  // create E_BTN6 button with text label
  pElemRef = gslc_ElemCreateBtnTxt(&m_gui,E_BTN6,E_PG3,
    (gslc_tsRect){10,200,30,30},(char*)"<<",0,E_FONT_TXT2,&CbBtnCommon);
  gslc_ElemSetFrameEn(&m_gui,pElemRef,true);
  
  // create E_BTN7 button with text label
  pElemRef = gslc_ElemCreateBtnTxt(&m_gui,E_BTN7,E_PG3,
    (gslc_tsRect){280,200,30,30},(char*)">>",0,E_FONT_TXT2,&CbBtnCommon);
  gslc_ElemSetFrameEn(&m_gui,pElemRef,true);
  
  // Create GSLC_ID_AUTO text label
  pElemRef = gslc_ElemCreateTxt(&m_gui,GSLC_ID_AUTO,E_PG3,(gslc_tsRect){10,10,60,20},
    (char*)"Page 3",0,E_FONT_TXT2);
//<InitGUI !End!>

  return true;
}

void setup()
{
  // ------------------------------------------------
  // Initialize
  // ------------------------------------------------
  Serial.begin(9600);
  // Wait for USB Serial 
  //delay(1000);  // NOTE: Some devices require a delay after Serial.begin() before serial port can be used

  gslc_InitDebug(&DebugOut);

  if (!gslc_Init(&m_gui,&m_drv,m_asPage,MAX_PAGE,m_asFont,MAX_FONT)) { return; }

  // ------------------------------------------------
  // Load Fonts
  // ------------------------------------------------
//<Load_Fonts !Start!>
    if (!gslc_FontAdd(&m_gui,E_FONT_TXT1,GSLC_FONTREF_PTR,NULL,1)) { return; }
    if (!gslc_FontAdd(&m_gui,E_FONT_TXT2,GSLC_FONTREF_PTR,NULL,2)) { return; }
//<Load_Fonts !End!>

  // ------------------------------------------------
  // Create graphic elements
  // ------------------------------------------------
  InitGUI();

  // ------------------------------------------------
  // Save some element references for quick access
  // ------------------------------------------------
//<Quick_Access !Start!>
  m_pElemProgress1  = gslc_PageFindElemById(&m_gui,E_PG_MAIN,E_PROGRESS1);
  m_pElemProgress2  = gslc_PageFindElemById(&m_gui,E_PG_MAIN,E_PROGRESS2);
  m_pElemTextbox2   = gslc_PageFindElemById(&m_gui,E_PG2,E_TXTBOX2);
//<Quick_Access !End!>

//<Startup !Start!>
  // ------------------------------------------------
  // Start up display on first page
  // ------------------------------------------------
  gslc_SetPageCur(&m_gui,E_PG_MAIN);
//<Startup !End!>

}

// -----------------------------------
// Main event loop
// -----------------------------------
void loop()
{

  // ------------------------------------------------
  // Update GUI Elements
  // ------------------------------------------------
  
  //TODO - Add update code for any text, gauges, or sliders
  
  // ------------------------------------------------
  // Periodically call GUIslice update function
  // ------------------------------------------------
  gslc_Update(&m_gui);
    
}

