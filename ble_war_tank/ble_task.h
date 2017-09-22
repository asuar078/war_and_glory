/*
 * ble_task.h
 *
 *  Created on: Sep 18, 2017
 *      Author: bigbywolf
 */

#ifndef TASK_BLE_TASK_H_
#define TASK_BLE_TASK_H_

#include <Arduino.h>
#include <SPI.h>

#include "Adafruit_BLE.h"
#include "Adafruit_BluefruitLE_SPI.h"
#include "Adafruit_BluefruitLE_UART.h"

#include "BluefruitConfig.h"

class ble_task {
public:
  ble_task(void);
  int create_task(void);
};


#endif /* TASK_BLE_TASK_H_ */
