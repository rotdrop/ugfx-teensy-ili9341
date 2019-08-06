#ifndef _SRC_ILI9341_DRIVER_HH_
#define _SRC_ILI9341_DRIVER_HH_

// including a source file is ugly, but those guys write anything into
// one big file.
#include <ILI9341_t3.h>
#include <SPI.h>


class ILI9341Driver
  : protected ILI9341_t3
{
  using BaseType = ILI9341_t3;
// Teensy 3.1 can only generate 30 MHz SPI when running at 120 MHz (overclock)
// At all other speeds, SPI.beginTransaction() will use the fastest available clock
//#define SPICLOCK 30000000
  static constexpr uint32_t SPICLOCK = 3000000;
 public:
  ILI9341Driver(uint8_t _CS, uint8_t _DC, uint8_t _RST = 255, uint8_t _LED = 255, uint8_t _MOSI=11, uint8_t _SCLK=13, uint8_t _MISO=12)
    : BaseType(_CS, _DC, _RST, _MOSI, _SCLK, _MISO)
    , _led(_LED)
  {
    if (_led != 255) {
      pinMode(_led, OUTPUT);
      setBacklight(100);
    }
  }

  using BaseType::begin;
  using BaseType::drawPixel;
  using BaseType::readPixel;
  using BaseType::readRect;
  using BaseType::fillRect;
  using BaseType::fillScreen;
  using BaseType::sleep;
  using BaseType::setRotation;

  void setBacklight(uint8_t percent) {
    analogWrite(_led, map(percent, 0, 100, 0, 255));
  }

  void startWrite(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1) {
    SPI.beginTransaction(SPISettings(SPICLOCK, MSBFIRST, SPI_MODE0));
    setAddr(x0, y0, x1, y1);
    writecommand_last(ILI9341_RAMWR); // write to RAM
    popLast = 0;
  }

  void pushColor(uint16_t color) {
    writeData16(color);
  }

  void endWrite() {
    if (popLast == 16) {
      writedata16_last(lastWrite16);
    }
    SPI.endTransaction();
  }

  void startRead(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1) {
    uint8_t dummy __attribute__((unused));
    SPI.beginTransaction(SPISettings(2000000, MSBFIRST, SPI_MODE0));
    setAddr(x0, y0, x1, y1);
    writecommand_cont(ILI9341_RAMRD); // read from RAM
    waitTransmitComplete();
    KINETISK_SPI0.PUSHR = 0 | (pcs_data << 16) | SPI_PUSHR_CTAS(0)| SPI_PUSHR_CONT | SPI_PUSHR_EOQ;
    while ((KINETISK_SPI0.SR & SPI_SR_EOQF) == 0) ;
    KINETISK_SPI0.SR = SPI_SR_EOQF;  // make sure it is clear
    while ((KINETISK_SPI0.SR & 0xf0)) {
      dummy = KINETISK_SPI0.POPR;	// Read a DUMMY byte but only once
    }
  }

  uint16_t readColor() {
    // like waitFifoNotFull but does not pop our return queue
    while ((KINETISK_SPI0.SR & (15 << 12)) > (3 << 12)) ;

    KINETISK_SPI0.PUSHR = 0 | (pcs_data << 16) | SPI_PUSHR_CTAS(0)| SPI_PUSHR_CONT;
    KINETISK_SPI0.PUSHR = 0 | (pcs_data << 16) | SPI_PUSHR_CTAS(0)| SPI_PUSHR_CONT;
    KINETISK_SPI0.PUSHR = 0 | (pcs_data << 16) | SPI_PUSHR_CTAS(0)| SPI_PUSHR_CONT;

    while ((KINETISK_SPI0.SR & 0xf0) < 0x30); // do we have at least 3 bytes in queue if so extract...
    uint8_t r = KINETISK_SPI0.POPR;		// Read a RED byte of GRAM
    uint8_t g = KINETISK_SPI0.POPR;		// Read a GREEN byte of GRAM
    uint8_t b = KINETISK_SPI0.POPR;		// Read a BLUE byte of GRAM

    return color565(r,g,b);
  }

  void endRead() {
    // like waitFifoNotFull but does not pop our return queue
    while ((KINETISK_SPI0.SR & (15 << 12)) > (3 << 12)) ;
    KINETISK_SPI0.PUSHR = 0 | (pcs_data << 16) | SPI_PUSHR_CTAS(0)| SPI_PUSHR_EOQ;
    while ((KINETISK_SPI0.SR & SPI_SR_EOQF) == 0) ;
    KINETISK_SPI0.SR = SPI_SR_EOQF;  // make sure it is clear
    uint8_t dummy __attribute__((unused));
    dummy = KINETISK_SPI0.POPR;
    while ((KINETISK_SPI0.SR & (15 << 12)) > (3 << 12)) ;
    SPI.endTransaction();
  }

  // like BaseType::writeRect(), but with the fancy uGfx offset support.
  void blitRect(int16_t x, int16_t y, int16_t w, int16_t h, const uint16_t *pcolors, uint16_t bufWidth) {
    SPI.beginTransaction(SPISettings(SPICLOCK, MSBFIRST, SPI_MODE0));
    setAddr(x, y, x+w-1, y+h-1);
    writecommand_cont(ILI9341_RAMWR);
    for (y = 0; y < h; ++y) {
      for(x = 0; x < w; ++x) {
        if (x == w-1 && y == h-1) {
          writedata16_last(*pcolors++);
        } else {
          writedata16_cont(*pcolors++);
        }
      }
      pcolors += bufWidth - w;
    }
    SPI.endTransaction();
  }
  void blitRect(int16_t x, int16_t y, int16_t w, int16_t h, const uint16_t *pcolors) {
    blitRect(x, y, w, h, pcolors, w);
  }
 protected:

  void writeData16(uint16_t d) {
    if (popLast == 16) {
      writedata16_cont(lastWrite16);
    }
    popLast = 16;
    lastWrite16 = d;
  }

  uint8_t _led;
 private:
  int popLast;
  uint8_t lastWrite8;
  uint16_t lastWrite16;
};

#endif // _SRC_ILI9341_DRIVER_HH_
