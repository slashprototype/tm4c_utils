/*
 * @file        pwm_module.h
 * @brief       This file contains all the function prototypes for the pwm_module.c file
 * 
 * 
 * @project     TM4C123 GPIO Utilities
 * @date        1 Jan 2025
 * @version     1.0.0
 * @author      Slashprototype
 */


#ifndef SRC_TM4C_UTILS_PWM_MODULE_H_
#define SRC_TM4C_UTILS_PWM_MODULE_H_

/// \cond
#include <stdbool.h>
#include <stdint.h>
/// \endcond
#include "tm4c_utils/pins.h"

/* Simple PWM module handler, must be configured to enable/disable 
 * PWM Pin signals related to DC motor control
 */
typedef struct pwm_module pwm_module_t;

struct pwm_module {
    pin_t* pwm_pin;              // PWM pin from Target
    uint32_t frequency_hz;       // PWM frequency, depends on pwmdiv /1 /2 /4 /8
    uint32_t period_clock_units; // PWM frequency, depends on pwmdiv /1 /2 /4 /8
    uint32_t width_clock_units;  // Duty cycle for PWM operation
    float duty_cycle_percentage; // PWM percentage in percentage 0 - 100
    uint32_t hw_base;            //TM4C inc hw_memmap.h PWM Base
    uint32_t sysctl_periph_pwm;  //TM4C driverlib sysctl.h PWM Peripheral
    uint32_t sysctl_pwmdiv;      //TM4C driverlib sysctl.h PWM Clock div
    uint32_t pin_mapping;        //TM4C driverlib pin_map.h PWM PIN
    uint32_t pwm_gen;            //TM4C driverlib PWM lib generator
    uint32_t pwm_gen_mode;       //TM4C driverlib PWM lib generator mode
    uint32_t pwm_out;            //TM4C driverlib PWM lib PWM Out
    uint32_t pwm_out_bit;        //TM4C driverlib PWM lib PWM Out bit
    bool is_ROM;                 // Select if program should be used by Flash or ROM
    bool is_enable;
    void (*init)(pwm_module_t*);
    void (*enable)(pwm_module_t*);  // PWM enable function
    void (*disable)(pwm_module_t*); // PWM disable function
    void (*configDutyCycle)(pwm_module_t*, float);
};

void pwmModuleSetup(pwm_module_t* pwm_module, uint32_t frequency_hz);

#endif /* SRC_TM4C_UTILS_PWM_MODULE_H_ */
