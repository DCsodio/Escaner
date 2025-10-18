/*
 * ControlVehiculo.h
 *
 *  Created on: 5 jul. 2025
 *      Author: Ezequiel
 */

#ifndef CONTROL_VEHICULO_CONTROLVEHICULO_H_
#define CONTROL_VEHICULO_CONTROLVEHICULO_H_

#include "tipos.h"

class ControlVehiculo {

public:
ControlVehiculo();

void adelante(uint32_t distanciaCm);
void atras(uint32_t distanciaCm);
void izquierda(uint32_t angulo);
void derecha(uint32_t angulo);

};

#endif /* CONTROL_VEHICULO_CONTROLVEHICULO_H_ */
