TARGET_EXEC ?= weather.bin
TARGET_DISK ?= weather.dsk
BUILD_DIR ?= ./build
SRC_DIRS ?= ./src

CC=cmoc
CP=cp
WRITECOCOFILE=writecocofile
DECB=decb

SRCS := $(shell find $(SRC_DIRS) -name *.cpp -or -name *.c -or -name *.asm)
OBJS := $(SRCS:%=$(BUILD_DIR)/%.o)
DEPS := $(OBJS:.o=.d)

CFLAGS=
LDFLAGS=-o$(TARGET_EXEC)

INC_DIRS := $(shell find $(SRC_DIRS) -type d)
INC_FLAGS := $(addprefix -I,$(INC_DIRS))

$(BUILD_DIR)/$(TARGET_EXEC): $(OBJS)
	$(CC) $(LDFLAGS) $(OBJS)
	$(RM) $(TARGET_DISK)
	$(DECB) dskini $(TARGET_DISK)
	$(WRITECOCOFILE) $(TARGET_DISK) $(TARGET_EXEC)
	$(CP) weather.dsk ~/Workspace/tnfs

# c source
$(BUILD_DIR)/%.c.o: %.c
	$(MKDIR_P) $(dir $@)
	$(CC) $(CPPFLAGS) $(CFLAGS) -o $@ -c $< 

.PHONY: clean

clean:
	$(RM) -r weather* $(BUILD_DIR)

-include $(DEPS)

MKDIR_P ?= mkdir -p
