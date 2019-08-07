GFXLIB = externals/ugfx

# OPT_OS = raw32
# GFXDRIVERS = gdisp/ILI9341
GFXDRIVERS = ginput/touch/MCU

include $(GFXLIB)/gfx.mk

all:
	@echo UGFXSRC = '$(subst externals,$$(top_srcdir)/externals, $(GFXSRC))'
	@echo UGFXDEFS = '$(patsubst %,-D%,$(GFXDEFS))'
