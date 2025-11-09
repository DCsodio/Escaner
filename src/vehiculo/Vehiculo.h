/*
 * Vehiculo.h
 *
 *  Created on: 5 jul. 2025
 *      Author: Ezequiel
 */

#ifndef VEHICULO_VEHICULO_H_
#define VEHICULO_VEHICULO_H_

#include "tipos.h"
#include "control_vehiculo/ControlVehiculo.h"

class Vehiculo {
private:
       uint32_t diametroRuedaCm;
       uint32_t largoCm;
       uint32_t anchoCm;
       uint32_t alturaCm;
       uint32_t posX;
       uint32_t posY;

       ControlVehiculo control;

   public:

	   Vehiculo(uint32_t _diametroRuedaCm, uint32_t _posX, uint32_t _posY,
				  uint32_t _largo,uint32_t _ancho,uint32_t _altura,
				   uint32_t portA1, uint32_t pinA1,
					uint32_t portA2, uint32_t pinA2,
					uint32_t portB1, uint32_t pinB1,
					uint32_t portB2, uint32_t pinB2,
					uint32_t _anchoPulso=0, uint32_t _periodo=100);

       //esto no cambia no necesita setters
       uint32_t getLargoCm(void);
       uint32_t getAnchoCm(void);
       uint32_t getAlturaCm(void);
       //esto si puede cambiar
       uint32_t getPosY(void);
       uint32_t getPosX(void);
       uint32_t getDiametroRuedaCm(void);
       void SetPosY(uint32_t _posY);
       void SetPosX(uint32_t _posX);
       void SetDiametroRuedaCm(uint32_t _nuevoDiametro);

       void adelante();
       void atras();
       void izquierda();
       void derecha();
       void detener();
       void cambiarVelocidad(uint8_t);


};

#endif /* VEHICULO_VEHICULO_H_ */
