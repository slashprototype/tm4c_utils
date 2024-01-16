/*
 * pins.h
 *
 *  Created on: 13 dic 2023
 *      Author: JUARELU1
 */

#ifndef SRC_LIBS_PINS_H_
#define SRC_LIBS_PINS_H_

#include <stdint.h>
#include <stdbool.h>

typedef struct{
    uint32_t GPIO_PIN;
    uint32_t GPIO_PORT;
    uint32_t SYSCTL_PERIPH;
    bool is_input;

}pin_t;

void initIOPin(bool is_input, pin_t *pin, uint32_t gpio_port, uint32_t gpio_pin, uint32_t sysctl_periph);
void initIOPinPWM(pin_t *pin, uint32_t sysctl_periph, uint32_t pin_pwm_map, uint32_t pwm_sysctl_div);
void setupIOPin(pin_t *pin, bool is_input);
void pinWrite(pin_t *pin, uint8_t value);
uint8_t pinRead(pin_t *pin);
void togglePin(pin_t *pin);
#endif /* SRC_LIBS_PINS_H_ */
