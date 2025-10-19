/*
 * protocoloPaquete.cpp
 *
 *  Created on: 18 oct. 2025
 *      Author: eze
 */

#include "protocoloPaquete.h"

uint32_t calcularChecksum(Paquete* pkt) {
    uint32_t sum = 0;

    for (uint32_t i = 0; i < sizeof(Paquete) - sizeof(pkt->checksum); i++) {
        sum += ((uint8_t*)pkt)[i];  // convertimos pkt a bytes directamente
    }

    return sum;
}
