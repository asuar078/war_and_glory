#include "L293D.h"

#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_types.h"
#include "inc/hw_memmap.h"
#include "inc/hw_timer.h"
#include "inc/hw_ints.h"
#include "driverlib/sysctl.h"
#include "driverlib/rom.h"
#include "driverlib/rom_map.h"
#include "driverlib/pin_map.h"
#include "driverlib/timer.h"
#include "driverlib/gpio.h"
#include "driverlib/interrupt.h"
#include "driverlib/pwm.h"


void L293D_init(L293D_pins *pins){

    // Enable the GPIO Port
    ROM_SysCtlPeripheralEnable(pins->forward_periph);
    ROM_SysCtlPeripheralEnable(pins->reverse_periph);

    // Set GPIO as output
    ROM_GPIOPinTypeGPIOOutput(pins->forward_port, pins->forward_pin);
    ROM_GPIOPinTypeGPIOOutput(pins->reverse_port, pins->reverse_pin);
}

int L293D_set(L293D_pins* pins, float duty){

    if(duty > 1 || duty < -1){
        return 1;
    }

    if (duty == 0){
        PWMPulseWidthSet(pins->enable_pwm_base, pins->enable_pwm_out_pin, 0);
        return 0;
    }
    if (duty > 0){
        GPIOPinWrite(pins->forward_port, pins->forward_pin, pins->forward_pin);
        GPIOPinWrite(pins->reverse_port, pins->reverse_pin, 0);
        PWMPulseWidthSet(pins->enable_pwm_base, pins->enable_pwm_out_pin, pins->period * duty);
        PWMOutputState(pins->enable_pwm_base, pins->enable_pwm_out_pin, true);
        return 0;
    }
    if (duty < 0){
        GPIOPinWrite(pins->forward_port, pins->forward_pin, 0);
        GPIOPinWrite(pins->reverse_port, pins->reverse_pin, pins->reverse_pin);
        PWMPulseWidthSet(pins->enable_pwm_base, pins->enable_pwm_out_pin, (pins->period * (duty*-1)));
        PWMOutputState(pins->enable_pwm_base, pins->enable_pwm_out_pin, true);
        return 0;
    }
}
