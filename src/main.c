// Raspberry Pi 5 Baremetal OS - Main Kernel
// Based on pi5_os reference implementation in C

#include <stdint.h>
#include "uart.h"
#include "timer.h"
#include "gpio.h"

// External symbols from linker script
extern uint64_t _BSS_START;
extern uint64_t _BSS_END;

// IRQ handler stub (called from assembly)
void c_irq_handler(void) {
    // IRQ handling would go here
}

// Test functions
static void test_memory(void) {
    uart_puts("  Memory Test: ");
    
    // Stack allocation test
    uint8_t test_array[256];
    uint32_t checksum = 0;
    
    for (int i = 0; i < 256; i++) {
        test_array[i] = i % 256;
        checksum += test_array[i];
    }
    
    uart_puts("checksum=");
    uart_put_hex(checksum);
    uart_puts(" OK\n");
}

static void test_timer(void) {
    uart_puts("  Timer Test: ");
    
    for (int i = 1; i <= 5; i++) {
        timer_delay_ms(200);
        uart_putc('0' + i);
        uart_putc(' ');
    }
    
    uart_puts("OK\n");
}

static void test_gpio(void) {
    uart_puts("  GPIO Test: ");
    
    // Test GPIO access
    const uint32_t GPIO_BASE = 0x107d200000UL;
    volatile uint32_t *gpio_status = (volatile uint32_t *)GPIO_BASE;
    uint32_t status = *gpio_status;
    
    uart_puts("status=");
    uart_put_hex(status);
    uart_puts(" OK\n");
}

static void run_hardware_tests(void) {
    uart_puts("\n=== PI5 HARDWARE TESTS ===\n");
    
    uart_puts("1. UART Test:\n");
    uart_puts("  Character output: Hello Pi5!\n");
    
    uart_puts("2. Memory Test:\n");
    test_memory();
    
    uart_puts("3. Timer Test:\n");
    test_timer();
    
    uart_puts("4. GPIO Test:\n");
    test_gpio();
    
    uart_puts("=== ALL TESTS COMPLETE ===\n\n");
}

static void simple_shell(void) {
    char input[128];
    int pos = 0;
    
    uart_puts("\n");
    uart_puts("========================================\n");
    uart_puts("   PI5 BAREMETAL OS - C VERSION        \n");
    uart_puts("   Based on pi5_os reference           \n");
    uart_puts("========================================\n");
    uart_puts("\n");
    uart_puts("Type 'help' for available commands\n\n");
    
    while (1) {
        uart_puts("pi5os> ");
        
        // Read input
        pos = 0;
        while (1) {
            char c = uart_getc();
            
            if (c == '\r' || c == '\n') {
                uart_puts("\n");
                input[pos] = '\0';
                break;
            } else if (c == 127 || c == '\b') {  // Backspace
                if (pos > 0) {
                    pos--;
                    uart_puts("\b \b");
                }
            } else if (pos < 127) {
                input[pos++] = c;
                uart_putc(c);
            }
        }
        
        // Process command
        if (pos == 0) continue;
        
        // Compare commands
        int is_help = 1, is_uptime = 1, is_test = 1, is_clear = 1, is_reboot = 1;
        const char *help = "help";
        const char *uptime = "uptime";
        const char *test = "test";
        const char *clear = "clear";
        const char *reboot = "reboot";
        
        for (int i = 0; help[i]; i++) {
            if (input[i] != help[i]) is_help = 0;
        }
        for (int i = 0; uptime[i]; i++) {
            if (input[i] != uptime[i]) is_uptime = 0;
        }
        for (int i = 0; test[i]; i++) {
            if (input[i] != test[i]) is_test = 0;
        }
        for (int i = 0; clear[i]; i++) {
            if (input[i] != clear[i]) is_clear = 0;
        }
        for (int i = 0; reboot[i]; i++) {
            if (input[i] != reboot[i]) is_reboot = 0;
        }
        
        if (is_help && input[4] == '\0') {
            uart_puts("Available commands:\n");
            uart_puts("  help    - Show this help\n");
            uart_puts("  uptime  - Show system uptime\n");
            uart_puts("  test    - Run hardware tests\n");
            uart_puts("  clear   - Clear screen\n");
            uart_puts("  reboot  - Restart system\n");
        } else if (is_uptime && input[6] == '\0') {
            uint32_t uptime_sec = timer_get_uptime_seconds();
            uint32_t hours = uptime_sec / 3600;
            uint32_t minutes = (uptime_sec % 3600) / 60;
            uint32_t seconds = uptime_sec % 60;
            
            uart_puts("System uptime: ");
            uart_put_hex(hours);
            uart_puts("h ");
            uart_put_hex(minutes);
            uart_puts("m ");
            uart_put_hex(seconds);
            uart_puts("s\n");
        } else if (is_test && input[4] == '\0') {
            run_hardware_tests();
        } else if (is_clear && input[5] == '\0') {
            uart_puts("\x1b[2J\x1b[H");  // ANSI clear screen
        } else if (is_reboot && input[6] == '\0') {
            uart_puts("Reboot not implemented. Please reset manually.\n");
        } else {
            uart_puts(input);
            uart_puts(": command not found\n");
        }
    }
}

// Main kernel entry point (called from startup.s)
void kernel_main(void) {
    // Initialize UART
    uart_init();
    
    // Print banner
    uart_puts("\n\n");
    uart_puts("========================================\n");
    uart_puts("   Raspberry Pi 5 Baremetal OS (C)     \n");
    uart_puts("   BCM2712 - ARM Cortex-A76            \n");
    uart_puts("========================================\n");
    uart_puts("\n");
    
    // Initialize hardware
    uart_puts("Initializing hardware...\n");
    timer_init();
    uart_puts("  Timer: OK\n");
    
    gpio_init();
    uart_puts("  GPIO: OK\n");
    
    uart_puts("Hardware initialization complete!\n");
    
    // Run hardware tests
    run_hardware_tests();
    
    // Start simple shell
    simple_shell();
    
    // Should never reach here
    while (1) {
        __asm__ volatile("wfe");
    }
}
