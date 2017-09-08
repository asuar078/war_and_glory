/*
 * timer.h
 *
 *  Created on: Sep 12, 2016
 *      Author: asuarez
 */

#ifndef DRIVERS_PERIPHERALS_TIMER_H_
#define DRIVERS_PERIPHERALS_TIMER_H_

#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"
#include "inc/hw_nvic.h"
#include "inc/hw_types.h"
#include "driverlib/interrupt.h"
#include "driverlib/pin_map.h"
#include "driverlib/rom.h"
#include "driverlib/sysctl.h"
#include "driverlib/timer.h"


/*
  Interrupt handler for the timer
*/
void SysTickInt(void);

/*
  Timer setup
*/
void TimerBegin();

/*
  This is the delay function.
*/
void Wait (uint32_t tempo) ;

uint32_t get_millis(void);

void clear_millis(void);

#endif /* DRIVERS_PERIPHERALS_TIMER_H_ */
