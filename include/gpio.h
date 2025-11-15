// Raspberry Pi 5 GPIO Controller Header
// RP1 GPIO based on Ubuntu linux-raspi pinctrl-rp1.c

#ifndef GPIO_H
#define GPIO_H

#include <stdint.h>

// RP1 GPIO base address
#define RP1_GPIO_BASE       0x1f000d0000UL

// GPIO register offsets
#define GPIO_STATUS         0x0000
#define GPIO_CTRL           0x0004

// GPIO pins
#define GPIO_LED_ACT        29  // Activity LED
#define GPIO_LED_PWR        31  // Power LED

// Function select values
#define GPIO_FUNC_UART      2
#define GPIO_FUNC_SIO       5   // Software controlled I/O

// Function declarations
void gpio_init(void);
void gpio_set_function(uint32_t pin, uint32_t function);
void gpio_set_output(uint32_t pin);
void gpio_set_input(uint32_t pin);
void gpio_set(uint32_t pin, int value);
int gpio_get(uint32_t pin);
void gpio_led_on(uint32_t pin);
void gpio_led_off(uint32_t pin);
void gpio_blink_activity_led(void);

#endif // GPIO_H
