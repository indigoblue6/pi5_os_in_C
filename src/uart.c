// Raspberry Pi 5 UART Driver Implementation
// BCM2712 UART (PL011) via RP1 chip
// Based on pi5_os reference

#include "uart.h"

// Initialize UART - bootloader has already configured it
void uart_init(void) {
    // No initialization needed for early UART
    // Already configured by bootloader
}

// Write a single character
void uart_putc(char c) {
    // Handle newline conversion to CRLF
    if (c == '\n') {
        uart_putc('\r');
    }
    
    // Wait for TX FIFO not full
    while (*BCM2712_UART_FR & UART_FR_TXFF) {
        __asm__ volatile("nop");
    }
    
    // Write character
    *BCM2712_UART_DR = (uint32_t)c;
}

// Write a string
void uart_puts(const char *s) {
    while (*s) {
        uart_putc(*s++);
    }
}

// Read a character
char uart_getc(void) {
    // Wait for RX FIFO not empty
    while (*BCM2712_UART_FR & UART_FR_RXFE) {
        __asm__ volatile("nop");
    }
    
    // Read character
    return (char)(*BCM2712_UART_DR & 0xFF);
}

// Print hex number for debugging
void uart_put_hex(uint32_t num) {
    const char hex_chars[] = "0123456789ABCDEF";
    uart_puts("0x");
    
    for (int i = 7; i >= 0; i--) {
        uint32_t nibble = (num >> (i * 4)) & 0xF;
        uart_putc(hex_chars[nibble]);
    }
}
