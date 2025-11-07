/*
 * DRIVER_ADC.h
 *
 *  Created on: Jan 3, 2020
 *      Author: GJF-Trabajo
 */

#ifndef DRIVER_ADC_DRIVER_ADC_H_
#define DRIVER_ADC_DRIVER_ADC_H_

#include "tipos.h"
#include "LPC845.h"
#include "TIMER.h"

#define 	CANTIDAD_CANALES_ADC	2


extern uint32_t	ADC_Cuentas;
extern bool ADC_on;

void ADC_Inicializar(void);
void ADC_Disparo(uint32_t Canal);
void ADC_Stop(void);
void ADC_Reset(void);
void ADC_Start(void);

extern "C" {
void ADC_SEQA_IRQHandler(void);
}


#endif /* DRIVER_ADC_DRIVER_ADC_H_ */
