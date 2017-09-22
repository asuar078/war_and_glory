/*
 * spi.c
 *
 *  Created on: Aug 25, 2016
 *      Author: asuarez
 */

#include <drivers/peripherals/spi.h>

void spi_init(SPI *spi){

	if (spi->ssi_base == SSI0_BASE){
		/* enable ssi function on port A, enable port A */
		ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_SSI0);
		ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);

		// Wait for the SSI0 module to be ready.
		while(!SysCtlPeripheralReady(SYSCTL_PERIPH_SSI0)) {}

		/* configure pins for ssi mode */
		ROM_GPIOPinConfigure(GPIO_PA2_SSI0CLK);
		ROM_GPIOPinConfigure(GPIO_PA4_SSI0RX);
		ROM_GPIOPinConfigure(GPIO_PA5_SSI0TX);

		/* configure pins for ssi mode */
		ROM_GPIOPinTypeSSI(GPIO_PORTA_BASE, GPIO_PIN_2|GPIO_PIN_4|GPIO_PIN_5);
	}
	if (spi->ssi_base == SSI2_BASE){
		/* enable ssi function on port B, enable port B */
		ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_SSI2);
		ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);

		// Wait for the SSI0 module to be ready.
		while(!SysCtlPeripheralReady(SYSCTL_PERIPH_SSI2)) {}

		/* configure pins for ssi mode */
		ROM_GPIOPinConfigure(GPIO_PB4_SSI2CLK);
		ROM_GPIOPinConfigure(GPIO_PB6_SSI2RX);
		ROM_GPIOPinConfigure(GPIO_PB7_SSI2TX);

		/* configure pins for ssi mode */
		ROM_GPIOPinTypeSSI(GPIO_PORTB_BASE, GPIO_PIN_4|GPIO_PIN_6|GPIO_PIN_7);
	} else{
		/* Something else */
	}

	/* ssi settings */
	//ROM_SSIConfigSetExpClk(SSI0_BASE,SysCtlClockGet(),SSI_FRF_MOTO_MODE_0, SSI_MODE_MASTER, 1000000, 8);
	ROM_SSIConfigSetExpClk(spi->ssi_base, spi->ssi_clk, spi->protocol, spi->mode, spi->bit_rate, spi->data_width);

	/* enable function */
	ROM_SSIEnable(spi->ssi_base);

    // enable GPIO port for cs pin
    ROM_SysCtlPeripheralEnable(spi->cs_peripheral);
    ROM_GPIOPinTypeGPIOOutput(spi->cs_port, spi->cs_pin);
}

void spi_transfer(SPI *spi, uint32_t *data_tx, uint32_t *data_rx, uint32_t num_of_packets){
    uint32_t index;		/* Used to count the number of bytes Tx or Rx */

	/* For loop used to send bytes of data */
	for(index = 0; index < num_of_packets; index++)
	{
		/* Send the data using the "blocking" put function */
		SSIDataPut(spi->ssi_base, data_tx[index]);

		while(SSIBusy(spi->ssi_base)){}

		/* Receive the data using the "blocking" get function */
		SSIDataGet(spi->ssi_base, &data_rx[index]);

		while(SSIBusy(spi->ssi_base)){}
	}
}

void spi_cs_enable(SPI *spi){
    GPIOPinWrite(spi->cs_port, spi->cs_pin, 0);     /* cs low */
}

void spi_cs_disable(SPI *spi){
    GPIOPinWrite(spi->cs_port, spi->cs_pin, spi->cs_pin);     /* cs high */
}
