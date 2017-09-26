/*********************************************************************
 This is an example for our nRF51822 based Bluefruit LE modules

 Pick one up today in the adafruit shop!

 Adafruit invests time and resources providing this open source code,
 please support Adafruit and open-source hardware by purchasing
 products from Adafruit!

 MIT license, check LICENSE for more information
 All text above, and the splash screen below must be included in
 any redistribution
*********************************************************************/

#include <Arduino.h>
#include <Arduino_FreeRTOS.h>
#include <semphr.h>
#include <SPI.h>
#include "Adafruit_BLE.h"
#include "Adafruit_BluefruitLE_SPI.h"
#include "Adafruit_BluefruitLE_UART.h"

#include "BluefruitConfig.h"
#include "ble_task.h"
#include "motor_task.h"

SemaphoreHandle_t serial_semaphore;

ble_task ble_t;
motor_task motor_t;

void setup(void)
{
  Serial.begin(115200);

  // Semaphores are useful to stop a Task proceeding, where it should be paused to wait,
  // because it is sharing a resource, such as the Serial port.
  // Semaphores should only be used whilst the scheduler is running, but we can set it up here.
  if ( serial_semaphore == NULL )  // Check to confirm that the Serial Semaphore has not already been created.
  {
    serial_semaphore = xSemaphoreCreateMutex();  // Create a mutex semaphore we will use to manage the Serial Port
    if ( ( serial_semaphore ) != NULL )
      xSemaphoreGive( ( serial_semaphore ) );  // Make the Serial Port available for use, by "Giving" the Semaphore.
  }

  ble_t.create_task();
  motor_t.create_task();

}
void loop(void)
{
}
