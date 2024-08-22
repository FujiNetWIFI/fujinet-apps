TARGET_EXEC ?= mastodon

BUILD_DIR ?= ./build
SRC_DIRS ?= ./src

CC=zcc
AS=zcc

SRCS := $(shell find $(SRC_DIRS) -name *.c -or -name *_pmd85.asm)
OBJS := $(SRCS:%=$(BUILD_DIR)/%.o)
DEPS := $(OBJS:.o=.d)

CFLAGS=+pmd85 -DBUILD_PMD85
LDFLAGS=+pmd85 -create-app -o $(TARGET_EXEC).bin
ASFLAGS=+pmd85


INC_DIRS := $(shell find $(SRC_DIRS) -type d)
INC_FLAGS := $(addprefix -I,$(INC_DIRS))

all: $(TARGET_EXEC).rmm

# executable
$(TARGET_EXEC).bin: $(OBJS)
	$(CC) $(LDFLAGS) $(OBJS)

# ROM Module
$(TARGET_EXEC).rmm: romload.bin $(TARGET_EXEC).bin
	(cat romload.bin; dd if=$(TARGET_EXEC).bin count=4 bs=1 status=none; \
	dd if=$(TARGET_EXEC).bin skip=13 bs=1 status=none) > $@

# ROM Module boot code
romload.bin:
	printf '\xcd\x00\x8c\x00\x00\xff\x17\x00\x00' > $@

# c source
$(BUILD_DIR)/%.c.o: %.c
	$(MKDIR_P) $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

# asm source
$(BUILD_DIR)/%.asm.o: %.asm
	$(MKDIR_P) $(dir $@)
	$(AS) $(ASFLAGS) -c $< -o $@

.PHONY: clean all

clean:
	$(RM) -r $(TARGET_EXEC)* $(BUILD_DIR) romload.bin

-include $(DEPS)

MKDIR_P ?= mkdir -p
