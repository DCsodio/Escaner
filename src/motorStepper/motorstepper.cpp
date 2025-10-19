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
						 enable(portEnable,pinEnable,SALIDA,1)
{
	// TODO Auto-generated constructor stub

}


