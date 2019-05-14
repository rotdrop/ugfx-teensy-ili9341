DEFS = @DEFS@

AM_CPPFLAGS = \
 -I$(top_srcdir)/external/teensy-cores/teensy3 \
 -I$(top_srcdir)

noinst_PROGRAMS = teensy36.elf
noinst_DATA = $(noinst_PROGRAMS:.elf=.hex)
noinst_LIBRARIES = libteensy3.a

AM_CXXFLAGS = @MCU_FLAGS@ @MCU_SPECS@
AM_CFLAGS = @MCU_FLAGS@ @MCU_SPECS@
AM_LDFLAGS = @MCU_LDFLAGS@ -T"$(MCU_LDSCRIPT)"

teensy36_elf_SOURCES = main.cc
teensy36_elf_LDADD = libteensy3.a

libteensy3_a_SOURCES =\
 $(top_srcdir)/external/teensy-cores/teensy3/analog.c\
 $(top_srcdir)/external/teensy-cores/teensy3/eeprom.c\
 $(top_srcdir)/external/teensy-cores/teensy3/keylayouts.c\
 $(top_srcdir)/external/teensy-cores/teensy3/math_helper.c\
 $(top_srcdir)/external/teensy-cores/teensy3/mk20dx128.c\
 $(top_srcdir)/external/teensy-cores/teensy3/nonstd.c\
 $(top_srcdir)/external/teensy-cores/teensy3/pins_teensy.c\
 $(top_srcdir)/external/teensy-cores/teensy3/serial1.c\
 $(top_srcdir)/external/teensy-cores/teensy3/serial2.c\
 $(top_srcdir)/external/teensy-cores/teensy3/serial3.c\
 $(top_srcdir)/external/teensy-cores/teensy3/serial4.c\
 $(top_srcdir)/external/teensy-cores/teensy3/serial5.c\
 $(top_srcdir)/external/teensy-cores/teensy3/serial6.c\
 $(top_srcdir)/external/teensy-cores/teensy3/serial6_lpuart.c\
 $(top_srcdir)/external/teensy-cores/teensy3/ser_print.c\
 $(top_srcdir)/external/teensy-cores/teensy3/touch.c\
 $(top_srcdir)/external/teensy-cores/teensy3/usb_desc.c\
 $(top_srcdir)/external/teensy-cores/teensy3/usb_dev.c\
 $(top_srcdir)/external/teensy-cores/teensy3/usb_joystick.c\
 $(top_srcdir)/external/teensy-cores/teensy3/usb_keyboard.c\
 $(top_srcdir)/external/teensy-cores/teensy3/usb_mem.c\
 $(top_srcdir)/external/teensy-cores/teensy3/usb_midi.c\
 $(top_srcdir)/external/teensy-cores/teensy3/usb_mouse.c\
 $(top_srcdir)/external/teensy-cores/teensy3/usb_mtp.c\
 $(top_srcdir)/external/teensy-cores/teensy3/usb_rawhid.c\
 $(top_srcdir)/external/teensy-cores/teensy3/usb_seremu.c\
 $(top_srcdir)/external/teensy-cores/teensy3/usb_serial.c\
 $(top_srcdir)/external/teensy-cores/teensy3/usb_touch.c\
 $(top_srcdir)/external/teensy-cores/teensy3/AudioStream.cpp\
 $(top_srcdir)/external/teensy-cores/teensy3/avr_emulation.cpp\
 $(top_srcdir)/external/teensy-cores/teensy3/DMAChannel.cpp\
 $(top_srcdir)/external/teensy-cores/teensy3/EventResponder.cpp\
 $(top_srcdir)/external/teensy-cores/teensy3/HardwareSerial1.cpp\
 $(top_srcdir)/external/teensy-cores/teensy3/HardwareSerial2.cpp\
 $(top_srcdir)/external/teensy-cores/teensy3/HardwareSerial3.cpp\
 $(top_srcdir)/external/teensy-cores/teensy3/HardwareSerial4.cpp\
 $(top_srcdir)/external/teensy-cores/teensy3/HardwareSerial5.cpp\
 $(top_srcdir)/external/teensy-cores/teensy3/HardwareSerial6.cpp\
 $(top_srcdir)/external/teensy-cores/teensy3/IntervalTimer.cpp\
 $(top_srcdir)/external/teensy-cores/teensy3/IPAddress.cpp\
 $(top_srcdir)/external/teensy-cores/teensy3/new.cpp\
 $(top_srcdir)/external/teensy-cores/teensy3/Print.cpp\
 $(top_srcdir)/external/teensy-cores/teensy3/Stream.cpp\
 $(top_srcdir)/external/teensy-cores/teensy3/Tone.cpp\
 $(top_srcdir)/external/teensy-cores/teensy3/usb_audio.cpp\
 $(top_srcdir)/external/teensy-cores/teensy3/usb_flightsim.cpp\
 $(top_srcdir)/external/teensy-cores/teensy3/usb_inst.cpp\
 $(top_srcdir)/external/teensy-cores/teensy3/WMath.cpp\
 $(top_srcdir)/external/teensy-cores/teensy3/WString.cpp\
 $(top_srcdir)/external/teensy-cores/teensy3/yield.cpp

SUFFIXES = .hex .lst

clean-local:
	rm -f *.hex *.lst

post_compile: $(noinst_DATA)
	true
#	@$(abspath $(TOOLSPATH))/teensy_post_compile -file="$(basename $<)" -path=$(CURDIR) -tools="$(abspath $(TOOLSPATH))"

reboot:
	true
#	@-$(abspath $(TOOLSPATH))/teensy_reboot

upload: post_compile reboot

%.hex: %.elf
	$(OBJCOPY) -O ihex -R .eeprom $< $@

%.lst: %.elf
	$(OBJDUMP) --disassemble -S $< > $@
