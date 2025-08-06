# Environment
CCTOOLS = gcc # C compiler for tools
CC = wcc
ASM = nasm
ASMFLAGS = -f bin

# Paths
BUILD_DIR = bin
SRC_DIR = src
TOOLS_SRC_DIR = tools

.PHONY: tools

all: tools wondertart

tools: always
	$(MAKE) -C $(TOOLS_SRC_DIR)/fatkatt CC=$(CCTOOLS) \
		BUILD_DIR=$(abspath $(BUILD_DIR))

wondertart: always
	$(BUILD_DIR)/mkfs.fk $(BUILD_DIR)/wondertart.img

always:
	mkdir -p bin

clean:
	rm $(BUILD_DIR)/*.o