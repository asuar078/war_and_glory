#include <stdbool.h>
#include <stdint.h>
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/gpio.h"
#include "driverlib/rom.h"
#include "utils/uartstdio.h"
#include "priorities.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "motor_task.h"

//*****************************************************************************
//
// The stack size for the LED toggle task.
//
//*****************************************************************************
#define MOTORTASKSTACKSIZE        128         // Stack size in words

//*****************************************************************************
//
// The item size and queue size for the LED message queue.
//
//*****************************************************************************
#define MOTOR_ITEM_SIZE           sizeof(uint8_t)
#define MOTOR_QUEUE_SIZE          5

static void motor_task(void *pvParameters)
{
    I2C i2c;
    i2c.base = I2C0_BASE;
    i2c.clk = SysCtlClockGet();
    i2c.fast_mode = false;

    i2c_init(&i2c);

    // Loop forever.
    while(1)
    {

    }
}

//*****************************************************************************
//
// Initializes the LED task.
//
//*****************************************************************************
uint32_t motor_task_init(void)
{

    // Print the current loggling LED and frequency.
    //UARTprintf("\nLed %d is blinking. [R, G, B]\n", g_ui8ColorsIndx);
    //UARTprintf("Led blinking frequency is %d ms.\n", (LED_TOGGLE_DELAY * 2));

    // Create the LED task.
    if(xTaskCreate(motor_task, (const portCHAR *)"Motor", MOTORTASKSTACKSIZE, NULL,
                   tskIDLE_PRIORITY + PRIORITY_MOTOR_TASK, NULL) != pdTRUE)
    {
        return(1);
    }

    // Success.
    return(0);
}
