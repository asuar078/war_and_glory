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

		/* configure pins for ssi mode */
		ROM_GPIOPinConfigure(GPIO_PA2_SSI0CLK);
		ROM_GPIOPinConfigure(GPIO_PA4_SSI0RX);
		ROM_GPIOPinConfigure(GPIO_PA5_SSI0TX);

		/* configure pins for ssi mode */
		ROM_GPIOPinTypeSSI(GPIO_PORTA_BASE, GPIO_PIN_2|GPIO_PIN_4|GPIO_PIN_5);
	}
	else{
		/* Something else */
	}

	/* ssi settings */
	//ROM_SSIConfigSetExpClk(SSI0_BASE,SysCtlClockGet(),SSI_FRF_MOTO_MODE_0, SSI_MODE_MASTER, 1000000, 8);
	ROM_SSIConfigSetExpClk(spi->ssi_base, spi->ssi_clk, spi->protocol, spi->mode, spi->bit_rate, spi->data_width);

	/* enable function */
	ROM_SSIEnable(spi->ssi_base);
}

void spi_transfer(uint32_t ssi_base, uint32_t *data_tx, uint32_t *data_rx, uint32_t num_of_packets){
    uint32_t index;		/* Used to count the number of bytes Tx or Rx */

	/* For loop used to send bytes of data */
	for(index = 0; index < num_of_packets; index++)
	{
		/* Send the data using the "blocking" put function */
		SSIDataPut(ssi_base, data_tx[index]);

		while(SSIBusy(ssi_base)){}

		/* Receive the data using the "blocking" get function */
		SSIDataGet(ssi_base, &data_rx[index]);

		while(SSIBusy(ssi_base)){}
	}
}
