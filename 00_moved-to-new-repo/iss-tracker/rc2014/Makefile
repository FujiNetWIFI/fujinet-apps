CC=zcc
TARGET_EXEC ?= iss

TARGET=+rc2014 -subtype=cpm -clib=sdcc_iy
CFLAGS=--max-allocs-per-node200000 -SO3

LIB_FUJINET := -l../../../fujinet-rc2014/libfn_cpm -l../../../fujinet-rc2014/libfujinet
LIB_FUJINET_INC := -I../../../fujinet-rc2014/lib/include

BUILD_DIR ?= ./build
SRC_DIRS ?= ./src

.PHONY: clean all $(BUILD_DIR)/$(TARGET_EXEC)

all: $(BUILD_DIR)/$(TARGET_EXEC)

$(BUILD_DIR)/$(TARGET_EXEC):
	$(MKDIR_P) $(dir $@)
	${CC} ${TARGET} -v -m --list ${CFLAGS} ${LIB_FUJINET_INC} ${LIB_FUJINET} -lm @src/iss.lst -o $@ -create-app


clean:
	$(RM) -r $(BUILD_DIR)

MKDIR_P ?= mkdir -p
