#ifndef __CJH_TEENSY_SERVOCONROLLER_HH__
#define __CJH_TEENSY_SERVOCONROLLER_HH__

#include "pins.hh"
#include "ftm.hh"

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
    : FTMOverflow<0>
  {
   protected:
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
      for (int i = 0; i < FTMModule::numInstances; ++i) {
        FTMModule::instance(i).SC = 0;
        FTMModule::instance(i).CNT = 0;
        FTMModule::instance(i).MOD = overflow_;
        FTMModule::instance(i).SC &= ~FTM_SC_TOIE;
        FTMModule::instance(i).SC = (Centered ? FTM_SC_CPWMS : 0)|FTM_SC_CLKS(1) | FTM_SC_PS(prescaler_);
        FTMModule::instance(i).PWMLOAD = (1U << FTMModule::numChannels[i]) - 1;
        FTMModule::instance(i).CONF &= ~FTM_CONF_GTBEOUT;
        FTMModule::instance(i).CONF |= FTM_CONF_GTBEEN;
        for (int pin = 0; pin < FTMModule::numChannels[i]; ++pin) {
          FTMModule::instance(i).CH[pin].SC &= ~FTM_CSC_CHIE;
          FTMModule::instance(i).CH[pin].SC = FTM_CSC_MSB|FTM_CSC_ELSB;
          FTMModule::instance(i).CH[pin].CV = 0; // tickCenter_; 
          FTMModule::pinConfig(i, pin) = PORT_PCR_MUX(4) | PORT_PCR_DSE | PORT_PCR_SRE;
        }
        FTMModule::instance(i).PWMLOAD |= FTM_PWMLOAD_LDOK;        
      }
      FTMModule::instance(0).CONF |= FTM_CONF_GTBEOUT;
      start();
    }

    virtual ~ServoController()
    {
      for (int i = 0; i < FTMModule::numInstances; ++i) {
        FTMModule::instance(i).SC = 0; // disable
      }
      stop();
    }

    /**Callback called from the overflow interrupt. Typically should be used to update the duty-cycles.
     */
    virtual void atPeriodEnd()
    {
      return;
    }
  };

} // CJHTeensy::

#endif // __CJH_TEENSY_SERVOCONROLLER_HH__
