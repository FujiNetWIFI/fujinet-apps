TARGET_EXEC ?= mastodon

BUILD_DIR ?= ./build
SRC_DIRS ?= ./src

CC=cmoc

SRCS := $(shell find $(SRC_DIRS) -name *.c)
OBJS := $(SRCS:%=$(BUILD_DIR)/%.o)
DEPS := $(OBJS:.o=.d)

CFLAGS=
LDFLAGS=-o$(TARGET_EXEC).bin

INC_DIRS := $(shell find $(SRC_DIRS) -type d)
INC_FLAGS := $(addprefix -I,$(INC_DIRS))

$(BUILD_DIR)/$(TARGET_EXEC): $(OBJS)
	$(CC) $(LDFLAGS) $(OBJS)
	$(RM) $(TARGET_EXEC)
	decb dskini $(TARGET_EXEC).dsk
	writecocofile $(TARGET_EXEC).dsk $(TARGET_EXEC).bin
	cp $(TARGET_EXEC).dsk ~/tnfs/

# c source
$(BUILD_DIR)/%.c.o: %.c
	$(MKDIR_P) $(dir $@)
	$(CC) $(CFLAGS) -o $@ -c $<

.PHONY: clean

clean:
	$(RM) -r $(TARGET_EXEC)*

-include $(DEPS)

MKDIR_P ?= mkdir -p
