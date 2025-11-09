/*
 * laserHl.cpp
 *
 *  Created on: 26 sep. 2025
 *      Author: Eze
 */

#include "laserHl.h"

/*
 * verifica el estado del sensor hasta que se logre conecetar
 */
laserHl::laserHl() {
	/*
	do {
		status = VL53L1X_BootState(VL53L1X_I2C_ADDR, &sensorState);
	}while (!sensorState);

	// Inicializar sensor
	status = VL53L1X_SensorInit(VL53L1X_I2C_ADDR);
	if (status != 0) {
		while (1); // Error en init, quedarse bloqueado
	}*/
}

/*
 * inicia el laser
 */




void laserHl::iniciarLaser(void){
	do {
			status = VL53L1X_BootState(VL53L1X_I2C_ADDR, &sensorState);
		}while (!sensorState);

	for(int i=0; i<500; i++){
	}
		// Inicializar sensor
	status = VL53L1X_SensorInit(VL53L1X_I2C_ADDR);
	if (status != 0) {
		while (1); // Error en init, quedarse bloqueado
	}
}

/*
 * rango: 1 corto alcance, 2 largo alcance
 * budget: 	15- 20-33ms alta velocidad corto alncande
 * 			50-100ms balance velocidad/precision
 * 			200 - 500ms	maxima presicion, largo alcance
 * periodo: debe ser igual o mayor que budget
 * 2,50,100 por defecto
 */
void laserHl::configurarLaser(uint32_t rango, uint32_t budget, uint32_t periodo){
	VL53L1X_SetDistanceMode(VL53L1X_I2C_ADDR, rango);        // 2 = largo alcance
	VL53L1X_SetTimingBudgetInMs(VL53L1X_I2C_ADDR, budget);   // 50ms budget
	VL53L1X_SetInterMeasurementInMs(VL53L1X_I2C_ADDR,periodo);
	VL53L1X_StartRanging(VL53L1X_I2C_ADDR);
}

/*
 * Distancia leida colocada en distanciaMm
 */
void laserHl::leerDistanciaMm(void){
	dataReady=0;
	while (dataReady == 0) {
		VL53L1X_CheckForDataReady(VL53L1X_I2C_ADDR, &dataReady);
	}
	VL53L1X_GetDistance(VL53L1X_I2C_ADDR, (uint16_t *)&distanciaMm);
	        // Limpiar la interrupción del sensor
	VL53L1X_ClearInterrupt(VL53L1X_I2C_ADDR);
}
/*
 * Devuelve distancia en milimetros
 */
uint16_t laserHl::getDistanciaMm(void){
	leerDistanciaMm();
	return distanciaMm;
}
