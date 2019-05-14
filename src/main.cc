#include <WProgram.h>

int main(void)
{
  pinMode(13, OUTPUT);
  while (1) {
    digitalWriteFast(13, HIGH);
    delay(50);
    digitalWriteFast(13, LOW);
    delay(5000);
  }
}
