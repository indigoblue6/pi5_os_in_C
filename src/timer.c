// Raspberry Pi 5 System Timer Implementation
// BCM2712 System Timer runs at 1MHz
// Based on pi5_os reference

#include "timer.h"

// Initialize system timer
void timer_init(void) {
    // BCM2712 system timer is already running at 1MHz
    // No initialization needed
}

// Get current timer tick count (microseconds)
uint64_t timer_get_ticks(void) {
    volatile uint32_t *timer_base = (volatile uint32_t *)SYSTEM_TIMER_BASE;
    uint32_t hi, lo;
    
    // Read high and low parts
    hi = timer_base[TIMER_CHI / 4];
    lo = timer_base[TIMER_CLO / 4];
    
    return ((uint64_t)hi << 32) | lo;
}

// Delay in microseconds
void timer_delay_us(uint32_t us) {
    uint64_t start = timer_get_ticks();
    uint64_t target = start + us;
    
    while (timer_get_ticks() < target) {
        __asm__ volatile("nop");
    }
}

// Delay in milliseconds
void timer_delay_ms(uint32_t ms) {
    timer_delay_us(ms * 1000);
}

// Get system uptime in seconds
uint32_t timer_get_uptime_seconds(void) {
    return (uint32_t)(timer_get_ticks() / 1000000);
}
