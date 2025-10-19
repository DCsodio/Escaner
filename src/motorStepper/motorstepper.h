/*
 * motorstepper.h
 *
 *  Created on: 18 oct. 2025
 *      Author: Eze
 */

#ifndef MOTORSTEPPER_MOTORSTEPPER_H_
#define MOTORSTEPPER_MOTORSTEPPER_H_
#include "Gpio.h"

class motorstepper {
private:
Gpio direccion;
Gpio paso;
Gpio enable;

public:
	motorstepper(uint32_t portDir,uint32_t pinDir,
			uint32_t portPaso, uint32_t pinPaso,
			uint32_t portEnable, uint32_t pinEnable);

};

#endif /* MOTORSTEPPER_MOTORSTEPPER_H_ */
