#include <WProgram.h>

int main(void)
{
  Serial.begin(9600);
  pinMode(13, OUTPUT);
  while (1) {
    Serial.println("Teensy 3.6");
    digitalWriteFast(13, HIGH);
    delay(50);
    digitalWriteFast(13, LOW);
    delay(5000);
  }
}
