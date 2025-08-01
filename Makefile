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

tools:
	$(MAKE) -C $(TOOLS_SRC_DIR)/mkfs.fatkatt CC=$(CCTOOLS) BUILD_DIR=$(abspath $(BUILD_DIR))

wondertart:
	mkdir -p bin
	dd if=/dev/zero of=$(BUILD_DIR)/wondertart.img bs=1b count=2880 conv=notrunc