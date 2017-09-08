#ifndef _L293D_H
#define _L293D_H

#include <stdint.h>

typedef struct {
    uint32_t period;
    uint32_t enable_pwm_base;
    uint32_t enable_pwm_out_pin;

    uint32_t forward_periph;
    uint32_t forward_port;
    uint8_t forward_pin;

    uint32_t reverse_periph;
    uint32_t reverse_port;
    uint8_t reverse_pin;
} L293D_pins;

void L293D_init(L293D_pins *pins);
int L293D_set(L293D_pins* pins, float duty);


#endif
