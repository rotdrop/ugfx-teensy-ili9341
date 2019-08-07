#include <TouchScreen.h>
#include "gmouse_lld_MCU_board.h"

struct GMouseTouchScreenDriver
  : GMouse
{
  TouchScreen* driver;
};

gBool init_board(GMouse *m, unsigned driverInstance)
{
  static_assert(sizeof(GMouseTouchScreenDriver) == sizeof(GMouseDriverData),
                "Driver data hack does not work.");
  if (driverInstance > 0) {
    return gFalse;
  }

  auto& board = static_cast<GMouseTouchScreenDriver&>(*m);

  board.driver = new TouchScreen(0, 0, 0, 0, 0);
  
  return gFalse;
}

void deinit_board(GMouse *m)
{
  auto& board = static_cast<GMouseTouchScreenDriver&>(*m);
  delete board.driver;
}

gBool read_xyz(GMouse *m, GMouseReading *prd)
{
  return gFalse;
}

