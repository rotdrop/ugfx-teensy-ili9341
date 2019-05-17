#include <WProgram.h>
#include <i2c_t3.h>

#include "pins.hh"
#include "servocontroller.hh"

using CJHTeensy::FTMModule;

class ServoController
  : CJHTeensy::ServoController<true>
{
  using BaseType = CJHTeensy::ServoController<true>;
 protected:
  bool forward = true;
  unsigned tick = BaseType::tickCenter_;
  
  virtual void handler(void)
  {
    for (int i = 0; i < FTMModule::numInstances; ++i) {
      for (int pin = 0; pin < FTMModule::numChannels[i]; ++pin) {
        if (forward) {
          if (tick < ServoController::tickMax_) {
            ++tick;
          } else {
            forward = !forward;
          }
        } else {
          if (tick > ServoController::tickMin_) {
            --tick;
          } else {
            forward = !forward;
          }
        }   
        FTMModule::instance(i).CH[pin].CV = tick;
      }
      FTMModule::instance(i).PWMLOAD |= FTM_PWMLOAD_LDOK;
    }
  }
};

ServoController servos;

extern "C" volatile uint32_t systick_millis_count;

// Memory
#define MEM_LEN 256
char databuf[MEM_LEN];
int count;
uint8_t target = 0x77; // target Slave address

int main(void)
{
  Serial.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);

  // Setup for Master mode, pins 18/19, external pullups, 400kHz, 200ms default timeout
  Wire.begin(I2C_MASTER, 0x00, I2C_PINS_18_19, I2C_PULLUP_EXT, 400000);
  Wire.setDefaultTimeout(200000); // 200ms
  
  while (1) {
    while (auto x = Serial.available()) {
      Serial.read();
      Serial.println("serial input");
      Serial.println(x, DEC);
    }
    Serial.println("Teensy 3.6");

    // Construct data message
    sprintf(databuf, "Data Message #%d", count++);

    // Print message
    Serial.printf("Sending to Slave: '%s' ", databuf);
        
    // Transmit to Slave
    Wire.beginTransmission(target);   // Slave address
    Wire.write(databuf,strlen(databuf)+1); // Write string to I2C Tx buffer (incl. string null at end)
    Wire.endTransmission();           // Transmit to Slave

    // Check if error occured
    if(Wire.getError()) {
      Serial.println("FAIL\n");
    } else {
      Serial.println("OK\n");
    }

    // Print message
    Serial.print("Reading from Slave: ");
        
    // Read from Slave
    Wire.requestFrom(target, (size_t)MEM_LEN); // Read from Slave (string len unknown, request full buffer)
    
    // Check if error occured
    if(Wire.getError()) {
      Serial.print("FAIL\n");
    } else {
      // If no error then read Rx data into buffer and print
      Wire.read(databuf, Wire.available());
      Serial.printf("'%s' OK\n",databuf);
    }
    
    digitalWriteFast(LED_BUILTIN, HIGH);
    delay(50);
    digitalWriteFast(LED_BUILTIN, LOW);
    delay(4950);
  }
}
