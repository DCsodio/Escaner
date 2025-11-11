/*
 * motorstepper.h
 *
 *  Created on: 18 oct. 2025
 *      Author: Eze
 */

#pragma once

#include "Gpio.h"

#define CANTPASOS180 180
#define CANTPASOS360 360

class motorstepper {
private:

Gpio direccion;
Gpio paso;
Gpio enable;

uint32_t cantidadPasos;

public:
	motorstepper(uint32_t portDir,uint32_t pinDir,
			uint32_t portPaso, uint32_t pinPaso,
			uint32_t portEnable, uint32_t pinEnable);
	void setDireccion(bool _direccion);
	void setEnable(bool _enable);
	void pulso();
	void moverPaso(bool sentido);
	uint32_t getCantidadPasos() const;
	void setCantidadPasos(uint32_t cantidadPasos);
};
