# common.mk
# 

$(info >>>>Starting common.mk)
-include $(FUJINET_BUILD_TOOLS_DIR)/makefiles/fujinet-lib.mk

VERSION_FILE := src/version.txt
ifeq (,$(wildcard $(VERSION_FILE)))
	VERSION_FILE =
	ERSION_STRING =
else
	VERSION_STRING := $(file < $(VERSION_FILE))
endif

CFLAGS += -Osir
