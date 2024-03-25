/*
 * timers.h
 *
 *  Created on: 14 dic 2023
 *      Author: JUARELU1
 */

#ifndef SRC_TM4C_UTILS_TIMERS_H_
#define SRC_TM4C_UTILS_TIMERS_H_

/* Standard library files should be excluded for DOXYGEN Documentation*/
/// \cond
#include <stdbool.h>
#include <stdint.h>
/// \endcond

typedef struct timer_module timer_module_t;

struct timer_module {
    uint32_t hw_timer_base;     // timer hw_memmap.h reference
    uint32_t hw_timer_int;      // interruption assignment
    uint32_t sysctl_peripheral; // sysctl.h timer x reference
    uint32_t timer_configure;   // timer.h configuration parameters
    uint32_t timer_name;        // timer.h Timer A, B, Both, etc.
    uint32_t timer_int_mode;    // timer.h interruption mode;
    uint32_t frequency_hz;      // value to configure timer period
    uint8_t prescaler;          // value to prescaler frequency
    uint32_t counter;           // User variable, can be apply to increase every ISR handler
    uint8_t is_enabled;
    void (*isrFunction)(void); // pointer to ISR handler function, assigned by user case application
    void (*intClear)(void);
    void (*enable)(timer_module_t*);
    void (*disable)(timer_module_t*);
    void (*configureFrequency)(timer_module_t*, uint32_t);
    void (*setup)(timer_module_t*);
};

/* Setup the timer0*/
void setupTimerModule(timer_module_t* timer);

#endif /* SRC_TM4C_UTILS_TIMERS_H_ */
