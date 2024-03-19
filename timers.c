/*
 * timers.c
 *
 *  Created on: 14 dic 2023
 *      Author: JUARELU1
 */

#include <src/tm4c_utils/pins.h>
#include <src/tm4c_utils/timers.h>
#include <stdbool.h>
#include <stdint.h>
#include "driverlib/interrupt.h"
#include "driverlib/sysctl.h"
#include "driverlib/timer.h"
#include "inc/hw_memmap.h"

static void disable(timer_module_t* timer) {
    TimerIntClear(timer->hw_timer_base, timer->timer_int_mode);
    TimerIntDisable(timer->hw_timer_base, timer->timer_int_mode);
    TimerDisable(timer->hw_timer_base, timer->timer_name);
    timer->is_enabled = 0;
}

static void enable(timer_module_t* timer) {
    TimerIntClear(timer->hw_timer_base, timer->timer_int_mode);
    TimerIntEnable(timer->hw_timer_base, timer->timer_int_mode);
    TimerEnable(timer->hw_timer_base, timer->timer_name);
    timer->is_enabled = 1;
}

static void configureFrequency(timer_module_t* timer, uint32_t frequency_hz) {
    if (frequency_hz == 0) {
        return;
    }
    uint32_t calculated_period = (SysCtlClockGet() / frequency_hz) - 1;
    timer->frequency_hz = frequency_hz;
    if (timer->is_enabled) {
        TimerIntDisable(timer->hw_timer_base, timer->timer_int_mode);
        TimerDisable(timer->hw_timer_base, timer->timer_name);
        /* Calculate the period value based on clock frequency*/
        TimerLoadSet(timer->hw_timer_base, timer->timer_name, calculated_period);
        TimerIntEnable(timer->hw_timer_base, timer->timer_int_mode);
        TimerEnable(timer->hw_timer_base, timer->timer_name);
    } else {
        /* Calculate the period value based on clock frequency*/
        TimerLoadSet(timer->hw_timer_base, timer->timer_name, calculated_period);
    }
}

static void setup(timer_module_t* timer) {
    /* Enable timer0 peripheral if isn't enabled yet*/
    if (!SysCtlPeripheralReady(timer->sysctl_peripheral)) {
        SysCtlPeripheralEnable(timer->sysctl_peripheral);
        while (!SysCtlPeripheralReady(timer->sysctl_peripheral)) {}
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

    IntEnable(timer->hw_timer_int);
    timer->is_enabled = 0;
}

void setupTimerModule(timer_module_t* timer) {
    timer->enable = &enable;
    timer->disable = &disable;
    timer->configureFrequency = &configureFrequency;
    timer->setup = &setup;
    /* Setup timer */
    timer->setup(timer);
}
