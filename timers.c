/*
 * @file        timers.c
 * @brief       Timer Utilities for TM4C123 Microcontroller
 * 
 * @details     This file contains utility functions for configuring and managing timers
 *              on the TM4C123 microcontroller. It provides functions to initialize timers,
 *              set frequencies, enable and disable timers, and handle timer interrupts.
 * 
 * @project     TM4C123 Timer Utilities
 * @date        1 Jan 2025
 * @version     1.0.0
 * @author      Slashprototype
 */

#include <stdbool.h>
#include <stdint.h>
#include "tm4c_utils/pins.h"
#include "tm4c_utils/timers.h"
#include "driverlib/interrupt.h"
#include "driverlib/sysctl.h"
#include "driverlib/timer.h"
#include "inc/hw_memmap.h"

static void intClear(timer_module_t* timer) {
    TimerIntClear(timer->hw_timer_base, timer->timer_int_mode);
}

static void disable(timer_module_t* timer) {
    TimerIntClear(timer->hw_timer_base, timer->timer_int_mode);
    TimerIntDisable(timer->hw_timer_base, timer->timer_int_mode);
    TimerDisable(timer->hw_timer_base, timer->timer_name);
    timer->is_enabled = 0;
}

static void enable(timer_module_t* timer) {
    if (!timer->is_enabled) {
        TimerIntClear(timer->hw_timer_base, timer->timer_int_mode);
        TimerIntEnable(timer->hw_timer_base, timer->timer_int_mode);
        TimerEnable(timer->hw_timer_base, timer->timer_name);
        timer->is_enabled = 1;
    }
}

static void configureFrequency(timer_module_t* timer, uint32_t frequency_hz) {
    if (frequency_hz == 0) {
        return;
    }
    // Calculate the period based on required frequency (hz)
    if (timer->frequency_hz != frequency_hz) {
        timer->frequency_hz = frequency_hz;
        uint32_t sys_clock = SysCtlClockGet();
        uint32_t calculated_period = (sys_clock / timer->frequency_hz);
        if (timer->is_enabled) {
            TimerIntDisable(timer->hw_timer_base, timer->timer_int_mode);
            TimerDisable(timer->hw_timer_base, timer->timer_name);
            /* Calculate the period value based on clock frequency*/
            TimerClockSourceSet(timer->hw_timer_base, TIMER_CLOCK_SYSTEM);
            TimerLoadSet(timer->hw_timer_base, timer->timer_name, calculated_period);
            /* Prescaler is 0 for timer0, do not divide the frequency*/
            TimerPrescaleSet(timer->hw_timer_base, timer->timer_name, timer->prescaler);
            TimerIntEnable(timer->hw_timer_base, timer->timer_int_mode);
            TimerEnable(timer->hw_timer_base, timer->timer_name);
        } else {
            /* Calculate the period value based on clock frequency*/
            TimerClockSourceSet(timer->hw_timer_base, TIMER_CLOCK_SYSTEM);
            TimerLoadSet(timer->hw_timer_base, timer->timer_name, calculated_period);
        }
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
    timer->frequency_hz = 1;
}

void setupTimerModule(timer_module_t* timer) {
    timer->enable = &enable;
    timer->disable = &disable;
    timer->configureFrequency = &configureFrequency;
    timer->setup = &setup;
    timer->intClear = &intClear;
    timer->is_enabled = 0;

    /* Setup timer */
    timer->setup(timer);
}
