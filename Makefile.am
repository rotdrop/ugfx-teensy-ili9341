AUTOMAKE_OPTIONS = subdir-objects
ACLOCAL_AMFLAGS = -I m4

SUBDIRS = src tools

DEFS = @DEFS@ @CC_MCU_DEFS@

AM_CPPFLAGS = \
 -I$(top_srcdir)/include \
 -I$(top_srcdir)

AM_CFLAGS = -std=c11
AM_CXXFLAGS = -std=c++17

