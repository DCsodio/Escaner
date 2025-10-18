/*
 * GPIO.h
 *
 *  Created on: 27 may. 2025
 *      Author: Ezequiel
 */

#ifndef GPIO_DRIVER_GPIO_H_
#define GPIO_DRIVER_GPIO_H_

#include "tipos.h"
#include "LPC845.h"

#define HIGH 1
#define LOW 0

class Gpio {

private:
	uint8_t port;
    uint8_t pin;
    uint8_t modo;

public:
	Gpio(uint8_t port, uint8_t pin, uint8_t dir, uint8_t state_init);

	void Set(uint8_t state);
	void Dir(uint8_t mode);
	uint32_t Get();
};

#endif /* GPIO_DRIVER_GPIO_H_ */
