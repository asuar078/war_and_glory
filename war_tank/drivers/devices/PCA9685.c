#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_i2c.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "inc/hw_gpio.h"
#include "driverlib/i2c.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/rom.h"
#include "PCA9685.h"
#include "../peripherals/i2c.h"

I2C PCA9685_i2c;

void PCA9685_init(I2C *i2c){

    PCA9685_i2c = *i2c;

//    PCA9685_i2c.base = I2C1_BASE;
//    PCA9685_i2c.clk = SysCtlClockGet();
//    PCA9685_i2c.fast_mode = false;

    i2c_init(&PCA9685_i2c);

}

void PCA9685_setPWMFreq(int freq){

    uint8_t prescale_val = (CLOCK_FREQ / 4096 / freq)  - 1;

    uint8_t control = MODE1;
    uint8_t data = 0x10;

//    i2c->write_byte(MODE1, 0x10); //sleep
    i2c_send_with_control(&PCA9685_i2c, &control, 1, &data, 1); // sleep

    control = PRE_SCALE;
    data = prescale_val;
//    i2c->write_byte(PRE_SCALE, prescale_val); // multiplyer for PWM frequency
    i2c_send_with_control(&PCA9685_i2c, &control, 1, &data, 1);

    control = MODE1;
    data = 0x80;
//    i2c->write_byte(MODE1, 0x80); //restart
    i2c_send_with_control(&PCA9685_i2c, &control, 1, &data, 1);

    control = MODE2;
    data = 0x04;
//    i2c->write_byte(MODE2, 0x04); //totem pole (default)
    i2c_send_with_control(&PCA9685_i2c, &control, 1, &data, 1);
}

void PCA9685_setPWM(uint8_t led, int on_value, int off_value){

}


void PCA9685_reset(void){

}
