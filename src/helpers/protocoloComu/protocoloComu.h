/*
 * protocoloComu.h
 *
 *  Created on: 5 oct. 2025
 *      Author: Eze
 */

#ifndef DRIVER_PROTOCOLOCOMU_PROTOCOLOCOMU_H_
#define DRIVER_PROTOCOLOCOMU_PROTOCOLOCOMU_H_

#include "Uart0.h"
#include "tipos.h"

#define MAX_DATOS 50

#define ERROR_DESBORDE 1
#define ESTABLE 0

class protocoloComu{
private:
int8_t datos[MAX_DATOS];
uint32_t indice=0;
int8_t caracterFinal;
bool nuevosDatos=false;

public:

	protocoloComu(uint8_t _caracterFinal);
	void cambiarCaracterFinal(uint8_t nuevoCaracter);
	void enviar(const char* _datos, uint32_t tamanio=0);
	bool leerDatos(void);
	int8_t* getDatos(void);
	bool nuevoMensaje(void);


};

#endif /* DRIVER_PROTOCOLOCOMU_PROTOCOLOCOMU_H_ */
