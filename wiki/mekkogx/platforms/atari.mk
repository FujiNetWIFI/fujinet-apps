EXECUTABLE = $(R2R_PD)/$(PRODUCT_BASE).com
DISK = $(R2R_PD)/$(PRODUCT_BASE).atr
LIBRARY = $(R2R_PD)/$(PRODUCT_BASE).$(PLATFORM).lib
DISK_TOOL = dir2atr
DISK_TOOL_INFO = https://www.horus.com/~hias/atari/\#atarisio (Linux) or https://github.com/EricCarrGH/atari-dist/releases/ (Mac)
DISK_TOOL_COPY = atr
DISK_TOOL_COPY_INFO = https://github.com/jhallen/atari-tools

MWD := $(realpath $(dir $(lastword $(MAKEFILE_LIST)))..)
include $(MWD)/common.mk
include $(MWD)/toolchains/cc65.mk

r2r:: $(BUILD_DISK) $(BUILD_LIB) $(R2R_EXTRA_DEPS)
	make -f $(PLATFORM_MK) $(PLATFORM)/r2r-post

PICOBOOT_BIN = picoboot.bin
ATRBOOT := $(CACHE_PLATFORM)/$(PICOBOOT_BIN)
DISK_TOOL_FLAGS_ATARI ?= -S -m -B $(ATRBOOT)

$(BUILD_DISK): $(BUILD_EXEC) $(ATRBOOT) $(DISK_EXTRA_DEPS) $(DISK_EXTRA_FILES) | $(R2R_PD)
	$(RM) $@
	$(RM) -rf $(CACHE_PLATFORM)/disk
	$(MKDIR_P) $(CACHE_PLATFORM)/disk
	cp $< $(CACHE_PLATFORM)/disk/$(EXEC_NAME_ATARI)
	$(call require,$(DISK_TOOL),$(DISK_TOOL_INFO))
	$(call require,$(DISK_TOOL_COPY),$(DISK_TOOL_COPY_INFO))
	$(foreach f,$(DISK_EXTRA_FILES),cp $(f) $(CACHE_PLATFORM)/disk ; )
	$(DISK_TOOL) $(DISK_TOOL_FLAGS_ATARI) $@ $(CACHE_PLATFORM)/disk
	make -f $(PLATFORM_MK) $(PLATFORM)/disk-post

PICOBOOT_DOWNLOAD_URL = https://github.com/FujiNetWIFI/assets/releases/download/picobin
$(ATRBOOT): | $(CACHE_PLATFORM)
	curl -L -o $@ $(PICOBOOT_DOWNLOAD_URL)/$(PICOBOOT_BIN)

# Arguments:
# $1 == atr flags
# $2 == source file
# $3 == destination name
# $4 == disk image
define copy-to-disk
    atr $4 put $1 $2 $3
endef
