/*
 * actuator.h
 *
 *  Created on: 27 mar 2024
 *      Author: JUARELU1
 */

#ifndef SRC_TM4C_UTILS_ACTUATOR_H_
#define SRC_TM4C_UTILS_ACTUATOR_H_

/* Standard library files should be excluded for DOXYGEN Documentation*/
/// \cond
#include <stdbool.h>
#include <stdint.h>
/// \endcond

#include "tm4c_utils/pins.h"

typedef struct actuator actuator_t;

struct actuator {
    uint8_t status;
    pin_t* gpio;
    void (*turnOn)(actuator_t*);
    void (*turnOff)(actuator_t*);
};

void setupActuator(actuator_t* actuator);

#endif /* SRC_TM4C_UTILS_ACTUATOR_H_ */
