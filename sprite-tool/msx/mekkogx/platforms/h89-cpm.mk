EXECUTABLE = $(R2R_PD)/$(PRODUCT_BASE).com
LIBRARY = $(R2R_PD)/$(PRODUCT_BASE).$(PLATFORM).lib

MWD := $(realpath $(dir $(lastword $(MAKEFILE_LIST)))..)
include $(MWD)/common.mk
include $(MWD)/toolchains/z88dk.mk

MSX_FLAGS = +cpm -create-app
CFLAGS += $(MSX_FLAGS)
LDFLAGS += $(MSX_FLAGS)

r2r:: $(BUILD_EXEC) $(BUILD_LIB) $(R2R_EXTRA_DEPS)
	make -f $(PLATFORM_MK) $(PLATFORM)/r2r-post
