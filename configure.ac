#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ(2.61)
AC_INIT(teensy-touchscreen-gui, 0.1, himself@claus-justus-heine.de)
AC_CONFIG_SRCDIR([src/main.cc])
AC_CONFIG_HEADER([config.h])
AM_INIT_AUTOMAKE([tar-ustar gnu 1.11 check-news dist-bzip2 dist-xz])
AM_MAINTAINER_MODE
AC_CONFIG_MACRO_DIR([m4])
_AC_SRCDIRS([.])

AH_TOP([
/**@file
 * Autoconf preprocessor macros.
 */
/**@addtogroup AUTOCONF
 * @{
 */
])
AH_BOTTOM([/*@} AUTOCONF Doxygen group. */])

AC_ARG_WITH([teensy-cores],
  AC_HELP_STRING([--with-teensy-cores=DIR],
    [Define to the base directory of the teensy-core project (default: ${ac_top_srcdir}/externals/teensy-cores/).]),
  [case "$withval" in
	yes|no)
	  AC_MSG_ERROR([We really need the location of the Teensy-core libraries, not ``yes'' or ``no'' or nothing.])
	  ;;
   	*)
	  TEENSY_CORES="$withval"
	  ;;
  esac],
  [TEENSY_CORES="\${top_srcdir}/externals/teensy-cores"])
AC_SUBST([TEENSY_CORES])
AC_DEFINE_UNQUOTED([TEENSY_CORES], $TEENSY_CORES, [Location of the Teensy cores library.])

AC_ARG_WITH([mcu],
  AC_HELP_STRING([--with-mcu=TARGET],
    [Use TARGET as destination ARM microcontroller. Default is ``teensy-3.2'']),
  [case "$withval" in
	yes|no)
	  AC_MSG_ERROR([Need to specify the target MCU])
	  ;;
   	*)
	  MCU="$withval"
	  ;;
  esac],
  [MCU=teensy-3.2])
AC_SUBST([MCU])
AC_DEFINE_UNQUOTED([MCU], "$MCU", [Define to the target MCU])

cross_compiling=yes

case "$MCU" in
    teensy*)
        ac_tool_prefix=arm-none-eabi-
        host_alias=arm
        MCU_FLAGS="-ffunction-sections -fdata-sections -DUSING_MAKEFILE -DUSB_SERIAL -DLAYOUT_US_ENGLISH"
        MCU_LDFLAGS="-Wl,--gc-sections,--relax,--defsym=__rtc_localtime=0"
        MCU_SPECS="-specs=\${top_srcdir}/specs/c-only-nano.specs"
        ;;
    *)
        AC_MSG_ERROR([Unsupported MCU])
        ;;
esac

case "$MCU" in
    teensy-3.2)
        MCU_LDSCRIPT="\${TEENSY_CORES}/teensy3/mk20dx256.ld"
        F_CPU=72000000
        MCU_FLAGS="-D__MK20DX256__ -march=armv7e-m -mthumb -mcpu=cortex-m4 -DF_CPU=${F_CPU} ${MCU_FLAGS}"
        MCU_TAG=TEENSY32
        ;;
    *)
        AC_MSG_ERROR([Unsupported MCU])
        ;;
esac

AC_SUBST(MCU_TAG)
AC_SUBST(MCU_FLAGS)
AC_SUBST(MCU_LDFLAGS)
AC_SUBST(MCU_SPECS)
AC_SUBST(MCU_LDSCRIPT)
AC_SUBST(F_CPU)
AC_DEFINE_UNQUOTED([F_CPU], $F_CPU, [Define to the CPU frequency of the uC])

AM_CONDITIONAL([TEENSY_3_6], [test $MCU = "teensy-3.6"])

if test "${CFLAGS+set}" != set; then
  CFLAGS="-Os -ggdb3 -Wall"
fi

if test "${CXXFLAGS+set}" != set; then
  CXXFLAGS="-std=gnu++17 -Os -ggdb3 -Wall -felide-constructors -fno-exceptions -fno-rtti"
fi

ac_save_LDFLAGS="$LDFLAGS"
LDFLAGS="-nostdlib $LDFLAGS"
LT_INIT([disable-shared])
LDFLAGS="$ac_save_LDFLAGS"

AC_CANONICAL_BUILD
AC_CANONICAL_HOST

# Checks for programs.
AC_PROG_CC
AC_PROG_CXX
AM_PROG_AS
# AC_PROG_RANLIB already there by virtue of LT_INIT
AM_PROG_CC_C_O
AC_CHECK_TOOL([SIZE], [size])
AC_CHECK_TOOL([OBJDUMP], [objdump])
AC_CHECK_TOOL([OBJCOPY], [objcopy])

m4_define(_LT_LANG_CXX_FOR_BUILD_CONFIG)
AX_PROG_CC_FOR_BUILD
AX_PROG_CXX_FOR_BUILD

ac_save_link=$ac_link
ac_save_LIBS=$LIBS
LIBS=$LIBS_FOR_BUILD
ac_link='$CC_FOR_BUILD -o conftest$BUILD_EXEEXT $CFLAGS_FOR_BUILD $CPPFLAGS_FOR_BUILD $LDFLAGS_FOR_BUILD conftest.$ac_ext $LIBS >&5'
AC_CHECK_LIB(usb, usb_open, [LIBS_FOR_BUILD="$LIBS_FOR_BUILD -lusb"])
ac_link=$ac_save_link
LIBS=$ac_save_LIBS
AC_SUBST([LIBS_FOR_BUILD])

AC_CONFIG_FILES([Makefile
                 tools/Makefile
                 src/Makefile])

AC_OUTPUT
