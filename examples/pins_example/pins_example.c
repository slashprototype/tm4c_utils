#include <src/tm4c_utils/pins.h>
#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"
#include "driverlib/timer.h"
#include "inc/hw_memmap.h"

pin_t PF2;

void initGPIOs(void) {
    initIOPin(0, &PF2, GPIO_PORTF_BASE, GPIO_PIN_2, SYSCTL_PERIPH_GPIOF);
}

void main() {
    //MCU Clock Configuration for 16MHZ
    SysCtlClockSet(SYSCTL_SYSDIV_1 | SYSCTL_USE_OSC | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ);
    initGPIOs();
    while (true) {
        SysCtlDelay(SysCtlClockGet() / 10);
        togglePin(&PF2);
    };
}
