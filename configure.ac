#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ(2.61)
AC_INIT(teensy-3-6, 0.1, himself@claus-justus-heine.de)
AC_CONFIG_SRCDIR([README])
AC_CONFIG_HEADER([config.h])
AM_INIT_AUTOMAKE([tar-ustar gnu 1.11 check-news dist-bzip2 dist-xz])
AM_MAINTAINER_MODE
AC_CONFIG_MACRO_DIR([m4])

AH_TOP([
/**@file
 * Autoconf preprocessor macros.
 */
/**@addtogroup AUTOCONF
 * @{
 */
])
AH_BOTTOM([/*@} AUTOCONF Doxygen group. */])

AC_ARG_WITH([mcu],
  AC_HELP_STRING([--with-mcu=TARGET],
    [Use TARGET as destination ARM MCU. Default is ``mk66fx1m0'']),
  [case "$withval" in
	yes|no)
	  AC_MSG_ERROR([Need to specify the target MCU])
	  ;;
   	*)
	  MCU="$withval"
	  ;;
  esac],
  [MCU=mk66fx1m0])
AC_SUBST([MCU])

LDSCRIPT=${MCU}.ld
AC_SUBST([LDSCRIPT])

ARCHFLAGS="-specs=nano.specs -specs=nosys.specs"

if test "${CFLAGS+set}" != set; then
  CFLAGS="-Os -ggdb3 -Wall"
fi
CFLAGS="${ARCHFLAGS} ${CFLAGS}"

if test "${CXXFLAGS+set}" != set; then
  CXXFLAGS="-std=c++17 -Os -ggdb3 -Wall -Wno-unused-local-typedefs"
fi
CXXFLAGS="${ARCHFLAGS} ${CXXFLAGS}"

LDFLAGS="${ARCHFLAGS} -Wl,--gc-sections,--relax -T${top_srcdir}/mk66fx1m0.ld"

ac_tool_prefix=arm-none-eabi-
host_alias=arm
cross_compiling=yes

AC_CANONICAL_BUILD
AC_CANONICAL_HOST
LT_INIT([disable-shared])

case "$MCU" in
    mk66fx1m0)
    CC_MCU_DEFS=
    LD_MCU_DEFS=
    ;;
    *)
    AC_MSG_ERROR([Unsupported MCU])
    ;;
esac
AC_SUBST([CC_MCU_DEFS])
AC_SUBST([LD_MCU_DEFS])

AM_CONDITIONAL([MK66FX1M0VMD18],[test $MCU = mk66fx1m0])

# Checks for programs.
AC_PROG_CC
AC_PROG_CXX
AM_PROG_AS
# AC_PROG_RANLIB already there by virtue of LT_INIT
AM_PROG_CC_C_O
AC_CHECK_TOOL([SIZE], [size])
AC_CHECK_TOOL([OBJDUMP], [objdump])
AC_CHECK_TOOL([OBJCOPY], [objcopy])

# Checks for libraries.

# Checks for header files.

AC_HEADER_STDBOOL

AC_CONFIG_FILES([Makefile])

AC_OUTPUT
