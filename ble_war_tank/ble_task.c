/*
 * ble_task.c
 *
 *  Created on: Sep 18, 2017
 *      Author: bigbywolf
 */

#include <stdbool.h>
#include <stdint.h>
#include <Arduino_FreeRTOS.h>
#include <semphr.h>  // add the FreeRTOS functions for Semaphores (or Flags).
#include "priorities.h"
/* #include "FreeRTOS.h" */
/* #include "task.h" */
/* #include "queue.h" */
/* #include "semphr.h" */
#include "motor_task.h"


//*****************************************************************************
//
// The stack size for the BLE toggle task.
//
//*****************************************************************************
#define BLETASKSTACKSIZE        128         // Stack size in words

static void ble_task(void *pvParameters){


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


