/*
 * brujulaHl.h
 *
 *  Created on: 18 oct. 2025
 *      Author: eze
 */

#ifndef HMC5883L_HL_BRUJULAHL_H_
#define HMC5883L_HL_BRUJULAHL_H_

#include "hmc5883l.h"

class brujulaHl {
private:
	HMC5883L_LPC845 mag;
public:
	brujulaHl();
	void inicializar(hmc5883MagGain ganancia);
	void Calibrar(float xOf, float yOf, float sx, float sy);
	float getGrados(void);
};

#endif /* HMC5883L_HL_BRUJULAHL_H_ */
