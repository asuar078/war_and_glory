/*
 * i2c.h
 *
 *  Created on: Sep 22, 2016
 *      Author: asuarez
 */

#ifndef DRIVERS_PERIPHERALS_I2C_H_
#define DRIVERS_PERIPHERALS_I2C_H_


typedef struct {
	uint32_t base;
	uint32_t clk;
	bool fast_mode;
	uint8_t i2c_addr;
} I2C;

void i2c_init(I2C *i2c);

void i2c_send(I2C *i2c, uint8_t *data_tx, uint16_t num_of_bytes);

void i2c_send_no_stop(I2C *i2c, uint8_t *data_tx, uint16_t num_of_bytes);

void i2c_send_with_control(I2C *i2c, uint8_t *control_bytes, uint8_t num_of_control, uint8_t *data_tx, uint16_t num_of_bytes);

uint32_t i2c_receive(I2C *i2c, uint8_t *control_bytes, uint16_t num_of_bytes);

void i2c_receive_burst(I2C *i2c, uint8_t *control_bytes, uint16_t num_of_control, uint8_t *data_rx, uint16_t num_of_read);

#endif /* DRIVERS_PERIPHERALS_I2C_H_ */
