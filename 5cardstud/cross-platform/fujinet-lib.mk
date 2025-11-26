# This makefile will download the specified fujinet lib for the current platform

FUJINET_LIB_VERSION := 4.8.3

# ///////////////////////////////////////////////////////////////////////////////
CURRENT_TARGET ?=$(TARGET)
FUJINET_LIB = ./_libs
FUJINET_LIB_VERSION_DIR = $(FUJINET_LIB)/$(FUJINET_LIB_VERSION)-$(CURRENT_TARGET)
FUJINET_LIB_NAME = fujinet-$(CURRENT_TARGET)-$(FUJINET_LIB_VERSION).lib
FUJINET_LIB_PATH = $(FUJINET_LIB_VERSION_DIR)/$(FUJINET_LIB_NAME)

FUJINET_LIB_DOWNLOAD_URL = https://github.com/FujiNetWIFI/fujinet-lib/releases/download/v$(FUJINET_LIB_VERSION)/fujinet-lib-$(CURRENT_TARGET)-$(FUJINET_LIB_VERSION).zip
FUJINET_LIB_DOWNLOAD_FILE = $(FUJINET_LIB)/fujinet-lib-$(CURRENT_TARGET)-$(FUJINET_LIB_VERSION).zip

ifneq ($(OS),Windows_NT) 
	CHECK_LIB_AVAIL = $(shell curl -Is $(FUJINET_LIB_DOWNLOAD_URL) | head -n 1)
	MKDIR = mkdir -p $1
else
	CHECK_LIB_AVAIL = $(shell curl -Is $(FUJINET_LIB_DOWNLOAD_URL) | find "HTTP/")
	MKDIR = mkdir $(subst /,\,$1)
endif

IS_LIB_NOT_FOUND = no #$(if $(findstring 404,$(CHECK_LIB_AVAIL)),yes,no)

.get_fujinet_lib:
ifeq ("$(wildcard $(FUJINET_LIB_DOWNLOAD_FILE))","")
	$(info Downloading fujinet-lib for $(CURRENT_TARGET) version $(FUJINET_LIB_VERSION) from $(FUJINET_LIB_DOWNLOAD_URL))

ifeq (yes,$(IS_LIB_NOT_FOUND))
	$(error $(CHECK_LIB_AVAIL)- Unable to find file $(FUJINET_LIB_DOWNLOAD_URL))
endif

	$(MKDIR) "$(FUJINET_LIB_VERSION_DIR)"
	curl -sL $(FUJINET_LIB_DOWNLOAD_URL) -o $(FUJINET_LIB_DOWNLOAD_FILE)
	tar -xf $(FUJINET_LIB_DOWNLOAD_FILE) -C $(FUJINET_LIB_VERSION_DIR)
endif

CFLAGS += --include-dir $(FUJINET_LIB_VERSION_DIR)
ASFLAGS += --asm-include-dir $(FUJINET_LIB_VERSION_DIR)
LIBS += $(FUJINET_LIB_PATH)
