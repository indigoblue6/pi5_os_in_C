// Raspberry Pi 5 System Timer Header
// BCM2712 System Timer

#ifndef TIMER_H
#define TIMER_H

#include <stdint.h>

// BCM2712 System Timer base address
#define SYSTEM_TIMER_BASE   0x107c003000UL
#define TIMER_CLO           0x04  // Timer Counter Lower 32 bits
#define TIMER_CHI           0x08  // Timer Counter Higher 32 bits

// Function declarations
void timer_init(void);
uint64_t timer_get_ticks(void);
void timer_delay_us(uint32_t us);
void timer_delay_ms(uint32_t ms);
uint32_t timer_get_uptime_seconds(void);

#endif // TIMER_H
