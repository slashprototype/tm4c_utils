/*
 * pins.c
 *
 *  Created on: 13 dic 2023
 *      Author: JUARELU1
 */


#include "utils/pins.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"

void initIOPin(bool is_input, pin_t *pin, uint32_t gpio_port, uint32_t gpio_pin, uint32_t sysctl_periph){
    pin->GPIO_PIN = gpio_pin;
    pin->GPIO_PORT = gpio_port;
    pin->SYSCTL_PERIPH = sysctl_periph;
    pin->is_input = is_input;
    setupIOPin(pin, is_input);
}

void initIOPinPWM(pin_t *pin, uint32_t pwm_sysctl_periph, uint32_t pin_pwm_map, uint32_t pwm_sysctl_div){
    /* Enable GPIO PORT Base Periph */
    if (!SysCtlPeripheralReady(pin->SYSCTL_PERIPH)){
        SysCtlPeripheralEnable(pin->SYSCTL_PERIPH);
        while(!SysCtlPeripheralReady(pin->SYSCTL_PERIPH)){
        }
    }
    /* Enable pwm sysctl peripheral*/
    if (!SysCtlPeripheralReady(pwm_sysctl_periph)){
        SysCtlPeripheralEnable(pwm_sysctl_periph);
        while(!SysCtlPeripheralReady(pwm_sysctl_periph)){
        }
    }
    GPIOPinTypeGPIOOutput(pin->GPIO_PORT, pin->GPIO_PIN);
    GPIOPinTypePWM(pin->GPIO_PORT, pin->GPIO_PIN);
    GPIOPinConfigure(pin_pwm_map);
    SysCtlPWMClockSet(pwm_sysctl_div);
}

void setupIOPin(pin_t *pin, bool is_input){
    /* Enable the peripheral clock connection for GPIO purpose*/
    if (!SysCtlPeripheralReady(pin->SYSCTL_PERIPH)){
        SysCtlPeripheralEnable(pin->SYSCTL_PERIPH);
        while(!SysCtlPeripheralReady(pin->SYSCTL_PERIPH)){
        }
    }
    /* Configure pin as input or output, includes dirModeSet and default strength*/
    if(is_input){
        GPIOPinTypeGPIOInput(pin->GPIO_PORT, pin->GPIO_PIN);
        /*TODO: Implement default and non-default settings*/
        GPIOPadConfigSet(pin->GPIO_PORT, pin->GPIO_PIN, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);
    }
    /* Initialize the output in Low state (OFF)*/
    else{
        GPIOPinTypeGPIOOutput(pin->GPIO_PORT, pin->GPIO_PIN);
        GPIOPinWrite(pin->GPIO_PORT, pin->GPIO_PORT, 0);
    }
}

void pinWrite(pin_t *pin, uint8_t value){
    if (pin->is_input == true){
        while(true){
            /* You shouldn't write to a pin which is not an output >:D*/
        }
    }
    else{
        /* Turn OFF (Low state) the selected GPIO*/
        if ( value == 0 ){
            GPIOPinWrite(pin->GPIO_PORT, pin->GPIO_PIN, 0);
        }
        /* Turn ON (High state) the selected GPIO*/
        else{
            GPIOPinWrite(pin->GPIO_PORT, pin->GPIO_PIN, pin->GPIO_PIN);
        }
    }
}

uint8_t pinRead(pin_t *pin){
    if (GPIOPinRead(pin->GPIO_PORT, pin->GPIO_PIN) > 0){
        return 1;
    }
    else{
        return 0;
    }
}

void togglePin(pin_t *pin){
    if (pin->is_input){}
    else{
        if (pinRead(pin) == 0){
            pinWrite(pin, 1);
        }
        else{
            pinWrite(pin, 0);
        }
    }
}