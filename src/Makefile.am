DEFS = @DEFS@

LOADER = $(top_builddir)/tools/teensy-loader

AM_CPPFLAGS = \
 -I$(srcdir)/ugfx-bindings/\
 -I$(top_srcdir)/externals/\
 -I$(top_srcdir)/externals/teensy-cores/teensy3\
 -I$(top_srcdir)/externals/i2c_t3\
 -I$(top_srcdir)/externals/GUIslice/src\
 -I$(top_srcdir)/externals/Adafruit-GFX\
 -I$(top_srcdir)/externals/ILI9341_t3\
 -I$(top_srcdir)/externals/SPI\
 -I$(top_srcdir)/externals/SD\
 -I$(top_srcdir)/externals/SD/utility\
 -I$(top_srcdir)/externals/TouchScreen\
 -I$(top_srcdir)\
 -DARDUINO=10805\
 -DTEENSYDUINO

noinst_PROGRAMS = teensy32-touchscreen-gui.elf
noinst_DATA = $(noinst_PROGRAMS:.elf=.hex)
noinst_LIBRARIES = \
 libteensy3.a\
 libi2c-t3.a\
 libSPI.a\
 libSD.a\
 libTouchScreen.a\
 libILI9341_t3.a

#libugfx.a

AM_CXXFLAGS = @MCU_FLAGS@ @MCU_SPECS@
AM_CFLAGS = @MCU_FLAGS@ @MCU_SPECS@
AM_LDFLAGS = @MCU_LDFLAGS@ -T"$(MCU_LDSCRIPT)"

teensy32_touchscreen_gui_elf_SOURCES =\
 main.cc
teensy32_touchscreen_gui_elf_LDADD =\
 libILI9341_t3.a\
 libSPI.a\
 libSD.a\
 libTouchScreen.a\
 libi2c-t3.a\
 libteensy3.a
# libugfx.a

include ugfx-bindings/ugfx.automake
$(srcdir)/ugfx/ugfx.automake: $(abs_top_srcdir)/ugfx.mk
	make -f $< -C $(top_srcdir) > $@.$$$$;\
  cmp -s $@.$$$$ $@ || mv $@.$$$$ $@

libugfx_a_SOURCES = $(UGFXSRC)\
 ugfx-bindings/gfxconf.h\
 ugfx-bindings/board_ILI9341.cpp\
 ugfx-bindings/board_ILI9341.h
libugfx_a_CPPFLAGS = $(UGFXDEFS) $(AM_CPPFLAGS)

libi2c_t3_a_SOURCES =\
 $(top_srcdir)/externals/i2c_t3/i2c_t3.cpp\
 $(top_srcdir)/externals/i2c_t3/i2c_t3.h

libteensy3_a_SOURCES =\
 $(top_srcdir)/externals/teensy-cores/teensy3/analog.c\
 $(top_srcdir)/externals/teensy-cores/teensy3/eeprom.c\
 $(top_srcdir)/externals/teensy-cores/teensy3/keylayouts.c\
 $(top_srcdir)/externals/teensy-cores/teensy3/math_helper.c\
 $(top_srcdir)/externals/teensy-cores/teensy3/mk20dx128.c\
 $(top_srcdir)/externals/teensy-cores/teensy3/nonstd.c\
 $(top_srcdir)/externals/teensy-cores/teensy3/pins_teensy.c\
 $(top_srcdir)/externals/teensy-cores/teensy3/serial1.c\
 $(top_srcdir)/externals/teensy-cores/teensy3/serial2.c\
 $(top_srcdir)/externals/teensy-cores/teensy3/serial3.c\
 $(top_srcdir)/externals/teensy-cores/teensy3/serial4.c\
 $(top_srcdir)/externals/teensy-cores/teensy3/serial5.c\
 $(top_srcdir)/externals/teensy-cores/teensy3/serial6.c\
 $(top_srcdir)/externals/teensy-cores/teensy3/serial6_lpuart.c\
 $(top_srcdir)/externals/teensy-cores/teensy3/ser_print.c\
 $(top_srcdir)/externals/teensy-cores/teensy3/touch.c\
 $(top_srcdir)/externals/teensy-cores/teensy3/usb_desc.c\
 $(top_srcdir)/externals/teensy-cores/teensy3/usb_dev.c\
 $(top_srcdir)/externals/teensy-cores/teensy3/usb_joystick.c\
 $(top_srcdir)/externals/teensy-cores/teensy3/usb_keyboard.c\
 $(top_srcdir)/externals/teensy-cores/teensy3/usb_mem.c\
 $(top_srcdir)/externals/teensy-cores/teensy3/usb_midi.c\
 $(top_srcdir)/externals/teensy-cores/teensy3/usb_mouse.c\
 $(top_srcdir)/externals/teensy-cores/teensy3/usb_mtp.c\
 $(top_srcdir)/externals/teensy-cores/teensy3/usb_rawhid.c\
 $(top_srcdir)/externals/teensy-cores/teensy3/usb_seremu.c\
 $(top_srcdir)/externals/teensy-cores/teensy3/usb_serial.c\
 $(top_srcdir)/externals/teensy-cores/teensy3/usb_touch.c\
 $(top_srcdir)/externals/teensy-cores/teensy3/AudioStream.cpp\
 $(top_srcdir)/externals/teensy-cores/teensy3/avr_emulation.cpp\
 $(top_srcdir)/externals/teensy-cores/teensy3/DMAChannel.cpp\
 $(top_srcdir)/externals/teensy-cores/teensy3/EventResponder.cpp\
 $(top_srcdir)/externals/teensy-cores/teensy3/HardwareSerial1.cpp\
 $(top_srcdir)/externals/teensy-cores/teensy3/HardwareSerial2.cpp\
 $(top_srcdir)/externals/teensy-cores/teensy3/HardwareSerial3.cpp\
 $(top_srcdir)/externals/teensy-cores/teensy3/HardwareSerial4.cpp\
 $(top_srcdir)/externals/teensy-cores/teensy3/HardwareSerial5.cpp\
 $(top_srcdir)/externals/teensy-cores/teensy3/HardwareSerial6.cpp\
 $(top_srcdir)/externals/teensy-cores/teensy3/IntervalTimer.cpp\
 $(top_srcdir)/externals/teensy-cores/teensy3/IPAddress.cpp\
 $(top_srcdir)/externals/teensy-cores/teensy3/new.cpp\
 $(top_srcdir)/externals/teensy-cores/teensy3/Print.cpp\
 $(top_srcdir)/externals/teensy-cores/teensy3/Stream.cpp\
 $(top_srcdir)/externals/teensy-cores/teensy3/Tone.cpp\
 $(top_srcdir)/externals/teensy-cores/teensy3/usb_audio.cpp\
 $(top_srcdir)/externals/teensy-cores/teensy3/usb_flightsim.cpp\
 $(top_srcdir)/externals/teensy-cores/teensy3/usb_inst.cpp\
 $(top_srcdir)/externals/teensy-cores/teensy3/WMath.cpp\
 $(top_srcdir)/externals/teensy-cores/teensy3/WString.cpp\
 $(top_srcdir)/externals/teensy-cores/teensy3/yield.cpp

libILI9341_t3_a_SOURCES =\
 $(top_srcdir)/externals/ILI9341_t3/font_ArialBold.c\
 $(top_srcdir)/externals/ILI9341_t3/font_ArialBold.h\
 $(top_srcdir)/externals/ILI9341_t3/font_Arial.c\
 $(top_srcdir)/externals/ILI9341_t3/font_Arial.h\
 $(top_srcdir)/externals/ILI9341_t3/glcdfont.c\
 $(top_srcdir)/externals/ILI9341_t3/ILI9341_t3.cpp\
 $(top_srcdir)/externals/ILI9341_t3/ILI9341_t3.h

libSPI_a_SOURCES =\
 $(top_srcdir)/externals/SPI/SPI.cpp\
 $(top_srcdir)/externals/SPI/SPI.h

libSD_a_SOURCES =\
 $(top_srcdir)/externals/SD/cache_t3.cpp\
 $(top_srcdir)/externals/SD/card_t3.cpp\
 $(top_srcdir)/externals/SD/dir_t3.cpp\
 $(top_srcdir)/externals/SD/examples\
 $(top_srcdir)/externals/SD/fat_t3.cpp\
 $(top_srcdir)/externals/SD/File.cpp\
 $(top_srcdir)/externals/SD/file_t3.cpp\
 $(top_srcdir)/externals/SD/init_t3.cpp\
 $(top_srcdir)/externals/SD/SD.cpp\
 $(top_srcdir)/externals/SD/SD.h\
 $(top_srcdir)/externals/SD/SD_t3.h\
 $(top_srcdir)/externals/SD/utility/FatStructs.h\
 $(top_srcdir)/externals/SD/utility/ioreg.h\
 $(top_srcdir)/externals/SD/utility/NXP_SDHC.cpp\
 $(top_srcdir)/externals/SD/utility/NXP_SDHC.h\
 $(top_srcdir)/externals/SD/utility/Sd2Card.cpp\
 $(top_srcdir)/externals/SD/utility/Sd2Card.h\
 $(top_srcdir)/externals/SD/utility/Sd2PinMap.h\
 $(top_srcdir)/externals/SD/utility/SdFat.h\
 $(top_srcdir)/externals/SD/utility/SdFatmainpage.h\
 $(top_srcdir)/externals/SD/utility/SdFatUtil.h\
 $(top_srcdir)/externals/SD/utility/SdFile.cpp\
 $(top_srcdir)/externals/SD/utility/SdInfo.h\
 $(top_srcdir)/externals/SD/utility/SdVolume.cpp

libTouchScreen_a_SOURCES =\
 $(top_srcdir)/externals/TouchScreen/TouchScreen.cpp\
 $(top_srcdir)/externals/TouchScreen/TouchScreen.h

SUFFIXES = .hex .lst

clean-local:
	rm -f *.hex *.lst

reboot: $(LOADER)
	$(LOADER) -v -s -b --mcu=$(MCU_TAG)

upload: $(LOADER) $(noinst_DATA)
	$(LOADER) -v -s -w --mcu=$(MCU_TAG) $(noinst_DATA)

%.hex: %.elf
	$(OBJCOPY) -O ihex $< $@

%.lst: %.elf
	$(OBJDUMP) --disassemble -S $< > $@
