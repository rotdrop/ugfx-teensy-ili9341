GFXLIB = externals/ugfx

OPT_OS = raw32

include $(GFXLIB)/gfx.mk

all:
	@echo UGFXSRC = '$(subst externals,$$(top_srcdir)/externals, $(GFXSRC))'
	@echo UGFXDEFS = '$(patsubst %,-D%,$(GFXDEFS))'
