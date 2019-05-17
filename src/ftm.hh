#ifndef __CJH_TEENSY_FTM_HH__
#define __CJH_TEENSY_FTM_HH__

#include <cstdint>
#include "pins.hh"

namespace CJHTeensy
{
  
  struct FTMModule // FTM register block (this layout is compatible to a TPM register block)
  {
    struct Channel // FTM & TPM Channels
    {
      uint32_t SC;
      uint32_t CV;
    };
    uint32_t SC;
    uint32_t CNT;
    uint32_t MOD;
    Channel CH[8];
    uint32_t CNTIN;
    uint32_t STATUS;
    uint32_t MODE;
    uint32_t SYNC;
    uint32_t OUTINIT;
    uint32_t OUTMASK;
    uint32_t COMBINE;
    uint32_t DEADTIME;
    uint32_t EXTTRIG;
    uint32_t POL;
    uint32_t FMS;
    uint32_t FILTER;
    uint32_t FLTCTRL;
    uint32_t QDCTRL;
    uint32_t CONF;
    uint32_t FLTPOL;
    uint32_t SYNCONF; 
    uint32_t INVCTRL;
    uint32_t SWOCTRL;
    uint32_t PWMLOAD;

    static constexpr int numInstances = 4;
    static constexpr int numChannels[numInstances] = {
      8, 2, 2, 8
    };
    static constexpr int irq[numInstances] = {
      42, 43, 44, 71
    }; 
    static constexpr uint32_t addr[numInstances] = {
      0x40038000, 0x40039000, 0x400B8000, 0x400B9000
    };
    static constexpr unsigned pins[numInstances][8] = {
      {
        CORE_FTM0_CH0_PIN, // PORTC_PCR1
        CORE_FTM0_CH1_PIN, // PORTC_PCR2
        CORE_FTM0_CH2_PIN, // PORTC_PCR3
        CORE_FTM0_CH3_PIN, // PORTC_PCR4
        CORE_FTM0_CH4_PIN, // PORTD_PCR4
        CORE_FTM0_CH5_PIN, // PORTD_PCR5
        CORE_FTM0_CH6_PIN, // PORTD_PCR6 
        CORE_FTM0_CH7_PIN, // 
      },
      {
        CORE_FTM1_CH0_PIN,
        CORE_FTM1_CH1_PIN,
      },
      {
        CORE_FTM2_CH0_PIN,
        CORE_FTM2_CH1_PIN,
      },
      {
        CORE_FTM3_CH0_PIN,
        CORE_FTM3_CH1_PIN,
        CORE_FTM3_CH2_PIN,
        CORE_FTM3_CH3_PIN,
        CORE_FTM3_CH4_PIN,
        CORE_FTM3_CH5_PIN,
        CORE_FTM3_CH6_PIN,
        CORE_FTM3_CH7_PIN,
      },
    };
    static constexpr volatile FTMModule& instance(int id) 
    {
      return *reinterpret_cast<volatile FTMModule *>(FTMModule::addr[id]);
    }
    static constexpr volatile uint32_t& pinConfig(int id, int channel)
    {
      return CJHTeensy::pinConfig(pins[id][channel]);
    }
  };

  template<unsigned Id>
  class FTMOverflow
  {
    static_assert(Id <= 3, "At most 4 FTM's are available");
   protected:
    template<unsigned Nr>
    friend void ftmIsr();

    static FTMOverflow* instance;
    FTMOverflow* previous;
    
    FTMOverflow()
      : previous(instance)
    {
      instance = this;
    }

    virtual ~FTMOverflow()
    {
      stop();
      instance = previous;
    }

    static void start()
    {
      FTMModule::instance(Id).SC &= ~FTM_SC_TOF;
      FTMModule::instance(Id).SC |= FTM_SC_TOIE;

      switch (Id) {
      case 0: NVIC_ENABLE_IRQ(IRQ_FTM0); break;
      case 1: NVIC_ENABLE_IRQ(IRQ_FTM0); break;
      case 2: NVIC_ENABLE_IRQ(IRQ_FTM0); break;
      default:
      case 3: NVIC_ENABLE_IRQ(IRQ_FTM0); break;
      }
    }

    static void stop()
    {
      FTMModule::instance(Id).SC = 0;
      
      switch (Id) {
      case 0: NVIC_DISABLE_IRQ(IRQ_FTM0); break;
      case 1: NVIC_DISABLE_IRQ(IRQ_FTM0); break;
      case 2: NVIC_DISABLE_IRQ(IRQ_FTM0); break;
      default:
      case 3: NVIC_DISABLE_IRQ(IRQ_FTM0); break;
      }
    }
    
    virtual void handler()
    {}
  };  

} // CJHTeensy::

#endif // __CJH_TEENSY_FTM_HH__