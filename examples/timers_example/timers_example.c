#include <src/tm4c_utils/timers.h>
#include "driverlib/sysctl.h"
#include "driverlib/timer.h"
#include "inc/hw_memmap.h"
timer_module_t timer0;

void timer0IsrHandler(void) {
    timer0.intClear(&timer0);
}

void initTimer0(void) {
    /* TIMER CONFIGURATION */
    timer0.hw_timer_base = TIMER0_BASE;
    timer0.sysctl_peripheral = SYSCTL_PERIPH_TIMER0;
    timer0.timer_configure = TIMER_CFG_PERIODIC;
    timer0.timer_name = TIMER_A;
    timer0.timer_int_mode = TIMER_TIMA_TIMEOUT;
    timer0.frequency_hz = 1000; // timer frecuency selected, e.g. 1000 = 0.001 s = 1ms
    timer0.prescaler = 0;
    timer0.counter = 0;
    timer0.isrFunction = &timer0IsrHandler;
}

void main() {
    //MCU Clock Configuration
    initTimer0();
    //Master interruptions should be enables as well
    while (true) {};
}
