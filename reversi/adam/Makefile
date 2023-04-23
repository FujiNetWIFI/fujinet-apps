# set these environment variables in your .basrc or windows environment
# to automagically copy the FUJIAPPLE.po file to the TNFS server

# .bashrc
# TNFS_SERVER="192.168.2.21"
# TNFS_SHARE="tnfs/apple"
# export TNFS_SERVER
# export TNFS_SHARE
#
# windows
# SET TNFS_SERVER=tnfs
# SET TNFS_SHARE=tnfs\apple

TNFS_SERVER = $(shell echo $$TNFS_SERVER)
TNFS_SHARE  = $(shell echo $$TNFS_SHARE)
OS          = $(shell echo $$OS)


TARGET_EXEC ?= reversi

BUILD_DIR ?= ./build
SRC_DIRS ?= ./src

CC=zcc
AS=zcc

ADDL_DIR1 := ../../../eoslib/src
ADDL_DIR2 := ../../../smartkeyslib/src
ADDL_DIR3 := ./src

ADDL_LIB1 := ../../../eoslib/eos.lib
ADDL_LIB2 := ../../../smartkeyslib/smartkeys.lib

SRCS := $(shell find $(SRC_DIRS) -name *.cpp -or -name *.c -or -name *.asm)
OBJS := $(SRCS:%=$(BUILD_DIR)/%.o)
DEPS := $(OBJS:.o=.d)

CFLAGS=+coleco -subtype=adam -DBUILD_ADAM  -I$(ADDL_DIR1) -I$(ADDL_DIR2) -I$(ADDL_DIR3)
LDFLAGS=+coleco -subtype=adam -pragma-redirect:CRT_FONT=_font_8x8_coleco_adam_system -o$(TARGET_EXEC) -create-app -l$(ADDL_LIB1) -l$(ADDL_LIB2) 
ASFLAGS=+coleco -subtype=adam


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
	$(RM) -r $(TARGET_EXEC)* $(BUILD_DIR)

go:
#    @echo "We make things go"
	/home/ndavie/Documents/ADAM/ColEm/colem -ntsc -adam $(TARGET_EXEC).ddp


tnfs: $(TARGET_EXEC).ddp 
ifneq ('$(TNFS_SERVER)','')

ifneq ('$(OS)','')
    cp reversi.ddp //$(TNFS_SERVER)/$(TNFS_SHARE)
else
	cp reversi.ddp /run/user/1000/gvfs/smb-share:server=$(TNFS_SERVER),share=$(TNFS_SHARE)
endif

else
	$(info ***********************************************)
	$(info *** No TNFS server environment variable set ***)
	$(info ***********************************************)
endif


-include $(DEPS)

MKDIR_P ?= mkdir -p


