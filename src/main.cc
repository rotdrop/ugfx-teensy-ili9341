#include <WProgram.h>
#include <i2c_t3.h>

#include "pins.hh"
#include "ftm.hh"

//using namespace CJHTeensy;

namespace CJHTeensy {

  static constexpr unsigned intLog2(unsigned value, unsigned log = 0)
  {
    return (value >> log) > 0 ? intLog2(value, log+1) : log;
  }  
  
  static constexpr unsigned deciUsToTicks(unsigned frequency, unsigned decius)
  {
    return decius * (frequency / 1000) / 10000 + decius * (frequency % 1000) / 10000000;
  }

  template<bool Centered = false>
  class ServoController
  {
   public:
    // FTM runs at F_BUS which is 60 MHz
    static constexpr uint8_t centeredFactor_ = Centered ? 2 : 1;
    static constexpr uint8_t frequency_ = 50U;
    static constexpr uint32_t clock_ = F_BUS;
    static constexpr uint8_t prescaler_ = intLog2(clock_ / (1U << 16) / frequency_ / centeredFactor_);
    static constexpr uint32_t tick_ = clock_ >> prescaler_;
    static constexpr uint16_t overflow_ = (tick_ + frequency_ - 1) / frequency_ / centeredFactor_;

    static constexpr uint16_t tickMin_ =     deciUsToTicks(tick_, 7500) / centeredFactor_;
    static constexpr uint16_t tickMax_ =    deciUsToTicks(tick_, 22500) / centeredFactor_;
    static constexpr uint16_t tickCenter_ = deciUsToTicks(tick_, 15000) / centeredFactor_;
  
   public:
    ServoController()
    {
      // synchronize all modules
      for (int i = 0; i < FTMModule::numInstances; ++i) {
        FTMModule::instance(i).SC = 0;
        FTMModule::instance(i).CNT = 0;
        FTMModule::instance(i).MOD = overflow_;
        FTMModule::instance(i).SC &= ~FTM_SC_TOIE;
        FTMModule::instance(i).SC = (Centered ? FTM_SC_CPWMS : 0)|FTM_SC_CLKS(1) | FTM_SC_PS(prescaler_);
        // enable PWMLOAD s.t. new valus are loaded at a defined time.
        FTMModule::instance(i).PWMLOAD = FTM_PWMLOAD_LDOK|((1U << FTMModule::numChannels[i]) - 1);
        FTMModule::instance(i).CONF &= ~FTM_CONF_GTBEOUT;
        FTMModule::instance(i).CONF |= FTM_CONF_GTBEEN;
        for (int pin = 0; pin < FTMModule::numChannels[i]; ++pin) {
          FTMModule::instance(i).CH[pin].SC &= ~FTM_CSC_CHIE;
          FTMModule::instance(i).CH[pin].SC = FTM_CSC_MSB|FTM_CSC_ELSB;
          FTMModule::instance(i).CH[pin].CV = tickCenter_;
          FTMModule::pinConfig(i, pin) = PORT_PCR_MUX(4) | PORT_PCR_DSE | PORT_PCR_SRE;
        }
      }
      FTMModule::instance(0).SC &= ~FTM_SC_TOF;
      FTMModule::instance(0).SC |= FTM_SC_TOIE;
      FTMModule::instance(0).CONF |= FTM_CONF_GTBEOUT;

      NVIC_ENABLE_IRQ(IRQ_FTM0);
    }
  };
} // CJHTeensy::

using ServoController = CJHTeensy::ServoController<true>;
using CJHTeensy::FTMModule;

volatile uint32_t jiffies;

void ftm0_isr()
{
  if (FTMModule::instance(0).SC & FTM_SC_TOF) {
    FTMModule::instance(0).SC &= ~FTM_SC_TOF;
  }
  ++jiffies;

  static bool forward = true;
  static unsigned tick = ServoController::tickCenter_;
  
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
    Serial.print("Teensy 3.6 @ ");
    Serial.println(jiffies, DEC);

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
