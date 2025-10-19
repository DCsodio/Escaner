/*
 * protocoloPaquete.h
 *
 *  Created on: 18 oct. 2025
 *      Author: eze
 */

#ifndef HELPERS_PROTOCOLOPAQUETE_PROTOCOLOPAQUETE_H_
#define HELPERS_PROTOCOLOPAQUETE_PROTOCOLOPAQUETE_H_

#include "tipos.h"

#pragma pack(push, 1)
typedef struct {
	uint8_t header[2];
	bool analizando;
	uint16_t distanciaMm;
	float grados;
	uint32_t posX;
	uint32_t posY;
	uint32_t checksum;
} Paquete;
#pragma pack(pop)

uint32_t calcularChecksum(Paquete* pkt);

#endif /* HELPERS_PROTOCOLOPAQUETE_PROTOCOLOPAQUETE_H_ */
