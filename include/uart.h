// Raspberry Pi 5 UART Driver Header
// BCM2712 UART (PL011) via RP1 chip

#ifndef UART_H
#define UART_H

#include <stdint.h>

// BCM2712 UART register addresses - based on pi5_os
#define BCM2712_UART_BASE   0x107d001000UL
#define BCM2712_UART_DR     ((volatile uint32_t*)(BCM2712_UART_BASE + 0x00))
#define BCM2712_UART_FR     ((volatile uint32_t*)(BCM2712_UART_BASE + 0x18))

// Flag Register bits
#define UART_FR_RXFE        (1 << 4)  // RX FIFO empty
#define UART_FR_TXFF        (1 << 5)  // TX FIFO full

// Function declarations
void uart_init(void);
void uart_putc(char c);
void uart_puts(const char *s);
char uart_getc(void);
void uart_put_hex(uint32_t num);

#endif // UART_H
