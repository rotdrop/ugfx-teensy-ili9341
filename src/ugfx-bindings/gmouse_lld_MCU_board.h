/*
 * This file is subject to the terms of the GFX License. If a copy of
 * the license was not distributed with this file, you can obtain one at:
 *
 *              http://ugfx.io/license.html
 */

#ifndef _LLD_GMOUSE_MCU_BOARD_H
#define _LLD_GMOUSE_MCU_BOARD_H

#include "gfx.h"
#include <ugfx/src/ginput/ginput_driver_mouse.h>

// Resolution and Accuracy Settings
#define GMOUSE_MCU_PEN_CALIBRATE_ERROR            8
#define GMOUSE_MCU_PEN_CLICK_ERROR                6
#define GMOUSE_MCU_PEN_MOVE_ERROR                 4
#define GMOUSE_MCU_FINGER_CALIBRATE_ERROR        14
#define GMOUSE_MCU_FINGER_CLICK_ERROR            18
#define GMOUSE_MCU_FINGER_MOVE_ERROR             14
#define GMOUSE_MCU_Z_MIN                          0  // The minimum Z reading
#define GMOUSE_MCU_Z_MAX                        100  // The maximum Z reading
#define GMOUSE_MCU_Z_TOUCHON                     80  // Values between this and Z_MAX are definitely pressed
#define GMOUSE_MCU_Z_TOUCHOFF                    70  // Values between this and Z_MIN are definitely not pressed

typedef struct GMouseDriverData
{
  GMouse m;
  void *data;
} GMouseDriverData;
  
// How much extra data to allocate at the end of the GMouse structure for the board's use
#define GMOUSE_MCU_BOARD_DATA_SIZE (sizeof(GMouseDriverData) - sizeof(GMouse))
#define GMOUSE_MCU_BOARD_DEINIT GFXON

#ifdef __cplusplus
extern "C" {
#endif

  gBool init_board(GMouse *m, unsigned driverinstance);
  void deinit_board(GMouse *m);
  gBool read_xyz(GMouse *m, GMouseReading *prd);

#ifdef __cplusplus
}
#endif

#endif /* _LLD_GMOUSE_MCU_BOARD_H */
