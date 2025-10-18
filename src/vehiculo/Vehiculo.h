/*
 * Vehiculo.h
 *
 *  Created on: 5 jul. 2025
 *      Author: Ezequiel
 */

#ifndef VEHICULO_VEHICULO_H_
#define VEHICULO_VEHICULO_H_

#include "tipos.h"
#include "ControlVehiculo.h"
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

       Vehiculo(   uint32_t _diametroRuedaCm, uint32_t _posX, uint32_t _posY,
                   uint32_t _largo,uint32_t _ancho,uint32_t _altura);

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

       void adelante(uint32_t distanciaCm);
       void atras(uint32_t distanciaCm);
       void izquierda(uint32_t angulo);
       void derecha(uint32_t angulo);


};

#endif /* VEHICULO_VEHICULO_H_ */
