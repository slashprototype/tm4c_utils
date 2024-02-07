#include "timers.h"
#include "driverlib/timer.h"
#include "inc/hw_memmap.h"
#include "driverlib/sysctl.h"

timer_module_t timer0;
timer_module_legacy_functions_t timer0_legacy_functions;

void timer0IsrHandler(void){
	timer0.legacy_functions->timerIntClear(&timer0);
}

void initTimer0 (void) {
    /* TIMER CONFIGURATION */
    timer0.hw_timer_base = TIMER0_BASE;
    timer0.sysctl_peripheral = SYSCTL_PERIPH_TIMER0;
    timer0.timer_configure = TIMER_CFG_PERIODIC;
    timer0.timer_name = TIMER_A;
    timer0.timer_int_mode = TIMER_TIMA_TIMEOUT;
    timer0.frequency_hz = 1000;     // timer frecuency selected, e.g. 1000 = 0.001 s = 1ms
    timer0.prescaler = 0;
    timer0.counter = 0;
    timer0.isrFunction = &timer0IsrHandler;

    timer0_legacy_functions.timerSetup = &timerSetup;
    timer0_legacy_functions.timerDisable = &timerDisable;
    timer0_legacy_functions.timerEnable = &timerEnable;
	timer0_legacy_functions.timerIntClear = &devTimerIntClear;
    timer0.legacy_functions = &timer0_legacy_functions;

    timer0.legacy_functions->timerSetup(&timer0);
    timer0.legacy_functions->timerEnable(&timer0);
}

void main (){
	//MCU Clock Configuration
	initTimer0();
	//Master interruptions should be enables as well
	while(true){};
}

