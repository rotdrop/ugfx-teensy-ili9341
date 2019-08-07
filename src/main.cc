/*
 *  teensy-touchscreen-gui -- proof-of-concept TFT with touchscren for Teensy3
 *  Copyright (C) 2019 Claus-Justus Heine <himself@claus-justus-heine.de>
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include <WProgram.h>
#include "ILI9341-driver.hh"
//#include <i2c_t3.h>
#include <ugfx/gfx.h>

extern "C" volatile uint32_t systick_millis_count;

#define TFT_CS  10
#define TFT_DC   9
#define TFT_RST 26
#define SD_CS   15

static ILI9341Driver tftDriver(TFT_CS, TFT_DC, TFT_RST);

void setup()
{
  coord_t               width, height;
  coord_t               i, j;

  // Initialize and clear the display
  gfxInit();
#if 0

  // Get the screen size
  width = gdispGetWidth();
  height = gdispGetHeight();

  // Draw something
  gdispDrawBox(10, 10, width / 2, height / 2, Yellow);
  gdispFillArea(width / 2, height / 2, width / 2 - 10, height / 2 - 10, Blue);
  gdispDrawLine(5, 30, width - 50, height - 40, Red);

  for (i = 5, j = 0; i < width && j < height; i += 7, j += i / 20) {
    gdispDrawPixel(i, j, White);
  }
#endif
}

int main(void)
{
  pinMode(LED_BUILTIN, OUTPUT);

  digitalWrite(SD_CS, HIGH);
  pinMode(SD_CS, OUTPUT);
  digitalWrite(SD_CS, HIGH);

  digitalWrite(TFT_CS, HIGH);
  pinMode(TFT_CS, OUTPUT);
  digitalWrite(TFT_CS, HIGH);

  setup();

  while (1) {
    while (auto x = Serial.available()) {
      Serial.read();
      Serial.println("serial input");
      Serial.println(x, DEC);
    }
    //Serial.println("Teensy 3.2 GUI");

    //loop();

#if 0
    digitalWriteFast(LED_BUILTIN, HIGH);
    delay(50);
    digitalWriteFast(LED_BUILTIN, LOW);
    delay(4950);
#endif
  }
}
