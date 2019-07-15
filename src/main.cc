#include <WProgram.h>
//#include <i2c_t3.h>
#include "data/guislice/guislice.ino"

extern "C" volatile uint32_t systick_millis_count;

// Memory
#define MEM_LEN 256
char databuf[MEM_LEN];
int count;
uint8_t target = 0x77; // target Slave address

#define TFT_CS 10
#define SD_CS 15

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

    loop();

#if 0
    digitalWriteFast(LED_BUILTIN, HIGH);
    delay(50);
    digitalWriteFast(LED_BUILTIN, LOW);
    delay(4950);
#endif
  }
}
