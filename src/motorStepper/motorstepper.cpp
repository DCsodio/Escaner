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
	setEnable(false);
}

void motorstepper::setDireccion(bool _direccion){
	direccion.Set(_direccion);
}
void motorstepper::setEnable(bool _enable){
	enable.Set(_enable);
	//for(int i=0; i<5000; i++);
}
void motorstepper::moverPaso(){
	paso.Set(1);
	for(int i=0; i<200; i++);
	paso.Set(0);
}

