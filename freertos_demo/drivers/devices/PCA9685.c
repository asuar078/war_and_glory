#include "PCA9685.h"
#include "../drivers/peripherals/i2c.h"

I2C PCA9685_i2c

void PCA9685_init(int bus,int addr){

    PCA9685_i2c.base = I2C1_BASE;
    PCA9685_i2c.clk = SysCtlClockGet();
    PCA9685_i2c.fast_mode = false;

    i2c_init(&PCA9685_i2c);

}

void PCA9685_setPWMFreq(int freq){

}

void PCA9685_setPWM(uint8_t led, int on_value, int off_value){

}

void PCA9685_setPWM(uint8_t led, int value){

}

int PCA9685_getPWM(uint8_t led){

}

void PCA9685_reset(void){

}
