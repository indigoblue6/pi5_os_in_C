# Raspberry Pi 5 Baremetal OS - Makefile
# Based on pi5_os reference implementation

# Cross-compiler toolchain
CROSS_COMPILE = aarch64-linux-gnu-
CC = $(CROSS_COMPILE)gcc
LD = $(CROSS_COMPILE)ld
OBJCOPY = $(CROSS_COMPILE)objcopy
OBJDUMP = $(CROSS_COMPILE)objdump

# Compiler flags
CFLAGS = -Wall -O2 -ffreestanding -nostdinc -nostdlib -nostartfiles
CFLAGS += -mcpu=cortex-a76 -mtune=cortex-a76
CFLAGS += -I./include

# Linker flags
LDFLAGS = -T ldscript.ld -nostdlib

# Source files
SRC_C = src/main.c src/uart.c src/timer.c src/gpio.c
SRC_ASM = src/startup.s

# Object files
OBJ_C = $(SRC_C:.c=.o)
OBJ_ASM = $(SRC_ASM:.s=.o)
OBJS = $(OBJ_ASM) $(OBJ_C)

# Output files
KERNEL_ELF = kernel8.elf
KERNEL_BIN = kernel8.img

# Default target
all: $(KERNEL_BIN)

# Build kernel binary
$(KERNEL_BIN): $(KERNEL_ELF)
	@echo "Creating kernel image: $@"
	$(OBJCOPY) -O binary $< $@
	@echo "Kernel size: $$(stat -c%s $@) bytes"

# Link kernel
$(KERNEL_ELF): $(OBJS)
	@echo "Linking kernel: $@"
	$(LD) $(LDFLAGS) -o $@ $(OBJS)

# Compile C files
%.o: %.c
	@echo "Compiling: $<"
	$(CC) $(CFLAGS) -c $< -o $@

# Compile assembly files
%.o: %.s
	@echo "Assembling: $<"
	$(CC) $(CFLAGS) -c $< -o $@

# Clean build artifacts
clean:
	@echo "Cleaning build artifacts..."
	rm -f $(OBJS) $(KERNEL_ELF) $(KERNEL_BIN)
	@echo "Clean complete"

# Disassemble kernel for debugging
disassemble: $(KERNEL_ELF)
	@echo "Disassembling kernel..."
	$(OBJDUMP) -D $< > kernel8.asm

# Display kernel information
info: $(KERNEL_BIN)
	@echo "========================================="
	@echo "Raspberry Pi 5 Kernel Information"
	@echo "========================================="
	@echo "Kernel Binary: $(KERNEL_BIN)"
	@echo "Size: $$(stat -c%s $(KERNEL_BIN)) bytes"
	@echo "Architecture: ARM64 (Cortex-A76)"
	@echo "Load Address: 0x200000"
	@echo "========================================="

# Connect to UART for debugging
uart:
	@echo "Connecting to UART (Ctrl+A, K to exit)..."
	@echo "Make sure UART is connected to GPIO 14/15"
	screen /dev/ttyUSB0 115200

# Help
help:
	@echo "Raspberry Pi 5 Baremetal OS - Build System"
	@echo ""
	@echo "Available targets:"
	@echo "  make          - Build kernel8.img"
	@echo "  make clean    - Remove build artifacts"
	@echo "  make disassemble - Generate assembly listing"
	@echo "  make info     - Display kernel information"
	@echo "  make uart     - Connect to UART console"
	@echo "  make help     - Show this help"

.PHONY: all clean disassemble info uart help
