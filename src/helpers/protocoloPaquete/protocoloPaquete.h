/*
 * protocoloPaquete.h
 *
 *  Created on: 18 oct. 2025
 *      Author: eze
 */

#ifndef HELPERS_PROTOCOLOPAQUETE_PROTOCOLOPAQUETE_H_
#define HELPERS_PROTOCOLOPAQUETE_PROTOCOLOPAQUETE_H_

#include "tipos.h"

//IEEE754 little-endian.
#pragma pack(push, 1)
typedef struct {
	uint8_t header[2];
	uint8_t analizando;
	uint16_t distanciaMm;
	float grados;
	uint16_t cantidadPasos;
	uint32_t pulsos;
	bool direccionAdelante;
	uint32_t checksum;
} Paquete;
#pragma pack(pop)

uint32_t calcularChecksum(volatile Paquete* pkt);

#endif /* HELPERS_PROTOCOLOPAQUETE_PROTOCOLOPAQUETE_H_ */
