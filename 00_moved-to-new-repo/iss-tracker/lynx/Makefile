SOURCE_DIR=src
BUILD_DIR=build
BIN = iss.lnx

CC65=$(CC65_HOME)
CC65_BIN=$(CC65)/bin
CC65_INC=$(CC65)/include
CC65_ASMINC=$(CC65)/asminc
CC65_TOOLS=$(CC65_BIN)

# Tool executables
CO=co65
CC=cc65
AS=ca65
AR=ar65
CL=cl65
SPRPCK=sprpck
CP=cp
RM=rm -f
ECHO=echo
TOUCH=touch

# Tool flags
SYS=lynx
TGI=$(SYS)-160-102-16
JOY=$(SYS)-stdjoy
SEGMENTS=--code-name CODE --rodata-name RODATA --bss-name BSS --data-name DATA
ASFLAGS=-t $(SYS) -I $(CC65_ASMINC)
CFLAGS=$(SEGMENTS) --all-cdecl -I src/headers -t $(SYS) -Cl -Or -O
LDFLAGS=-C lynx.cfg -t $(SYS) $(SYS).lib
SPRFLAGS=-t6 -p1

.SUFFIXES:
.PHONY: clean mkbuild all
VPATH = src/c src/images src/asm

# == Assembly sources to assembly objects
ASM_SOURCES = $(wildcard $(SOURCE_DIR)/asm/*.s)
ASM_OBJECTS = $(patsubst $(SOURCE_DIR)/asm/%.s, $(BUILD_DIR)/%.o, $(ASM_SOURCES))
$(BUILD_DIR)/%.o: %.s
	@$(ECHO) == Assembling $< =\> $@
	@$(AS) $(ASFLAGS) $< -o $@

# == C sources to assembly intermediate files to C objects
C_SOURCES = $(wildcard $(SOURCE_DIR)/c/*.c)
C_OBJECTS = $(patsubst $(SOURCE_DIR)/c/%.c, $(BUILD_DIR)/%.o, $(C_SOURCES))
$(BUILD_DIR)/%.o: %.c
	@$(ECHO) == Compiling $< =\> $@
	@$(CC) $(CFLAGS) $< -o $(BUILD_DIR)/$*.s
	@$(AS) $(ASFLAGS) $(BUILD_DIR)/$*.s -o $@

# == Bitmap files to intermediate assembly to bitmap objects
BMP_SOURCES = $(wildcard $(SOURCE_DIR)/images/*.bmp)
BMP_OBJECTS = $(patsubst $(SOURCE_DIR)/images/%.bmp, $(BUILD_DIR)/%.o, $(BMP_SOURCES))
$(BUILD_DIR)/%.o: %.bmp
	@$(ECHO) == Converting bitmap $< to $@
	@$(SPRPCK) $(SPRFLAGS) $< $@
	@$(ECHO) .global _$* > $(BUILD_DIR)/$*.s
	@$(ECHO) .segment \"RODATA\" >> $(BUILD_DIR)/$*.s
	@$(ECHO) _$*: .incbin \"$*.spr\" >> $(BUILD_DIR)/$*.s
	@$(AS) $(ASFLAGS) $(BUILD_DIR)/$*.s -o $@

$(BUILD_DIR)/img_turkey1.o : SPRFLAGS+=-a030019

# == Link all objects to final binary
$(BIN): $(BMP_OBJECTS) $(ASM_OBJECTS) $(C_OBJECTS)
	@$(ECHO) == Linking binary executable to $@
	@$(CL) $(LDFLAGS) $^ -o $@

mkbuild:
	@mkdir -p build

all: mkbuild $(BIN)

clean:
	@$(ECHO) == Cleaning up
	@$(RM) -rf $(BUILD_DIR) $(BIN)

