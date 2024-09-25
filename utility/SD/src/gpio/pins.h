#ifndef PINS_H
#define PINS_H

#include <WVariant.h>

enum gpio_pmux { 
    GPIO_PMUX_A, 
    GPIO_PMUX_B,
    GPIO_PMUX_C,
    GPIO_PMUX_D,
    GPIO_PMUX_E,
    GPIO_PMUX_F,
    GPIO_PMUX_G,
    GPIO_PMUX_H,
    GPIO_PMUX_I,
    GPIO_PMUX_J,
    GPIO_PMUX_K,
    GPIO_PMUX_L,
    GPIO_PMUX_M,
    GPIO_PMUX_N
};

enum gpio_direction { 
    GPIO_DIRECTION_OFF, 
    GPIO_DIRECTION_IN, 
    GPIO_DIRECTION_OUT };

enum gpio_pull_mode { 
    GPIO_PULL_OFF, 
    GPIO_PULL_UP, 
    GPIO_PULL_DOWN };

typedef struct {
    EPortType type;
    uint32_t pin_no;
} pin_t;

void sdhc_setup();

void set_pin_pmux(const pin_t * pin, enum gpio_pmux mux);

void set_pin_dir(const pin_t * pin, enum gpio_direction direction);

void set_pin_pull_mode(const pin_t * pin, enum gpio_pull_mode pull_mode);

void set_pin_level(const pin_t * pin, uint8_t level);

void toggle_pin_level(const pin_t * pin);

#endif // PINS_H