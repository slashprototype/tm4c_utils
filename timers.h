/*
 * timers.h
 *
 *  Created on: 14 dic 2023
 *      Author: JUARELU1
 */

#ifndef SRC_TM4C_UTILS_TIMERS_H_
#define SRC_TM4C_UTILS_TIMERS_H_

<<<<<<< HEAD
=======
/* Standard library files should be excluded for DOXYGEN Documentation*/
>>>>>>> 6ba254c23c36c9ea9d6f9414b750a2c9b899239f
/// \cond
#include <stdint.h>
#include <stdbool.h>
/// \endcond

typedef struct timer_module_legacy_functions timer_module_legacy_functions_t;

typedef struct{
    uint32_t hw_timer_base;         // timer hw_memmap.h reference
    uint32_t sysctl_peripheral;     // sysctl.h timer x reference
    uint32_t timer_configure;       // timer.h configuration parameters
    uint32_t timer_name;            // timer.h Timer A, B, Both, etc.
    uint32_t timer_int_mode;        // timer.h interruption mode;
    uint32_t frequency_hz;             // value to configure timer period
    uint8_t prescaler;              // value to prescaler frequency
    uint32_t counter;               // User variable, can be apply to increase every ISR handler
    void (*isrFunction) (void);     // pointer to ISR handler function
    timer_module_legacy_functions_t* legacy_functions;  // Default functionalities implemented

}timer_module_t;

struct timer_module_legacy_functions{
    void (*timerSetup)(timer_module_t*);
    void (*timerDisable)(timer_module_t*);
    void (*timerEnable)(timer_module_t*);
    void (*timerIntClear)(timer_module_t*);
};

/* Function to handle the timer0 ISR*/
void timer0IntHandler( void );
/* Setup the timer0*/
void timerSetup( timer_module_t* timer );
/* Enable the timer0 ISR, start the timeout ISR*/
void timerEnable( timer_module_t* timer );
/* Disable and clear the timer0 ISR caused by timeout of TimerA*/
void timerDisable( timer_module_t* timer );
/* Legacy Interruption clear, should be called at isr handler*/
void devTimerIntClear( timer_module_t* timer);


#endif /* SRC_TM4C_UTILS_TIMERS_H_ */
