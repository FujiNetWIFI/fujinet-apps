EXECUTABLE = $(R2R_PD)/$(PRODUCT_BASE).ptp
LIBRARY = $(R2R_PD)/$(PRODUCT_BASE).$(PLATFORM).lib
AS_DEFAULT = zcc

MWD := $(realpath $(dir $(lastword $(MAKEFILE_LIST)))..)
include $(MWD)/common.mk
include $(MWD)/toolchains/z88dk.mk

$(PLATFORM_UC)_FLAGS = +$(PLATFORM)
CFLAGS += $($(PLATFORM_UC)_FLAGS)
ASFLAGS += $($(PLATFORM_UC)_FLAGS) -c
LDFLAGS += $($(PLATFORM_UC)_FLAGS)

r2r:: $(BUILD_EXEC) $(BUILD_LIB) $(R2R_EXTRA_DEPS)
	make -f $(PLATFORM_MK) $(PLATFORM)/r2r-post
