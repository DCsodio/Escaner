/*
 * brujulaHl.cpp
 *
 *  Created on: 18 oct. 2025
 *      Author: eze
 */

#include "brujulaHl.h"

brujulaHl::brujulaHl() {
	// TODO Auto-generated constructor stub

}

void brujulaHl::inicializar(hmc5883MagGain ganancia){
	while (!mag.begin(ganancia)){
		for(int i=0; i<100; i++);//aca podemos enviar algo que se esta inicializando la brujula
	}

}

float brujulaHl::getGrados(void){
	return mag.getHeading();
}
