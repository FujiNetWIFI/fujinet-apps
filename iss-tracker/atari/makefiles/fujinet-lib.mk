FUJINET_LIB_VERSION := 4.3.1

FUJINET_LIB = _libs
FUJINET_LIB_VERSION_DIR = $(FUJINET_LIB)/$(FUJINET_LIB_VERSION)-$(CURRENT_TARGET)
FUJINET_LIB_PATH = $(FUJINET_LIB_VERSION_DIR)/fujinet-$(CURRENT_TARGET)-$(FUJINET_LIB_VERSION).lib

FUJINET_LIB_DOWNLOAD_URL = https://github.com/FujiNetWIFI/fujinet-lib/releases/download/v$(FUJINET_LIB_VERSION)/fujinet-lib-$(CURRENT_TARGET)-$(FUJINET_LIB_VERSION).zip
FUJINET_LIB_DOWNLOAD_FILE = $(FUJINET_LIB)/fujinet-lib-$(CURRENT_TARGET)-$(FUJINET_LIB_VERSION).zip

.get_fujinet_lib:
	@if [ ! -f "$(FUJINET_LIB_DOWNLOAD_FILE)" ]; then \
		if [ -d "$(FUJINET_LIB_VERSION_DIR)" ]; then \
		  echo "A directory already exists with version $(FUJINET_LIB_VERSION) - please remove it first"; \
			exit 1; \
		fi; \
		HTTPSTATUS=$$(curl -Is $(FUJINET_LIB_DOWNLOAD_URL) | head -n 1 | awk '{print $$2}'); \
		if [ "$${HTTPSTATUS}" == "404" ]; then \
			echo "ERROR: Unable to find file $(FUJINET_LIB_DOWNLOAD_URL)"; \
			exit 1; \
		fi; \
		echo "Downloading fujinet-lib for $(CURRENT_TARGET) version $(FUJINET_LIB_VERSION) from $(FUJINET_LIB_DOWNLOAD_URL)"; \
		mkdir -p $(FUJINET_LIB); \
		curl -sL $(FUJINET_LIB_DOWNLOAD_URL) -o $(FUJINET_LIB_DOWNLOAD_FILE); \
		echo "Unzipping to $(FUJINET_LIB)"; \
		unzip -o $(FUJINET_LIB_DOWNLOAD_FILE) -d $(FUJINET_LIB_VERSION_DIR); \
		echo "Unzip complete."; \
	fi

CFLAGS += --include-dir $(FUJINET_LIB_VERSION_DIR)
ASFLAGS += --asm-include-dir $(FUJINET_LIB_VERSION_DIR)
LIBS += $(FUJINET_LIB_PATH)
ALL_TASKS += .get_fujinet_lib
