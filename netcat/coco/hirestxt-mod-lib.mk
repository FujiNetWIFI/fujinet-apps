HIRESTXT_LIB_VERSION := 0.5.0.1
CURRENT_TARGET = coco

HIRESTXT_LIB = _cache
HIRESTXT_LIB_VERSION_DIR = $(HIRESTXT_LIB)/hirestxt-$(HIRESTXT_LIB_VERSION)-$(CURRENT_TARGET)
HIRESTXT_LIB_PATH = $(HIRESTXT_LIB_VERSION_DIR)
HIRESTXT_LIB_DOWNLOAD_URL = https://github.com/RichStephens/hirestxt-mod/releases/download/$(HIRESTXT_LIB_VERSION)/hirestxt-mod-bin-$(HIRESTXT_LIB_VERSION).tar.gz
HIRESTXT_LIB_DOWNLOAD_FILE = $(HIRESTXT_LIB)/hirestxt-mod-bin-$(HIRESTXT_LIB_VERSION).tar.gz
HIRESTXT_LIB_BASENAME := $(notdir $(HIRESTXT_LIB_PATH))
HIRESTXT_LIB_SYMLINK  := libhirestxt.a

.get_hirestxt_lib:
	@if [ ! -f "$(HIRESTXT_LIB_DOWNLOAD_FILE)" ]; then \
		if [ -d "$(HIRESTXT_LIB_VERSION_DIR)" ]; then \
			echo "A directory already exists with version $(HIRESTXT_LIB_VERSION) - please remove it first"; \
			exit 1; \
		fi; \
		HTTPSTATUS=$$(curl -Is $(HIRESTXT_LIB_DOWNLOAD_URL) | head -n 1 | awk '{print $$2}'); \
		if [ "$${HTTPSTATUS}" == "404" ]; then \
			echo "ERROR: Unable to find file $(HIRESTXT_LIB_DOWNLOAD_URL)"; \
			exit 1; \
		fi; \
		echo "Downloading hirestxt-mod for $(CURRENT_TARGET) version $(HIRESTXT_LIB_VERSION) from $(HIRESTXT_LIB_DOWNLOAD_URL)"; \
		mkdir -p $(HIRESTXT_LIB); \
		mkdir -p $(HIRESTXT_LIB_VERSION_DIR); \
		curl -sL $(HIRESTXT_LIB_DOWNLOAD_URL) -o $(HIRESTXT_LIB_DOWNLOAD_FILE); \
		echo "Unzipping to $(HIRESTXT_LIB_VERSION_DIR)"; \
		tar -xf $(HIRESTXT_LIB_DOWNLOAD_FILE) -C $(HIRESTXT_LIB_VERSION_DIR); \
		echo "Unzip complete."; \
		( cd "$(HIRESTXT_LIB_VERSION_DIR)" && ln -sf "$(HIRESTXT_LIB_SYMLINK)" "$(HIRESTXT_LIB_BASENAME)" ); \
	fi; 

CFLAGS += -I$(HIRESTXT_LIB_VERSION_DIR)
ASFLAGS += --asm-include-dir $(HIRESTXT_LIB_VERSION_DIR)
LIBS +=  -L $(HIRESTXT_LIB_PATH) -lhirestxt
