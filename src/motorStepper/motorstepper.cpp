/*
 * motorstepper.cpp
 *
 *  Created on: 18 oct. 2025
 *      Author: Eze
 */

#include "motorstepper.h"

motorstepper::motorstepper(	uint32_t portDir,uint32_t pinDir,
							uint32_t portPaso, uint32_t pinPaso,
							uint32_t portEnable, uint32_t pinEnable)
						:direccion(portDir,pinDir,SALIDA,0),
						 paso(portPaso, pinPaso,SALIDA,0),
						 enable(portEnable,pinEnable,SALIDA,0)
{
}

void motorstepper::setDireccion(bool _direccion){
	direccion.Set(_direccion);
}
void motorstepper::setEnable(bool _enable){
	enable.Set(_enable);
}
void motorstepper::moverPaso(){
	paso.Set(1);
	for(uint16_t i=0; i<100; i++);
	paso.Set(0);
}

