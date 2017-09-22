/*
 * spi.h
 *
 *  Created on: Aug 25, 2016
 *      Author: asuarez
 */

#ifndef DRIVERS_PERIPHERALS_SPI_H_
#define DRIVERS_PERIPHERALS_SPI_H_

#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "inc/hw_ssi.h"
#include "inc/hw_types.h"
#include "driverlib/ssi.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/sysctl.h"
#include "driverlib/interrupt.h"
#include "driverlib/rom.h"

typedef struct spi{
	uint32_t ssi_base;
	uint32_t ssi_clk;
	uint32_t protocol;
	uint32_t mode;
	uint32_t bit_rate;
	uint32_t data_width;

	uint32_t cs_peripheral;
    uint32_t cs_port;
    uint8_t cs_pin;
} SPI;

void spi_init(SPI *spi);

/*
 * Transfer data SSI internal FIFO buffer
 * Buffer allows up to eight 16-bit values to be stored
 * independly.
 */
void spi_transfer(SPI *spi, uint32_t *data_tx, uint32_t *data_rx, uint32_t num_of_packets);

void spi_cs_enable(SPI *spi);

void spi_cs_disable(SPI *spi);

#endif /* DRIVERS_PERIPHERALS_SPI_H_ */
