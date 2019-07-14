CC = $(CC_FOR_BUILD)
EXEEXT = $(BUILD_EXEEXT)
CFLAGS = $(CFLAGS_FOR_BUILD)
LIBS = $(LIBS_FOR_BUILD)

noinst_PROGRAMS = teensy-loader

teensy_loader_SOURCES = $(top_srcdir)/externals/teensy-loader/teensy_loader_cli.c
teensy_loader_CPPFLAGS = -DUSE_LIBUSB
