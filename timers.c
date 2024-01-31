/*
 * timers.c
 *
 *  Created on: 14 dic 2023
 *      Author: JUARELU1
 */


#include <stdint.h>
#include <stdbool.h>
#include <tm4c_utils/pins.h>
#include <tm4c_utils/timers.h>
#include "driverlib/interrupt.h"
#include "driverlib/timer.h"
#include "inc/hw_memmap.h"
#include "driverlib/sysctl.h"
#include "libs/heparine_pump.h"



void devTimerIntClear(timer_module_t* timer){
    TimerIntClear(timer->hw_timer_base, timer->timer_int_mode);
}

void timerDisable(timer_module_t* timer){
    TimerIntClear(timer->hw_timer_base, timer->timer_int_mode);
    TimerIntDisable(timer->hw_timer_base, timer->timer_int_mode);
    TimerDisable(timer->hw_timer_base, timer->timer_name);
}

void timerEnable(timer_module_t* timer){
    TimerIntDisable(timer->hw_timer_base, timer->timer_int_mode);
    TimerDisable(timer->hw_timer_base, timer->timer_name);
    /* Calculate the period value based on clock frequency*/
    uint32_t calculated_period = (SysCtlClockGet()/timer->frequency_hz)-1;
    TimerLoadSet(timer->hw_timer_base, timer->timer_name, calculated_period);

    TimerIntEnable(timer->hw_timer_base, timer->timer_int_mode);
    TimerEnable(timer->hw_timer_base, timer->timer_name);
}

void timerSetup( timer_module_t* timer ){
    /* Enable timer0 peripheral if isn't enabled yet*/
    if (!SysCtlPeripheralReady( timer->sysctl_peripheral )){
        SysCtlPeripheralEnable( timer->sysctl_peripheral );
        while(!SysCtlPeripheralReady( timer->sysctl_peripheral ))
        {}
    }
    /* Config timer0 in periodic mode*/
    TimerConfigure(timer->hw_timer_base, timer->timer_configure);
    /* Prescaler is 0 for timer0, do not divide the frequency*/
    TimerPrescaleSet(timer->hw_timer_base, timer->timer_name, timer->prescaler);
    /* Register the interruption handler function in the dynamic vector table*/
    TimerIntRegister(timer->hw_timer_base, timer->timer_name, timer->isrFunction);
    /* Disable the timer interruption*/
    TimerIntDisable(timer->hw_timer_base, timer->timer_int_mode);
    /* Disable the timer*/
    TimerDisable(timer->hw_timer_base, timer->timer_name);
}




