#include <Arduino.h>
#include "motor_task.h"
#include <stdbool.h>
#include <stdint.h>
#include <Arduino_FreeRTOS.h>
#include <task.h>
#include <semphr.h>  // add the FreeRTOS functions for Semaphores (or Flags).
#include <queue.h>
#include "priorities.h"
#include "L293D.h"

//*****************************************************************************
//
// The stack size for the motor task.
//
//*****************************************************************************
#define MOTORTASKSTACKSIZE        128         // Stack size in words

//*****************************************************************************
//
// The item size and queue size for the LED message queue.
//
//*****************************************************************************
#define MOTOR_ITEM_SIZE           sizeof(BUTTON)
#define MOTOR_QUEUE_SIZE          5

// Motor 1 pins
const int speedPin_1 = 3;    // that is the pin that we use to control the motor's speed
const int forwardPin_1 = 5; // this is the pin that we use to tell the motor to go forward
const int reversePin_1 = 2; // this is the pin that we use to tell the motor to go reverse

// Motor 2 pins
const int speedPin_2 = 6;    // that is the pin that we use to control the motor's speed
const int forwardPin_2 = 9; // this is the pin that we use to tell the motor to go forward
const int reversePin_2 = 10; // this is the pin that we use to tell the motor to go reverse

L293D motor1(speedPin_1, forwardPin_1, reversePin_1);
L293D motor2(speedPin_2, forwardPin_2, reversePin_2);

int motor_direc_1 = 0;
int motor_direc_2 = 0;

float motor_speed_1 = 1;
float motor_speed_2 = 1;

QueueHandle_t motor_queue;
extern SemaphoreHandle_t serial_semaphore;

#define FORWARD 5
#define BACK 6
#define LEFT 7
#define RIGHT 8

int motor_control(BUTTON *button){

  if(button->pressed == 0){
      motor_direc_1 = 0;
      motor_direc_2 = 0;

      return 0;
  }

  switch (button->butt_num) {
    case 1:
      motor_speed_1 = 1;
      motor_speed_2 = 1;
      break;
    case 2:
      motor_speed_1 = 0.75;
      motor_speed_2 = 0.75;
      break;
    case 3:
      motor_speed_1 = 0.50;
      motor_speed_2 = 0.50;
      break;
    case 4:
      motor_speed_1 = 0.25;
      motor_speed_2 = 0.25;
      break;

    case FORWARD:
      Serial.println("forward");
      motor_direc_1 = 1;
      motor_direc_2 = 1;
      break;
    case BACK:
      Serial.println("back");
      motor_direc_1 = -1;
      motor_direc_2 = -1;
      break;
    case LEFT:
      Serial.println("left");
      motor_direc_1 = -1;
      motor_direc_2 = 1;
      break;
    case RIGHT:
      Serial.println("right");
      motor_direc_1 = 1;
      motor_direc_2 = -1;
      break;
    default:
      return 1;
  }

  return 0;
}

motor_task::motor_task(void){

}

void print_button(BUTTON *press){
  Serial.print("Button: "); Serial.println(press->butt_num);
  Serial.print("Pressed: "); Serial.println(press->pressed);
}

static void task_loop(void *pvParameters)
{

    BUTTON button_pressed;
    // Loop forever.
    while(1)
    {

      // if ( xSemaphoreTake( serial_semaphore, ( TickType_t ) 5 ) == pdTRUE )
      // {
      //   Serial.println("motor task");
      //   xSemaphoreGive( serial_semaphore ); // Now free or "Give" the Serial Port for others.
      // }

      // Read the next message, if available on queue.
      if(xQueueReceive(motor_queue, &button_pressed, 0) == pdPASS)
      {
        if ( xSemaphoreTake( serial_semaphore, ( TickType_t ) 5 ) == pdTRUE )
        {
            motor_control(&button_pressed);
           xSemaphoreGive( serial_semaphore ); // Now free or "Give" the Serial Port for others.
        }
      }

      motor1.set(motor_direc_1*motor_speed_1);
      motor2.set(motor_direc_2*motor_speed_2);

      taskYIELD();
    // vTaskDelay(200);

    }
}

//*****************************************************************************
//
// Initializes the motor task.
//
//*****************************************************************************
int motor_task::create_task(void)
{

  // Create a queue for sending messages to the LED task.
    motor_queue = xQueueCreate(MOTOR_QUEUE_SIZE, MOTOR_ITEM_SIZE);

    // Create the task.
    if(xTaskCreate(task_loop, (const portCHAR *)"Motor", MOTORTASKSTACKSIZE, NULL,
                   tskIDLE_PRIORITY + PRIORITY_MOTOR_TASK, NULL) != pdTRUE)
    {
        return(1);
    }

    // Success.
    return(0);
}
