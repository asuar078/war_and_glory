/*
 * timer.c
 *
 *  Created on: Sep 12, 2016
 *      Author: asuarez
 */

#include <drivers/peripherals/timer.h>

volatile uint32_t millis = 0;

uint32_t get_millis(void){
    return millis;
}

void clear_millis(void){
    millis = 0;
}

/*
  Interrupt handler for the timer
*/
void SysTickInt(void)
{
  uint32_t status=0;

  status = TimerIntStatus(TIMER5_BASE,true);
  TimerIntClear(TIMER5_BASE,status);
  millis++;
}

/*
  Timer setup
*/
void TimerBegin(){

  //We set the load value so the timer interrupts each 1ms
  uint32_t Period;
  //Period = 80000; //1ms
  Period = 40000; //0.5ms

  SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER5);
  SysCtlDelay(3);
  /*
    Configure the timer as periodic, by omission it's in count down mode.
    It counts from the load value to 0 and then resets back to the load value.
  REMEMBER: You need to configure the timer before setting the load and match
  */
  TimerConfigure(TIMER5_BASE, TIMER_CFG_PERIODIC);
  TimerLoadSet(TIMER5_BASE, TIMER_A, Period -1);

  TimerIntRegister(TIMER5_BASE, TIMER_A, SysTickInt);

  /*
    Enable the timeout interrupt. In count down mode it's when the timer reaches
  0 and resets back to load. In count up mode it's when the timer reaches load
  and resets back to 0.
  */
  TimerIntEnable(TIMER5_BASE, TIMER_TIMA_TIMEOUT);

  TimerEnable(TIMER5_BASE, TIMER_A);
}

/*
  This is the delay function.
*/
void Wait (uint32_t tempo) {
  volatile uint32_t temp = millis;
  while ( (millis-temp) < tempo);
}
