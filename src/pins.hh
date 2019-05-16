#ifndef __CJH_TEENSY_PINS_HH__
#define __CJH_TEENSY_PINS_HH__

#include <cstdint>
#include <core_pins.h>

namespace CJHTeensy
{
  static constexpr volatile uint32_t& pinConfig(unsigned pin)
  {
    switch (pin) {
    case 0: return CORE_PIN0_CONFIG;
    case 1: return CORE_PIN1_CONFIG;
    case 2: return CORE_PIN2_CONFIG;
    case 3: return CORE_PIN3_CONFIG;
    case 4: return CORE_PIN4_CONFIG;
    case 5: return CORE_PIN5_CONFIG;
    case 6: return CORE_PIN6_CONFIG;
    case 7: return CORE_PIN7_CONFIG;
    case 8: return CORE_PIN8_CONFIG;
    case 9: return CORE_PIN9_CONFIG;
    case 10: return CORE_PIN10_CONFIG;
    case 11: return CORE_PIN11_CONFIG;
    case 12: return CORE_PIN12_CONFIG;
    case 13: return CORE_PIN13_CONFIG;
    case 14: return CORE_PIN14_CONFIG;
    case 15: return CORE_PIN15_CONFIG;
    case 16: return CORE_PIN16_CONFIG;
    case 17: return CORE_PIN17_CONFIG;
    case 18: return CORE_PIN18_CONFIG;
    case 19: return CORE_PIN19_CONFIG;
    case 20: return CORE_PIN20_CONFIG;
    case 21: return CORE_PIN21_CONFIG;
    case 22: return CORE_PIN22_CONFIG;
    case 23: return CORE_PIN23_CONFIG;
    case 24: return CORE_PIN24_CONFIG;
    case 25: return CORE_PIN25_CONFIG;
    case 26: return CORE_PIN26_CONFIG;
    case 27: return CORE_PIN27_CONFIG;
    case 28: return CORE_PIN28_CONFIG;
    case 29: return CORE_PIN29_CONFIG;
    case 30: return CORE_PIN30_CONFIG;
    case 31: return CORE_PIN31_CONFIG;
    case 32: return CORE_PIN32_CONFIG;
    case 33: return CORE_PIN33_CONFIG;
    case 34: return CORE_PIN34_CONFIG;
    case 35: return CORE_PIN35_CONFIG;
    case 36: return CORE_PIN36_CONFIG;
    case 37: return CORE_PIN37_CONFIG;
    case 38: return CORE_PIN38_CONFIG;
    case 39: return CORE_PIN39_CONFIG;
    case 40: return CORE_PIN40_CONFIG;
    case 41: return CORE_PIN41_CONFIG;
    case 42: return CORE_PIN42_CONFIG;
    case 43: return CORE_PIN43_CONFIG;
    case 44: return CORE_PIN44_CONFIG;
    case 45: return CORE_PIN45_CONFIG;
    case 46: return CORE_PIN46_CONFIG;
    case 47: return CORE_PIN47_CONFIG;
    case 48: return CORE_PIN48_CONFIG;
    case 49: return CORE_PIN49_CONFIG;
    case 50: return CORE_PIN50_CONFIG;
    case 51: return CORE_PIN51_CONFIG;
    case 52: return CORE_PIN52_CONFIG;
    case 53: return CORE_PIN53_CONFIG;
    case 54: return CORE_PIN54_CONFIG;
    case 55: return CORE_PIN55_CONFIG;
    case 56: return CORE_PIN56_CONFIG;
    case 57: return CORE_PIN57_CONFIG;
    case 58: return CORE_PIN58_CONFIG;
    case 59: return CORE_PIN59_CONFIG;
    case 60: return CORE_PIN60_CONFIG;
    case 61: return CORE_PIN61_CONFIG;
    case 62: return CORE_PIN62_CONFIG;
    default:
    case 63: return CORE_PIN63_CONFIG;
    }
  }

} // CJHTeensy::

#endif // __CJH_TEENSY_PINS_HH__
