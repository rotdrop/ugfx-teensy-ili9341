#include <WProgram.h>

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
volatile auto blah = FTMModule::instance(0).PWMLOAD;

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
    blah = FTMModule::instance(i).PWMLOAD;
  }
}

ServoController servos;

extern "C" volatile uint32_t systick_millis_count;

int main(void)
{
  Serial.begin(9600);
  pinMode(13, OUTPUT);
  while (1) {
    while (auto x = Serial.available()) {
      Serial.read();
      Serial.println("serial input");
      Serial.println(x, DEC);
    }
    Serial.println("Teensy 3.6");
    Serial.println(jiffies, DEC);
    Serial.println("OVF");
    Serial.println(ServoController::overflow_, DEC);
    Serial.println((ServoController::tick_ + ServoController::frequency_ - 1) / ServoController::frequency_, DEC);
    Serial.println(FTMModule::instance(0).CH[0].CV, DEC);
    Serial.println(FTMModule::instance(0).PWMLOAD, HEX);
    Serial.println(blah, HEX);
    digitalWriteFast(13, HIGH);
    delay(50);
    digitalWriteFast(13, LOW);
    delay(5000);
  }
}
