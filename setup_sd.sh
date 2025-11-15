#!/bin/bash
# Raspberry Pi 5 SD Card Setup Script

echo "========================================="
echo "Raspberry Pi 5 Baremetal OS SD Setup"
echo "========================================="
echo ""

# Check if SD card mount point is provided
if [ -z "$1" ]; then
    echo "Usage: $0 /path/to/sd/boot/partition"
    echo ""
    echo "Example:"
    echo "  sudo ./setup_sd.sh /media/user/bootfs"
    echo ""
    echo "NOTE: You need the following files in boot partition:"
    echo "  - bootcode.bin (Pi5 doesn't use this)"
    echo "  - start4.elf or start.elf"
    echo "  - fixup4.dat or fixup.dat"
    echo "  - bcm2712-rpi-5-b.dtb (device tree)"
    echo "  - overlays/ directory"
    echo ""
    echo "These files can be obtained from:"
    echo "  https://github.com/raspberrypi/firmware/tree/master/boot"
    exit 1
fi

BOOT_MOUNT="$1"

if [ ! -d "$BOOT_MOUNT" ]; then
    echo "Error: Directory $BOOT_MOUNT does not exist"
    exit 1
fi

echo "Boot partition: $BOOT_MOUNT"
echo ""

# Check for kernel8.img
if [ ! -f "kernel8.img" ]; then
    echo "Error: kernel8.img not found in current directory"
    echo "Please run 'make' first"
    exit 1
fi

echo "Copying files to SD card..."
echo ""

# Copy kernel
echo "  - kernel8.img"
cp kernel8.img "$BOOT_MOUNT/"

# Copy config.txt
echo "  - config.txt"
cp config.txt "$BOOT_MOUNT/"

echo ""
echo "Files copied successfully!"
echo ""
echo "IMPORTANT: Make sure your SD card also has:"
echo "  - Raspberry Pi firmware files (start*.elf, fixup*.dat)"
echo "  - Device tree file (bcm2712-rpi-5-b.dtb)"
echo "  - overlays/ directory"
echo ""
echo "You can get these from a standard Raspberry Pi OS installation"
echo "or from: https://github.com/raspberrypi/firmware/tree/master/boot"
echo ""
echo "========================================="
