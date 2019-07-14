#include <WProgram.h>
//#include <i2c_t3.h>
#include "data/guislice/guislice.ino"

extern "C" volatile uint32_t systick_millis_count;

// Memory
#define MEM_LEN 256
char databuf[MEM_LEN];
int count;
uint8_t target = 0x77; // target Slave address

int main(void)
{
  setup();
  
  pinMode(LED_BUILTIN, OUTPUT);

  while (1) {
    while (auto x = Serial.available()) {
      Serial.read();
      Serial.println("serial input");
      Serial.println(x, DEC);
    }
    Serial.println("Teensy 3.2 GUI");

    loop();
    
    digitalWriteFast(LED_BUILTIN, HIGH);
    delay(50);
    digitalWriteFast(LED_BUILTIN, LOW);
    delay(4950);
  }
}
