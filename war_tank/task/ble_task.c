/*
 * ble_task.c
 *
 *  Created on: Sep 18, 2017
 *      Author: bigbywolf
 */

#include <stdbool.h>
#include <stdint.h>
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/pin_map.h"
#include "driverlib/gpio.h"
#include "driverlib/rom.h"
#include "driverlib/i2c.h"
#include "driverlib/sysctl.h"
#include "driverlib/systick.h"
#include "driverlib/pwm.h"

#include "utils/uartstdio.h"

#include "priorities.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "motor_task.h"

#include "../drivers/peripherals/spi.h"

//*****************************************************************************
//
// The stack size for the BLE toggle task.
//
//*****************************************************************************
#define BLETASKSTACKSIZE        128         // Stack size in words

xSemaphoreHandle spiMutex;

void print_array(int32_t *arr, uint32_t size){
    int i;
    for(i = 0; i < size; i++){
        UARTprintf("RX[%d] = %c\n\r", i, arr[i]);
    }
    UARTprintf("\n\r");
}

static void ble_task(void *pvParameters){

    /* SPI communication */
    SPI bluefruit = {
        .ssi_base = SSI2_BASE,
        .ssi_clk = SysCtlClockGet(),
        .protocol = SSI_FRF_MOTO_MODE_0,
        .mode = SSI_MODE_MASTER,
        .bit_rate = 1000000,
        .data_width = 8,

        .cs_peripheral = SYSCTL_PERIPH_GPIOB,
        .cs_port = GPIO_PORTB_BASE,
        .cs_pin = GPIO_PIN_5
    };

    spi_init(&bluefruit);

    // Create a mutex to guard the spi bus.
    spiMutex = xSemaphoreCreateMutex();

    UARTprintf("bluefruit start\n\r");

    /* MCP3912_config_pins(mcp_pins); */
    uint8_t transfer_size = 13;
    uint32_t data[13] = {0}; // An array for 10 data bytes
    data[0] = "A"; data[1] = "T"; data[2] = "I"; data[3] = "\n";

    spi_cs_enable(&bluefruit);
    vTaskDelay( 0.1 );
    spi_transfer(&bluefruit, data, data, 13);
    spi_cs_disable(&bluefruit);

 while(1){

//     xSemaphoreTake(spiMutex, portMAX_DELAY); // Suspend this task indefinitely until the mutex is available
//     xSemaphoreGive(spiMutex);



 }

}

uint32_t ble_task_init(void)
{

    // Create the LED task.
    if(xTaskCreate(ble_task, (const portCHAR *)"BLE", BLETASKSTACKSIZE, NULL,
                   tskIDLE_PRIORITY + PRIORITY_BLE_TASK, NULL) != pdTRUE)
    {
        return(1);
    }

    // Success.
    return(0);
}


