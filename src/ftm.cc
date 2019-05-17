#include "ftm.hh"

namespace CJHTeensy
{

  template<unsigned Id>
  void ftmIsr()
  {
    if (FTMModule::instance(Id).SC & FTM_SC_TOF) {
      FTMModule::instance(Id).SC &= ~FTM_SC_TOF;
      if (FTMOverflow<Id>::instance) {
        FTMOverflow<Id>::instance->handler();
      }  
    }
  }

  template<>
  FTMOverflow<0>* FTMOverflow<0>::instance = nullptr;  

  template<>
  FTMOverflow<1>* FTMOverflow<1>::instance = nullptr;  

  template<>
  FTMOverflow<2>* FTMOverflow<2>::instance = nullptr;  

  template<>
  FTMOverflow<3>* FTMOverflow<3>::instance = nullptr;  

}

using CJHTeensy::ftmIsr;

void ftm0_isr()
{
  ftmIsr<0>();
}

void ftm1_isr()
{
  ftmIsr<1>();
}

void ftm2_isr()
{
  ftmIsr<2>();
}

void ftm3_isr()
{
  ftmIsr<3>();
}
