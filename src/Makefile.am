DEFS = @DEFS@

LOADER = $(top_builddir)/tools/teensy-loader

AM_CPPFLAGS = \
 -I$(top_srcdir)/externals/teensy-cores/teensy3\
 -I$(top_srcdir)/externals/i2c_t3\
 -I$(top_srcdir)/externals/GUIslice/src\
 -I$(top_srcdir)/externals/Adafruit-GFX\
 -I$(top_srcdir)/externals/ILI9341_t3\
 -I$(top_srcdir)/externals/SPI\
 -I$(top_srcdir)/externals/SD\
 -I$(top_srcdir)\
 -DARDUINO=10805

noinst_PROGRAMS = teensy32-touchscreen-gui.elf
noinst_DATA = $(noinst_PROGRAMS:.elf=.hex)
noinst_LIBRARIES = \
 libteensy3.a\
 libi2c-t3.a\
 libGUIslice.a\
 libILI9341_t3.a

AM_CXXFLAGS = @MCU_FLAGS@ @MCU_SPECS@
AM_CFLAGS = @MCU_FLAGS@ @MCU_SPECS@
AM_LDFLAGS = @MCU_LDFLAGS@ -T"$(MCU_LDSCRIPT)"

teensy32_touchscreen_gui_elf_SOURCES =\
 main.cc

teensy32_touchscreen_gui_elf_LDADD =\
 libGUIslice.a\
 libILI9341_t3.a\
 libSPI.a\
 libSD.a\
 libi2c-t3.a\
 libteensy3.a

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

libGUIslice_a_SOURCES =\
$(top_srcdir)/externals/GUIslice/src/GUIslice.c\
$(top_srcdir)/externals/GUIslice/src/GUIslice_config_ard.h\
$(top_srcdir)/externals/GUIslice/src/GUIslice_config.h\
$(top_srcdir)/externals/GUIslice/src/GUIslice_config_linux.h\
$(top_srcdir)/externals/GUIslice/src/GUIslice_drv_adagfx.cpp\
$(top_srcdir)/externals/GUIslice/src/GUIslice_drv_adagfx.h\
$(top_srcdir)/externals/GUIslice/src/GUIslice_drv.h\
$(top_srcdir)/externals/GUIslice/src/GUIslice_drv_m5stack.cpp\
$(top_srcdir)/externals/GUIslice/src/GUIslice_drv_m5stack.h\
$(top_srcdir)/externals/GUIslice/src/GUIslice_drv_sdl.c\
$(top_srcdir)/externals/GUIslice/src/GUIslice_drv_sdl.h\
$(top_srcdir)/externals/GUIslice/src/GUIslice_drv_tft_espi.cpp\
$(top_srcdir)/externals/GUIslice/src/GUIslice_drv_tft_espi.h\
$(top_srcdir)/externals/GUIslice/src/GUIslice_drv_utft.cpp\
$(top_srcdir)/externals/GUIslice/src/GUIslice_drv_utft.h\
$(top_srcdir)/externals/GUIslice/src/GUIslice_ex.h\
$(top_srcdir)/externals/GUIslice/src/GUIslice.h\
$(top_srcdir)/externals/GUIslice/src/GUIslice_th.cpp\
$(top_srcdir)/externals/GUIslice/src/GUIslice_th.h\
$(top_srcdir)/externals/GUIslice/src/GUIslice_th_XPT2046.h\
$(top_srcdir)/externals/GUIslice/src/GUIslice_version.h\
$(top_srcdir)/externals/GUIslice/src/GUIslice/elem/XCheckbox.c\
$(top_srcdir)/externals/GUIslice/src/GUIslice/elem/XCheckbox.h\
$(top_srcdir)/externals/GUIslice/src/GUIslice/elem/XGauge.c\
$(top_srcdir)/externals/GUIslice/src/GUIslice/elem/XGauge.h\
$(top_srcdir)/externals/GUIslice/src/GUIslice/elem/XGraph.c\
$(top_srcdir)/externals/GUIslice/src/GUIslice/elem/XGraph.h\
$(top_srcdir)/externals/GUIslice/src/GUIslice/elem/XKeyPad_Alpha.c\
$(top_srcdir)/externals/GUIslice/src/GUIslice/elem/XKeyPad_Alpha.h\
$(top_srcdir)/externals/GUIslice/src/GUIslice/elem/XKeyPad.c\
$(top_srcdir)/externals/GUIslice/src/GUIslice/elem/XKeyPad.h\
$(top_srcdir)/externals/GUIslice/src/GUIslice/elem/XKeyPad_Num.c\
$(top_srcdir)/externals/GUIslice/src/GUIslice/elem/XKeyPad_Num.h\
$(top_srcdir)/externals/GUIslice/src/GUIslice/elem/XListbox.c\
$(top_srcdir)/externals/GUIslice/src/GUIslice/elem/XListbox.h\
$(top_srcdir)/externals/GUIslice/src/GUIslice/elem/XRingGauge.c\
$(top_srcdir)/externals/GUIslice/src/GUIslice/elem/XRingGauge.h\
$(top_srcdir)/externals/GUIslice/src/GUIslice/elem/XSelNum.c\
$(top_srcdir)/externals/GUIslice/src/GUIslice/elem/XSelNum.h\
$(top_srcdir)/externals/GUIslice/src/GUIslice/elem/XSlider.c\
$(top_srcdir)/externals/GUIslice/src/GUIslice/elem/XSlider.h\
$(top_srcdir)/externals/GUIslice/src/GUIslice/elem/XSpinner.c\
$(top_srcdir)/externals/GUIslice/src/GUIslice/elem/XSpinner.h\
$(top_srcdir)/externals/GUIslice/src/GUIslice/elem/XTemplate.c\
$(top_srcdir)/externals/GUIslice/src/GUIslice/elem/XTemplate.h\
$(top_srcdir)/externals/GUIslice/src/GUIslice/elem/XTextbox.c\
$(top_srcdir)/externals/GUIslice/src/GUIslice/elem/XTextbox.h

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
 $(top_srcdir)/externals/SPI/cache_t3.cpp\
 $(top_srcdir)/externals/SPI/card_t3.cpp\
 $(top_srcdir)/externals/SPI/dir_t3.cpp\
 $(top_srcdir)/externals/SPI/examples\
 $(top_srcdir)/externals/SPI/fat_t3.cpp\
 $(top_srcdir)/externals/SPI/File.cpp\
 $(top_srcdir)/externals/SPI/file_t3.cpp\
 $(top_srcdir)/externals/SPI/init_t3.cpp\
 $(top_srcdir)/externals/SPI/SD.cpp\
 $(top_srcdir)/externals/SPI/SD.h\
 $(top_srcdir)/externals/SPI/SD_t3.h\
 $(top_srcdir)/externals/SPI/utility/FatStructs.h\
 $(top_srcdir)/externals/SPI/utility/ioreg.h\
 $(top_srcdir)/externals/SPI/utility/NXP_SDHC.cpp\
 $(top_srcdir)/externals/SPI/utility/NXP_SDHC.h\
 $(top_srcdir)/externals/SPI/utility/Sd2Card.cpp\
 $(top_srcdir)/externals/SPI/utility/Sd2Card.h\
 $(top_srcdir)/externals/SPI/utility/Sd2PinMap.h\
 $(top_srcdir)/externals/SPI/utility/SdFat.h\
 $(top_srcdir)/externals/SPI/utility/SdFatmainpage.h\
 $(top_srcdir)/externals/SPI/utility/SdFatUtil.h\
 $(top_srcdir)/externals/SPI/utility/SdFile.cpp\
 $(top_srcdir)/externals/SPI/utility/SdInfo.h\
 $(top_srcdir)/externals/SPI/utility/SdVolume.cpp

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
