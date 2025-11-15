// Raspberry Pi 5 GPIO Controller Implementation
// RP1 GPIO based on pi5_os reference

#include "gpio.h"
#include "timer.h"

#define GPIO_CTRL_FUNCSEL_MASK  0x1F

// Helper function to read GPIO register
static uint32_t gpio_read_reg(uint32_t pin, uint32_t offset) {
    volatile uint32_t *reg = (volatile uint32_t *)(RP1_GPIO_BASE + (pin * 8) + offset);
    return *reg;
}

// Helper function to write GPIO register
static void gpio_write_reg(uint32_t pin, uint32_t offset, uint32_t value) {
    volatile uint32_t *reg = (volatile uint32_t *)(RP1_GPIO_BASE + (pin * 8) + offset);
    *reg = value;
}

// Initialize GPIO controller
void gpio_init(void) {
    // Test GPIO access
    uint32_t status = gpio_read_reg(0, GPIO_STATUS);
    (void)status;  // Prevent unused variable warning
    
    // GPIO is initialized by bootloader
}

// Set GPIO function
void gpio_set_function(uint32_t pin, uint32_t function) {
    uint32_t ctrl = gpio_read_reg(pin, GPIO_CTRL);
    ctrl &= ~GPIO_CTRL_FUNCSEL_MASK;
    ctrl |= (function & GPIO_CTRL_FUNCSEL_MASK);
    gpio_write_reg(pin, GPIO_CTRL, ctrl);
}

// Set GPIO as output
void gpio_set_output(uint32_t pin) {
    gpio_set_function(pin, GPIO_FUNC_SIO);
}

// Set GPIO as input
void gpio_set_input(uint32_t pin) {
    gpio_set_function(pin, GPIO_FUNC_SIO);
}

// Set GPIO pin value
void gpio_set(uint32_t pin, int value) {
    // Implementation would require SIO base access
    // Simplified for now
    (void)pin;
    (void)value;
}

// Get GPIO pin value
int gpio_get(uint32_t pin) {
    uint32_t status = gpio_read_reg(pin, GPIO_STATUS);
    return (status & 1);
}

// Turn on LED
void gpio_led_on(uint32_t pin) {
    gpio_set_output(pin);
    gpio_set(pin, 1);
}

// Turn off LED
void gpio_led_off(uint32_t pin) {
    gpio_set_output(pin);
    gpio_set(pin, 0);
}

// Blink activity LED
void gpio_blink_activity_led(void) {
    gpio_led_on(GPIO_LED_ACT);
    timer_delay_ms(100);
    gpio_led_off(GPIO_LED_ACT);
    timer_delay_ms(100);
}
