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

#include "../drivers/devices/L293D.h"

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
#define MOTOR_ITEM_SIZE           sizeof(uint8_t)
#define MOTOR_QUEUE_SIZE          5

static uint32_t pwm_init(void){

    uint32_t ulPeriod;

    //Configure PWM Clock to match system
    SysCtlPWMClockSet(SYSCTL_PWMDIV_1);

    // Enable the peripherals used by this program.
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM1);  //The Tiva Launchpad has two modules (0 and 1). Module 1 covers the LED pins
    ulPeriod = SysCtlClockGet() / 500; //PWM frequency 500HZ

    //Configure PF1,PF2,PF3 Pins as PWM
//    GPIOPinConfigure(GPIO_PF1_M1PWM5);
    GPIOPinConfigure(GPIO_PF2_M1PWM6);
    GPIOPinConfigure(GPIO_PF3_M1PWM7);
    GPIOPinTypePWM(GPIO_PORTF_BASE, GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3);

    //Configure PWM Options
    //PWM_GEN_2 Covers M1PWM4 and M1PWM5
    //PWM_GEN_3 Covers M1PWM6 and M1PWM7 See page 207 4/11/13 DriverLib doc
//    PWMGenConfigure(PWM1_BASE, PWM_GEN_2, PWM_GEN_MODE_UP_DOWN | PWM_GEN_MODE_NO_SYNC);
    PWMGenConfigure(PWM1_BASE, PWM_GEN_3, PWM_GEN_MODE_UP_DOWN | PWM_GEN_MODE_NO_SYNC);

    //Set the Period (expressed in clock ticks)
//    PWMGenPeriodSet(PWM1_BASE, PWM_GEN_2, ulPeriod);
    PWMGenPeriodSet(PWM1_BASE, PWM_GEN_3, ulPeriod);

    //Set PWM duty-50% (Period /2)
//    PWMPulseWidthSet(PWM1_BASE, PWM_OUT_5,ulPeriod/2);
    PWMPulseWidthSet(PWM1_BASE, PWM_OUT_6,ulPeriod/2);
    PWMPulseWidthSet(PWM1_BASE, PWM_OUT_7,ulPeriod/2);

//    PWMPulseWidthSet(PWM1_BASE, PWM_OUT_6,0);
//    PWMPulseWidthSet(PWM1_BASE, PWM_OUT_7,0);

    // Enable the PWM generator
//    PWMGenEnable(PWM1_BASE, PWM_GEN_2);
    PWMGenEnable(PWM1_BASE, PWM_GEN_3);

    // Turn on the Output pins
//    PWMOutputState(PWM1_BASE, PWM_OUT_5_BIT |PWM_OUT_6_BIT|PWM_OUT_7_BIT, true);
    PWMOutputState(PWM1_BASE, PWM_OUT_6_BIT|PWM_OUT_7_BIT, true);

    return ulPeriod;
}

static void motor_task(void *pvParameters)
{

    uint32_t pwm_period = pwm_init();

    L293D_pins motor1 = {
         .period = pwm_period,
         .enable_pwm_base = PWM1_BASE,
         .enable_pwm_out_pin = PWM_OUT_6,

         .forward_periph = SYSCTL_PERIPH_GPIOD,
         .forward_port = GPIO_PORTD_BASE,
         .forward_pin = GPIO_PIN_2,

         .reverse_periph = SYSCTL_PERIPH_GPIOD,
         .reverse_port = GPIO_PORTD_BASE,
         .reverse_pin = GPIO_PIN_3
    };

    L293D_init(&motor1);

    // Loop forever.
    while(1)
    {
        //Drive servo to 90 degrees
        L293D_set(&motor1, 0);
        vTaskDelay( 2000 );
        L293D_set(&motor1, 0.9);
        vTaskDelay( 2000 );
        L293D_set(&motor1, -0.5);
        vTaskDelay( 2000 );
    }
}

//*****************************************************************************
//
// Initializes the motor task.
//
//*****************************************************************************
uint32_t motor_task_init(void)
{
    // Create the task.
    if(xTaskCreate(motor_task, (const portCHAR *)"Motor", MOTORTASKSTACKSIZE, NULL,
                   tskIDLE_PRIORITY + PRIORITY_MOTOR_TASK, NULL) != pdTRUE)
    {
        return(1);
    }

    // Success.
    return(0);
}
