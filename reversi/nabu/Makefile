
# set these environment variables in your .basrc or windows environment
# to automagically copy the FUJIAPPLE.po file to the TNFS server

# .bashrc
# TNFS_SERVER="192.168.2.21"
# TNFS_SHARE="tnfs"
# export TNFS_SERVER
# export TNFS_SHARE
#
# windows
# SET TNFS_SERVER=tnfs
# SET TNFS_SHARE=tnfs

PROJECT_DEVICE := nabu
TNFS_SERVER ?= $(shell echo $$TNFS_SERVER)
TNFS_SHARE  ?= $(shell echo $$TNFS_SHARE)
TNFS_DIR    ?= NABU
OS          ?= $(shell echo $$OS)
COLEM_PATH  ?= $(shell echo $$COLEM_PATH)
NABEM_PATH  ?= $(shell echo $$NABEM_PATH)
APPEM_PATH  ?= $(shell echo $$APPEM_PATH)
NABU_CACHE  ?= $(shell echo $$NABU_CACHE)

TARGET_EXEC ?= reversi.nabu

ifneq ('$(OS)','')
	WINE_REQUIRED := 'wine '
else
	WINE_REQUIRED := ''
endif
BUILD_DIR ?= ./build
SRC_DIRS ?= ./src

CC=zcc
AS=z88dk-z80asm

ADDL_DIR1 :=
ADDL_DIR2 := 
ADDL_DIR3 := 

ADDL_LIB1 :=
ADDL_LIB2 := 

SRCS := $(shell find $(SRC_DIRS) -name *.cpp -or -name *.c -or -name *.asm)
OBJS := $(SRCS:%=$(BUILD_DIR)/%.o)
DEPS := $(OBJS:.o=.d)

CFLAGS=+nabu  -vn -m  -compiler=sdcc -DBUILD_NABU -I$(ADDL_DIR1) -I$(ADDL_DIR2) -I$(ADDL_DIR3)
LDFLAGS=+nabu -o$(TARGET_EXEC) -create-app -l$(ADDL_LIB1) -l$(ADDL_LIB2) 
ASFLAGS=+nabu 

INC_DIRS := $(shell find $(SRC_DIRS) -type d)
INC_FLAGS := $(addprefix -I,$(INC_DIRS))

$(BUILD_DIR)/$(TARGET_EXEC): $(OBJS)
	$(CC) $(LDFLAGS) $(OBJS)

# c source
$(BUILD_DIR)/%.c.o: %.c
	$(MKDIR_P) $(dir $@)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

# asm source
$(BUILD_DIR)/%.asm.o: %.asm
	$(MKDIR_P) $(dir $@)
	$(AS) $(ASFLAGS) -c $< -o $@

.PHONY: clean

clean:
	$(RM) $(TARGET_EXEC)
	$(RM) -r $(BUILD_DIR)

go:
#ifeq ('$(PROJECT_DEVICE)','colem')
#	$(COLEM_PATH)/colem -ntsc -adam $(TARGET_EXEC).ddp
#endif
#ifeq ('$(PROJECT_DEVICE)','nabu')
#    cp ./$(TARGET_EXEC) $(NABU_CACHE)/000001.nabu
#	$(WINE_REQUIRED) $(NABEM_PATH)/mame nabupc -window -kbd nabu_hle -hcca null_modem -bitb socket.127.0.0.1:5816
#endif
#ifeq ('$(PROJECT_DEVICE)','apple')
#	$(WINE_REQUIRED) $(APPEM_PATH)/applewin -d1 $(TARGET_EXEC).po
#endif


-include $(DEPS)

MKDIR_P ?= mkdir -p

tnfs: $(TARGET_EXEC)
	cp $(TARGET_EXEC) $(NABU_CACHE)/000001.nabu 
ifneq ('$(TNFS_SERVER)','')
ifneq ('$(OS)','')
    cp $(TARGET_EXEC) //$(TNFS_SERVER)/$(TNFS_SHARE)/$(TNFS_DIR)
else
	cp $(TARGET_EXEC) /run/user/1000/gvfs/smb-share:server=$(TNFS_SERVER),share=$(TNFS_SHARE)/$(TNFS_DIR)
endif
else
	$(info ***********************************************)
	$(info *** No TNFS server environment variable set ***)
	$(info ***********************************************)
endif