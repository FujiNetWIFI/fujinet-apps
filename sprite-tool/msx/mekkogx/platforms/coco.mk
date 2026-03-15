EXECUTABLE = $(R2R_PD)/$(PRODUCT_BASE).bin
DISK = $(R2R_PD)/$(PRODUCT_BASE).dsk
LIBRARY = $(R2R_PD)/lib$(PRODUCT_BASE).$(PLATFORM).a
DISK_TOOL = decb
DISK_TOOL_INFO = https://github.com/nitros9project/toolshed

MWD := $(realpath $(dir $(lastword $(MAKEFILE_LIST)))..)
include $(MWD)/common.mk
include $(MWD)/toolchains/cmoc.mk

r2r:: $(BUILD_DISK) $(BUILD_LIB) $(R2R_EXTRA_DEPS)
	@make -f $(PLATFORM_MK) $(PLATFORM)/r2r-post

$(BUILD_DISK): $(BUILD_EXEC) $(DISK_EXTRA_DEPS) $(DISK_EXTRA_FILES) | $(R2R_PD)
	$(RM) $@
	$(call require,$(DISK_TOOL),$(DISK_TOOL_INFO))
	$(DISK_TOOL) dskini $@
	$(call copy-to-disk,-b -2,$<,$(shell echo $(notdir $<) | tr '[:lower:]' '[:upper:]'),$@)
	$(foreach f,$(DISK_EXTRA_FILES),$(call copy-to-disk,-b,$(f),$(shell echo $(notdir $(f)) | tr '[:lower:]' '[:upper:]'),$@);)
	@make -f $(PLATFORM_MK) $(PLATFORM)/disk-post

# Arguments:
# $1 == DISK_TOOL flags
# $2 == source file
# $3 == destination name
# $4 == disk image
define copy-to-disk
    $(DISK_TOOL) copy $1 $2 $4,$3
endef
