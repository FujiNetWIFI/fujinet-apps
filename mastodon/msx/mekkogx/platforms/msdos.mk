PRODUCT_BASE = $(PRODUCT_BASE)
EXECUTABLE = $(R2R_PD)/$(PRODUCT_BASE).exe
DISK = $(R2R_PD)/$(PRODUCT_BASE).img
LIBRARY = $(R2R_PD)/lib$(PRODUCT_BASE).$(PLATFORM).a
DISK_TOOL_CREATE = dd
DISK_TOOL_FORMAT = mformat
DISK_TOOL_COPY = mcopy
DISK_SIZE_KB = 360

MWD := $(realpath $(dir $(lastword $(MAKEFILE_LIST)))..)
include $(MWD)/common.mk
include $(MWD)/toolchains/ow2.mk

CFLAGS += -D__MSDOS__

r2r:: $(BUILD_DISK) $(BUILD_EXEC) $(BUILD_LIB) $(R2R_EXTRA_DEPS)
	make -f $(PLATFORM_MK) $(PLATFORM)/r2r-post

$(BUILD_DISK): $(BUILD_EXEC) $(DISK_EXTRA_DEPS) $(DISK_EXTRA_FILES) | $(R2R_PD)
	$(RM) $@
	$(RM) -rf $(CACHE_PLATFORM)/disk
	$(MKDIR_P) $(CACHE_PLATFORM)/disk
	cp $< $(CACHE_PLATFORM)/disk
	$(call require,$(DISK_TOOL_FORMAT),)
	$(call require,$(DISK_TOOL_COPY),)
	$(DISK_TOOL_CREATE) if=/dev/zero of=$@ bs=1024 count=$(DISK_SIZE_KB)
	$(DISK_TOOL_FORMAT) -i $@ -f $(DISK_SIZE_KB)
	$(DISK_TOOL_COPY) -i $@ $(CACHE_PLATFORM)/disk/* "::/"
	$(foreach f,$(DISK_EXTRA_FILES),$(call copy-to-disk,,$(f),,$@);)
	@make -f $(PLATFORM_MK) $(PLATFORM)/disk-post

# Arguments:
# $1 == DISK_TOOL_COPY flags
# $2 == source file
# $3 == destination name
# $4 == disk image
define copy-to-disk
    $(DISK_TOOL_COPY) -i $4 $1 $2 "::/$3"
endef
