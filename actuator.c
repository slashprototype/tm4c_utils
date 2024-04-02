/*
 * actuator.c
 *
 *  Created on: 27 mar 2024
 *      Author: JUARELU1
 */

#include "tm4c_utils/actuator.h"

static void turnOn(actuator_t* actuator) {
    pinWrite(actuator->gpio, 1);
    actuator->status = pinRead(actuator->gpio);
}

static void turnOff(actuator_t* actuator) {
    pinWrite(actuator->gpio, 0);
    actuator->status = pinRead(actuator->gpio);
}

void setupActuator(actuator_t* actuator) {
    actuator->turnOn = &turnOn;
    actuator->turnOff = &turnOff;
}