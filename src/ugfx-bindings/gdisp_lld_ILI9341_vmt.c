// The sole purpose of this driver is to really "instantiate"
//
// const GDISPVMT GDISP_DRIVER_VMT[1]
//
// which -- right or wrong -- does not happen when piping
// gdisp_driver.h through a or certain c++ compilers

#include "gfx.h"
#define GDISP_DRIVER_VMT GDISPVMT_ILI9341
#include "gdisp_lld_config.h"
#include <ugfx/src/gdisp/gdisp_driver.h>
