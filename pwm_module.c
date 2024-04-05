/*
 * pwm_module.c
 *
 *  Created on: 5 abr 2024
 *      Author: JUARELU1
 */

/// \cond
#include <stdbool.h>
#include <stdint.h>
/// \endcond
#include "driverlib/pwm.h"
#include "driverlib/sysctl.h"
#include "tm4c_utils/pins.h"
#include "tm4c_utils/pwm_module.h"

#define TARGET_IS_BLIZZARD_RB1
#include "driverlib/rom.h"
#include "driverlib/rom_map.h"

static void configDutyCycle(pwm_module_t* pwm_module, float duty_cycle_percentage) {
    if (duty_cycle_percentage != pwm_module->duty_cycle_percentage) {
        pwm_module->duty_cycle_percentage = duty_cycle_percentage;
        pwm_module->width_clock_units = pwm_module->period_clock_units * (duty_cycle_percentage / 100);
        if (pwm_module->is_enable) {
            if (pwm_module->is_ROM) {
                ROM_PWMGenDisable(pwm_module->hw_base, pwm_module->pwm_gen);
                ROM_PWMGenConfigure(pwm_module->hw_base, pwm_module->pwm_gen, pwm_module->pwm_gen_mode);
                ROM_PWMGenPeriodSet(pwm_module->hw_base, pwm_module->pwm_gen, pwm_module->period_clock_units);
                ROM_PWMPulseWidthSet(pwm_module->hw_base, pwm_module->pwm_out, pwm_module->width_clock_units - 1);
                ROM_PWMGenEnable(pwm_module->hw_base, pwm_module->pwm_gen);
                ROM_PWMOutputState(pwm_module->hw_base, pwm_module->pwm_out_bit, true);
            } else {
                PWMGenDisable(pwm_module->hw_base, pwm_module->pwm_gen);
                PWMGenConfigure(pwm_module->hw_base, pwm_module->pwm_gen, pwm_module->pwm_gen_mode);
                PWMGenPeriodSet(pwm_module->hw_base, pwm_module->pwm_gen, pwm_module->period_clock_units);
                PWMPulseWidthSet(pwm_module->hw_base, pwm_module->pwm_out, pwm_module->width_clock_units - 1);
                PWMGenEnable(pwm_module->hw_base, pwm_module->pwm_gen);
                PWMOutputState(pwm_module->hw_base, pwm_module->pwm_out_bit, true);
            }
        } else {
            if (pwm_module->is_ROM) {
                ROM_PWMGenDisable(pwm_module->hw_base, pwm_module->pwm_gen);
                ROM_PWMGenConfigure(pwm_module->hw_base, pwm_module->pwm_gen, pwm_module->pwm_gen_mode);
                ROM_PWMGenPeriodSet(pwm_module->hw_base, pwm_module->pwm_gen, pwm_module->period_clock_units);
                ROM_PWMPulseWidthSet(pwm_module->hw_base, pwm_module->pwm_out, pwm_module->width_clock_units - 1);
            } else {
                PWMGenDisable(pwm_module->hw_base, pwm_module->pwm_gen);
                PWMGenConfigure(pwm_module->hw_base, pwm_module->pwm_gen, pwm_module->pwm_gen_mode);
                PWMGenPeriodSet(pwm_module->hw_base, pwm_module->pwm_gen, pwm_module->period_clock_units);
                PWMPulseWidthSet(pwm_module->hw_base, pwm_module->pwm_out, pwm_module->width_clock_units - 1);
            }
        }
    }
}

static void enable(pwm_module_t* pwm_module) {
    if (pwm_module->is_ROM && !pwm_module->is_enable) {
        ROM_PWMGenEnable(pwm_module->hw_base, pwm_module->pwm_gen);
        ROM_PWMOutputState(pwm_module->hw_base, pwm_module->pwm_out_bit, true);

    } else if (!pwm_module->is_ROM && !pwm_module->is_enable) {
        PWMGenEnable(pwm_module->hw_base, pwm_module->pwm_gen);
        PWMOutputState(pwm_module->hw_base, pwm_module->pwm_out_bit, true);
    }
    pwm_module->is_enable = true;
}

static void disable(pwm_module_t* pwm_module) {
    if (pwm_module->is_ROM) {
        ROM_PWMGenDisable(pwm_module->hw_base, pwm_module->pwm_gen);
        ROM_PWMOutputState(pwm_module->hw_base, pwm_module->pwm_out_bit, false);

    } else {
        PWMGenDisable(pwm_module->hw_base, pwm_module->pwm_gen);
        PWMOutputState(pwm_module->hw_base, pwm_module->pwm_out_bit, false);
    }
    pwm_module->is_enable = false;
}

static void init(pwm_module_t* pwm_module) {
    pwm_module->is_enable = true;
    initIOPinPWM(pwm_module->pwm_pin, pwm_module->sysctl_periph_pwm, pwm_module->pin_mapping,
                 pwm_module->sysctl_pwmdiv);
    pwm_module->configDutyCycle(pwm_module, pwm_module->duty_cycle_percentage);
}

void pwmModuleSetup(pwm_module_t* pwm_module, uint32_t frequency_hz) {

    // Functions Assignment
    pwm_module->enable = &enable;
    pwm_module->disable = &disable;
    pwm_module->configDutyCycle = &configDutyCycle;
    pwm_module->init = &init;

    // Values Assignment
    pwm_module->frequency_hz = frequency_hz;
    pwm_module->duty_cycle_percentage = 0;

    pwm_module->period_clock_units = SysCtlClockGet() / pwm_module->frequency_hz;

    // Init the PWM module using the received configuration parameters
    pwm_module->init(pwm_module);
}
