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

#define MAX_DATOS 100

#define ERROR_DESBORDE 1
#define ESTABLE 0

class protocoloComu{
private:
uint8_t datos[MAX_DATOS];
uint32_t largoCadena;
uint32_t indice=0;
uint8_t caracterFinal;
uint8_t caracterInicial;
bool deteccionDatos;
bool nuevosDatos;

public:

	protocoloComu( uint8_t _caracterInicial, uint8_t _caracterFinal);
	void cambiarCaracterFinal(uint8_t nuevoCaracter);
	void cambiarCaracterInicial(uint8_t nuevoCaracter);
	void enviar(const char* _datos, uint32_t tamanio=0);
	bool leerDatos(void);
	uint8_t* getDatos(void);
	bool nuevoMensaje(void);
	uint32_t getLargoCadena() const;
};

#endif /* DRIVER_PROTOCOLOCOMU_PROTOCOLOCOMU_H_ */
