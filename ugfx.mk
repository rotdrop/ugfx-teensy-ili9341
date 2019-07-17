GFXLIB = externals/ugfx

OPT_OS = raw32

include $(GFXLIB)/gfx.mk

all:
	@echo GFXSRC = '$(subst externals,$$(top_srcdir)/externals, $(GFXSRC))'
	@echo GFXDEFS = $(GFXDEFS)
