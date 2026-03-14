EXECUTABLE = $(R2R_PD)/$(PRODUCT_BASE).rom
LIBRARY = $(R2R_PD)/$(PRODUCT_BASE).$(PLATFORM).lib

MWD := $(realpath $(dir $(lastword $(MAKEFILE_LIST)))..)
include $(MWD)/common.mk
include $(MWD)/toolchains/z88dk.mk

MSXROM_FLAGS = +msx -subtype=rom
CFLAGS += $(MSXROM_FLAGS)
LDFLAGS += $(MSXROM_FLAGS)

r2r:: $(EXECUTABLE) $(R2R_EXTRA_DEPS)
	make -f $(PLATFORM_MK) $(PLATFORM)/r2r-post
