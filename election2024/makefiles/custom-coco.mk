###################################################################
# COCO
###################################################################
ifeq ($(DEBUG),true)
    $(info >Starting custom-coco.mk)
endif

# COMPILE FLAGS

CC=cmoc
AS=cmoc
LD=lwlink

#CFLAGS=+coleco -subtype=coco
#LDFLAGS=+coleco -subtype=coco -o$(DIST_DIR)/$(PROGRAM).ddp -create-app -lsmartkeys -leos
#ASFLAGS=+coleco -subtype=coco

################################################################
# DISK creation

SUFFIX = .bin
VICE_HOME := $(dir $(shell which xvic))

DISK_FILE = $(DIST_DIR)/$(PROGRAM).dsk



################################################################
# TESTING / EMULATOR
