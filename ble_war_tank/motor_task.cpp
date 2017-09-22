// #include <stdbool.h>
// #include <stdint.h>
// #include <Arduino_FreeRTOS.h>
// #include <semphr.h>  // add the FreeRTOS functions for Semaphores (or Flags).
// #include "motor_task.h"
// #include "priorities.h"
//
// //*****************************************************************************
// //
// // The stack size for the motor task.
// //
// //*****************************************************************************
// #define MOTORTASKSTACKSIZE        128         // Stack size in words
//
// //*****************************************************************************
// //
// // The item size and queue size for the LED message queue.
// //
// //*****************************************************************************
// #define MOTOR_ITEM_SIZE           sizeof(uint8_t)
// #define MOTOR_QUEUE_SIZE          5
//
//
// static void motor_task(void *pvParameters)
// {
//
//     // Loop forever.
//     while(1)
//     {
//     }
// }
//
// //*****************************************************************************
// //
// // Initializes the motor task.
// //
// //*****************************************************************************
// uint32_t motor_task_init(void)
// {
//     // Create the task.
//     if(xTaskCreate(motor_task, (const portCHAR *)"Motor", MOTORTASKSTACKSIZE, NULL,
//                    tskIDLE_PRIORITY + PRIORITY_MOTOR_TASK, NULL) != pdTRUE)
//     {
//         return(1);
//     }
//
//     // Success.
//     return(0);
// }
