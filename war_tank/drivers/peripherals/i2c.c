/*
 * i2c.c
 *
 *  Created on: Sep 22, 2016
 *      Author: asuarez
 */

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

#include "drivers/peripherals/i2c.h"

void i2c_init(I2C *i2c){

	if (i2c->base == I2C0_BASE){
		//enable I2C module 0
		SysCtlPeripheralEnable(SYSCTL_PERIPH_I2C0);

		//reset module
		SysCtlPeripheralReset(SYSCTL_PERIPH_I2C0);

		//enable GPIO peripheral that contains I2C 0
		SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);

		// Configure the pin muxing for I2C0 functions on port B2 and B3.
		GPIOPinConfigure(GPIO_PB2_I2C0SCL);
		GPIOPinConfigure(GPIO_PB3_I2C0SDA);

		// Select the I2C function for these pins.
		GPIOPinTypeI2CSCL(GPIO_PORTB_BASE, GPIO_PIN_2);
		GPIOPinTypeI2C(GPIO_PORTB_BASE, GPIO_PIN_3);

		// Enable and initialize the I2C0 master module.  Use the system clock for
		// the I2C0 module.  The last parameter sets the I2C data transfer rate.
		// If false the data rate is set to 100kbps and if true the data rate will
		// be set to 400kbps.
		I2CMasterInitExpClk(i2c->base, i2c->clk, i2c->fast_mode);

		//clear I2C FIFOs
		HWREG(I2C0_BASE + I2C_O_FIFOCTL) = 80008000;
	}
	else if (i2c->base == I2C1_BASE){
		/* Enable Peripheral Clocks */
		ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_I2C1);

		//reset module
		SysCtlPeripheralReset(SYSCTL_PERIPH_I2C1);

		//enable GPIO peripheral that contains I2C 1
		ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);

		// Configure the pin muxing for I2C1
		ROM_GPIOPinConfigure(GPIO_PA6_I2C1SCL);
		ROM_GPIOPinConfigure(GPIO_PA7_I2C1SDA);

		// Select the I2C function for these pins.
		ROM_GPIOPinTypeI2CSCL(GPIO_PORTA_BASE, GPIO_PIN_6);
		ROM_GPIOPinTypeI2C(GPIO_PORTA_BASE, GPIO_PIN_7);

		// Enable and initialize the I2C0 master module.  Use the system clock for
		// the I2C0 module.  The last parameter sets the I2C data transfer rate.
		// If false the data rate is set to 100kbps and if true the data rate will
		// be set to 400kbps.
		I2CMasterInitExpClk(i2c->base, i2c->clk, i2c->fast_mode);

		//clear I2C FIFOs
		HWREG(I2C1_BASE + I2C_O_FIFOCTL) = 80008000;
	}
	else{
		/* Something else */
	}
}


void i2c_send(I2C *i2c, uint8_t *data_tx, uint16_t num_of_bytes){

	// Tell the master module what address it will place on the bus when
	// communicating with the slave.
	I2CMasterSlaveAddrSet(i2c->base, i2c->i2c_addr, false);

	//put data to be sent into FIFO
	I2CMasterDataPut(i2c->base, *data_tx++);

	//if there is only one argument, we only need to use the
	//single send I2C function
	if(num_of_bytes == 1)
	{
		//Initiate send of data from the MCU
		I2CMasterControl(i2c->base, I2C_MASTER_CMD_SINGLE_SEND);

		// Wait until MCU is done transferring.
		while(I2CMasterBusy(i2c->base));
	}
	//otherwise, we start transmission of multiple bytes on the
	//I2C bus
	else
	{
		//Initiate send of data from the MCU
		I2CMasterControl(i2c->base, I2C_MASTER_CMD_BURST_SEND_START);

		// Wait until MCU is done transferring.
		while(I2CMasterBusy(i2c->base));
        uint8_t i;
		//send num_of_args-2 pieces of data, using the
		//BURST_SEND_CONT command of the I2C module
		for(i = 1; i < (num_of_bytes - 1); i++)
		{
			//put next piece of data into I2C FIFO
			I2CMasterDataPut(i2c->base, *data_tx++);
			//send next data that was just placed into FIFO
			I2CMasterControl(i2c->base, I2C_MASTER_CMD_BURST_SEND_CONT);

			// Wait until MCU is done transferring.
			while(I2CMasterBusy(i2c->base));
		}

		//put last piece of data into I2C FIFO
		I2CMasterDataPut(i2c->base, *data_tx);
		//send next data that was just placed into FIFO
		I2CMasterControl(i2c->base, I2C_MASTER_CMD_BURST_SEND_FINISH);
		// Wait until MCU is done transferring.
		while(I2CMasterBusy(i2c->base));
	}
}

void i2c_send_no_stop(I2C *i2c, uint8_t *data_tx, uint16_t num_of_bytes){

	// Tell the master module what address it will place on the bus when
	// communicating with the slave.
	I2CMasterSlaveAddrSet(i2c->base, i2c->i2c_addr, false);

	//put data to be sent into FIFO
	I2CMasterDataPut(i2c->base, *data_tx++);

	//if there is only one argument, we only need to use the
	//single send I2C function
	if(num_of_bytes == 1)
	{
		//Initiate send of data from the MCU
		I2CMasterControl(i2c->base, I2C_MASTER_CMD_SINGLE_SEND);

		// Wait until MCU is done transferring.
		while(I2CMasterBusy(i2c->base));
	}
	//otherwise, we start transmission of multiple bytes on the
	//I2C bus
	else
	{
		//Initiate send of data from the MCU
		I2CMasterControl(i2c->base, I2C_MASTER_CMD_BURST_SEND_START);

		// Wait until MCU is done transferring.
		while(I2CMasterBusy(i2c->base));
        uint8_t i;
		//send num_of_args-2 pieces of data, using the
		//BURST_SEND_CONT command of the I2C module
		for(i = 1; i < num_of_bytes; i++)
		{
			//put next piece of data into I2C FIFO
			I2CMasterDataPut(i2c->base, *data_tx++);
			//send next data that was just placed into FIFO
			I2CMasterControl(i2c->base, I2C_MASTER_CMD_BURST_SEND_CONT);

			// Wait until MCU is done transferring.
			while(I2CMasterBusy(i2c->base));
		}
	}
}

void i2c_send_with_control(I2C *i2c, uint8_t *control_bytes, uint8_t num_of_control, uint8_t *data_tx, uint16_t num_of_bytes){
	// Tell the master module what address it will place on the bus when
	// communicating with the slave.
	I2CMasterSlaveAddrSet(i2c->base, i2c->i2c_addr, false);

    //put data to be sent into FIFO
    I2CMasterDataPut(i2c->base, *control_bytes++);

    //Initiate send of data from the MCU
    I2CMasterControl(i2c->base, I2C_MASTER_CMD_BURST_SEND_START);

    while(I2CMasterBusy(i2c->base));

    uint8_t i;
    for(i = 1; i < num_of_control; i++){
        //put data to be sent into FIFO
        I2CMasterDataPut(i2c->base, *control_bytes++);

        //Initiate send of data from the MCU
        I2CMasterControl(i2c->base, I2C_MASTER_CMD_BURST_SEND_CONT);

        // Wait until MCU is done transferring.
        while(I2CMasterBusy(i2c->base));
    }

    //send num_of_args-2 pieces of data, using the
    //BURST_SEND_CONT command of the I2C module
    for(i = 0; i < (num_of_bytes - 1); i++)
    {
        //put next piece of data into I2C FIFO
        I2CMasterDataPut(i2c->base, *data_tx++);
        //send next data that was just placed into FIFO
        I2CMasterControl(i2c->base, I2C_MASTER_CMD_BURST_SEND_CONT);

        // Wait until MCU is done transferring.
        while(I2CMasterBusy(i2c->base));
    }

    //put last piece of data into I2C FIFO
    I2CMasterDataPut(i2c->base, *data_tx);
    //send next data that was just placed into FIFO
    I2CMasterControl(i2c->base, I2C_MASTER_CMD_BURST_SEND_FINISH);
    // Wait until MCU is done transferring.
    while(I2CMasterBusy(i2c->base));
}

uint32_t i2c_receive(I2C *i2c, uint8_t *control_bytes, uint16_t num_of_control){

	i2c_send_no_stop(i2c, control_bytes, num_of_control);

	//specify that we are going to read from slave device
	I2CMasterSlaveAddrSet(i2c->base, i2c->i2c_addr, true);

	//send control byte and read from the register we
	//specified
	I2CMasterControl(i2c->base, I2C_MASTER_CMD_SINGLE_RECEIVE);

	//wait for MCU to finish transaction
	while(I2CMasterBusy(i2c->base));

	//return data pulled from the specified register
	return I2CMasterDataGet(i2c->base);
}

void i2c_receive_burst(I2C *i2c, uint8_t *control_bytes, uint16_t num_of_control, uint8_t *data_rx, uint16_t num_of_read){

	i2c_send_no_stop(i2c, control_bytes, num_of_control);

	//if there is only one argument, we only need to use the
	//single send I2C function
	if(num_of_read == 1)
	{
		//specify that we are going to read from slave device
		I2CMasterSlaveAddrSet(i2c->base, i2c->i2c_addr, true);

		//send control byte and read from the register we
		//specified
		I2CMasterControl(i2c->base, I2C_MASTER_CMD_SINGLE_RECEIVE);

		//wait for MCU to finish transaction
		while(I2CMasterBusy(i2c->base));

		// data pulled from the specified register
		*data_rx = I2CMasterDataGet(i2c->base);
	}
	//otherwise, we start transmission of multiple bytes on the
	//I2C bus
	else
	{
		//specify that we are going to read from slave device
		I2CMasterSlaveAddrSet(i2c->base, i2c->i2c_addr, true);

		//send next data that was just placed into FIFO
		I2CMasterControl(i2c->base, I2C_MASTER_CMD_BURST_RECEIVE_START);

		// Wait until MCU is done transferring.
		while(I2CMasterBusy(i2c->base));

		*data_rx++ = I2CMasterDataGet(i2c->base);

        uint16_t i;
		for(i = 1; i < (num_of_read - 1); i++)
		{
			//send next data that was just placed into FIFO
			I2CMasterControl(i2c->base, I2C_MASTER_CMD_BURST_RECEIVE_CONT);

			// Wait until MCU is done transferring.
			while(I2CMasterBusy(i2c->base));

			// data pulled from the specified register
			*data_rx++ = I2CMasterDataGet(i2c->base);
		}

		//send next data that was just placed into FIFO
		I2CMasterControl(i2c->base, I2C_MASTER_CMD_BURST_RECEIVE_FINISH);
		// Wait until MCU is done transferring.
		while(I2CMasterBusy(i2c->base));

		*data_rx = I2CMasterDataGet(i2c->base);
	}
}

