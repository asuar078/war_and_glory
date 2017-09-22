/*
 * ble_task.c
 *
 *  Created on: Sep 18, 2017
 *      Author: bigbywolf
 */
#include <Arduino.h>
#include "ble_task.h"
#include <stdbool.h>
#include <stdint.h>
#include <Arduino_FreeRTOS.h>
#include <semphr.h>  // add the FreeRTOS functions for Semaphores (or Flags).
#include "priorities.h"
/* #include "FreeRTOS.h" */
/* #include "task.h" */
 #include "queue.h"
/* #include "semphr.h" */
#include "motor_task.h"


#include "Adafruit_BLE.h"
#include "Adafruit_BluefruitLE_SPI.h"
#include "Adafruit_BluefruitLE_UART.h"

#include "BluefruitConfig.h"

//*****************************************************************************
// The stack size for the BLE toggle task.
//*****************************************************************************
#define BLETASKSTACKSIZE        128         // Stack size in words

#define FACTORYRESET_ENABLE        1
#define MINIMUM_FIRMWARE_VERSION   "0.7.0"

#define RED_LED A0
#define GREEN_LED A1

extern QueueHandle_t motor_queue;

Adafruit_BluefruitLE_SPI ble(BLUEFRUIT_SPI_CS, BLUEFRUIT_SPI_IRQ, BLUEFRUIT_SPI_RST);

int32_t charid_string;
int32_t charid_number;
BUTTON button_pressed;

// A small helper
void error(const __FlashStringHelper*err) {
  Serial.println(err);
  while (1);
}

void connected(void)
{
  Serial.println( F("Connected") );
  digitalWrite(RED_LED, LOW);
  digitalWrite(GREEN_LED, HIGH);
}

void disconnected(void)
{
  Serial.println( F("Disconnected") );
  digitalWrite(RED_LED, HIGH);
  digitalWrite(GREEN_LED, LOW);
}

void BleUartRX(char data[], uint16_t len)
{
  Serial.print( F("[BLE UART RX]" ) );
  Serial.write(data, len);
  Serial.println();

  // Buttons
  if (data[1] == 'B') {
    button_pressed.butt_num = data[2] - '0';
    button_pressed.pressed = data[3] - '0';
    Serial.print ("Button "); Serial.print(button_pressed.butt_num);
    if (button_pressed.pressed) {
      Serial.println(" pressed");
    } else {
      Serial.println(" released");
    }

    if(xQueueSend(motor_queue, &button_pressed, portMAX_DELAY) !=
       pdPASS)
    {
        // Error. The queue should never be full. If so print the
        // error message on UART and wait for ever.
        Serial.println("\nQueue full. This should never happen.\n");
    }
  }

}

void BleGattRX(int32_t chars_id, uint8_t data[], uint16_t len)
{
  Serial.print( F("[BLE GATT RX] (" ) );
  Serial.print(chars_id);
  Serial.print(") ");

  if (chars_id == charid_string)
  {
    Serial.write(data, len);
    Serial.println();
  }else if (chars_id == charid_number)
  {
    int32_t val;
    memcpy(&val, data, len);
    Serial.println(val);
  }
}

ble_task::ble_task(void){

}

static void task_loop( void *pvParameters __attribute__((unused)) ) {
  pinMode(RED_LED, OUTPUT);
  pinMode(GREEN_LED, OUTPUT);

  digitalWrite(RED_LED, HIGH);
  digitalWrite(GREEN_LED, LOW);

  Serial.println(F("BLE War Tank"));
  Serial.println(F("-------------------------------------"));

  /* Initialise the module */
  Serial.print(F("Initialising the Bluefruit LE module: "));

  if ( !ble.begin(VERBOSE_MODE) )
  {
    error(F("Couldn't find Bluefruit, make sure it's in CoMmanD mode & check wiring?"));
  }
  Serial.println( F("OK!") );

  if ( FACTORYRESET_ENABLE )
  {
    /* Perform a factory reset to make sure everything is in a known state */
    Serial.println(F("Performing a factory reset: "));
    if ( ! ble.factoryReset() ){
      error(F("Couldn't factory reset"));
    }
  }

  if ( !ble.isVersionAtLeast(MINIMUM_FIRMWARE_VERSION) )
  {
    error( F("Callback requires at least 0.7.0") );
  }

  Serial.println( F("Adding Service 0x1234 with 2 chars 0x2345 & 0x6789") );
  ble.sendCommandCheckOK( F("AT+GATTADDSERVICE=uuid=0x1234") );
  ble.sendCommandWithIntReply( F("AT+GATTADDCHAR=UUID=0x2345,PROPERTIES=0x08,MIN_LEN=1,MAX_LEN=6,DATATYPE=string,DESCRIPTION=string,VALUE=abc"), &charid_string);
  ble.sendCommandWithIntReply( F("AT+GATTADDCHAR=UUID=0x6789,PROPERTIES=0x08,MIN_LEN=4,MAX_LEN=4,DATATYPE=INTEGER,DESCRIPTION=number,VALUE=0"), &charid_number);

  ble.reset();

  /* Disable command echo from Bluefruit */
  ble.echo(false);

  Serial.println(F("Requesting Bluefruit info:"));
  /* Print Bluefruit information */
  ble.info();

  /* Set callbacks */
  ble.setConnectCallback(connected);
  ble.setDisconnectCallback(disconnected);
  ble.setBleUartRxCallback(BleUartRX);

  /* Only one BLE GATT function should be set, it is possible to set it
  multiple times for multiple Chars ID  */
  ble.setBleGattRxCallback(charid_string, BleGattRX);
  ble.setBleGattRxCallback(charid_number, BleGattRX);

 while(1){

   Serial.println("ble task");
//     xSemaphoreTake(spiMutex, portMAX_DELAY); // Suspend this task indefinitely until the mutex is available
//     xSemaphoreGive(spiMutex);
  ble.update(200);
  vTaskDelay(100);

 }
}

int ble_task::create_task(void)
{
    // Create task.
    if(xTaskCreate(task_loop, (const portCHAR *)"BLE", BLETASKSTACKSIZE, NULL,
                   tskIDLE_PRIORITY + PRIORITY_BLE_TASK, NULL) != pdTRUE)
    {
        return(1);
    }

    // Success.
    return(0);
}
