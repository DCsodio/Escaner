/*
 * ControlVehiculo.cpp
 *
 *  Created on: 5 jul. 2025
 *      Author: Ezequiel
 */


#include "ControlVehiculo.h"

ControlVehiculo::ControlVehiculo(uint32_t portA1, uint32_t pinA1,
		uint32_t portA2, uint32_t pinA2,
		uint32_t portB1, uint32_t pinB1,
		uint32_t portB2, uint32_t pinB2,
		uint32_t _anchoPulso, uint32_t _periodo)
	:motorA1(portA1,pinA1,0,100),
	 motorA2(portA2,pinA2,0,100),
	 motorB1(portB1,pinB1,0,100),
	 motorB2(portB2,pinB2,0,100)

{
	motorA1.Set(0);
	motorA2.Set(0);
	motorB1.Set(0);
	motorB2.Set(0);
}

void ControlVehiculo::adelante(){
	motorA1.Set(0);
	motorA2.Set(velPct);
	motorB1.Set(velPct);
	motorB2.Set(0);
}
void ControlVehiculo::atras(){
	motorA1.Set(velPct);
	motorA2.Set(0);
	motorB1.Set(0);
	motorB2.Set(velPct);
}
void ControlVehiculo::izquierda(){
	motorA1.Set(0);
	motorA2.Set(velPct);
	motorB1.Set(0);
	motorB2.Set(velPct);
}
void ControlVehiculo::derecha(){
	motorA1.Set(velPct);
	motorA2.Set(0);
	motorB1.Set(velPct);
	motorB2.Set(0);
}
void ControlVehiculo::detener(){
	motorA1.Set(0);
	motorA2.Set(0);
	motorB1.Set(0);
	motorB2.Set(0);
}

void ControlVehiculo::setVelPct(uint8_t _velPct){
	velPct= _velPct;
}

