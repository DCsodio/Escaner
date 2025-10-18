/*
 * laserHl.h
 *
 *  Created on: 26 sep. 2025
 *      Author: Eze
 */

#ifndef VL53L1X_HL_LASERHL_H_
#define VL53L1X_HL_LASERHL_H_

#include "../../inc/VL53L1/VL53L1X_api.h"
#include "tipos.h"

#define VL53L1X_I2C_ADDR  0x29

class laserHl {
private:
	uint16_t distanciaMm=0;
	uint8_t dataReady = 0;
	uint8_t sensorState = 0;
	VL53L1X_ERROR status = 0;

public:
	laserHl();
	void iniciarLaser(void);
	void configurarLaser(uint32_t rango=2, uint32_t budget=50, uint32_t periodo=100);
	void leerDistanciaMm(void);
	uint16_t getDistanciaMm(void);

};

#endif /* VL53L1X_HL_LASERHL_H_ */
