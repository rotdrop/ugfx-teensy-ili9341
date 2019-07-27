#include <WProgram.h>
//#include <i2c_t3.h>
#include <ugfx/gfx.h>

extern "C" volatile uint32_t systick_millis_count;

// Memory
#define MEM_LEN 256
char databuf[MEM_LEN];
int count;
uint8_t target = 0x77; // target Slave address

#define TFT_CS 10
#define SD_CS 15

void setup()
{
  coord_t               width, height;
  coord_t               i, j;

  // Initialize and clear the display
  gfxInit();

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
