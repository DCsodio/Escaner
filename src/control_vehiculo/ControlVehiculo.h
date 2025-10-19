/*
 * ControlVehiculo.h
 *
 *  Created on: 5 jul. 2025
 *      Author: Ezequiel
 */

#ifndef CONTROL_VEHICULO_CONTROLVEHICULO_H_
#define CONTROL_VEHICULO_CONTROLVEHICULO_H_

#include "tipos.h"
#include "GpioPwm.h"

class ControlVehiculo {
private:
	GpioPwm motorA1;GpioPwm motorA2;
	GpioPwm motorB1;GpioPwm motorB2;
public:
ControlVehiculo(uint32_t portA1, uint32_t pinA1,
		uint32_t portA2, uint32_t pinA2,
		uint32_t portB1, uint32_t pinB1,
		uint32_t portB2, uint32_t pinB2,
		uint32_t _anchoPulso=0, uint32_t _periodo=100);

void adelante(void);
void atras(void);
void izquierda(void);
void derecha(void);
void detener(void);

};

#endif /* CONTROL_VEHICULO_CONTROLVEHICULO_H_ */
