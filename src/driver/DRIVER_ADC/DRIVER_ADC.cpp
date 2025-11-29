/*
 * DRIVER_ADC.c
 *
 *  Created on: Jan 3, 2020
 *      Author: GJF-Trabajo
 */

#include "DRIVER_ADC.h"

uint32_t	ADC_Cuentas;
bool ADC_on=false;;

TIMER 	TimerADC(0,0,nullptr);

/*****************************************************************************
** Function name:		ADC_Init
**
** Descriptions:		Inicializamos el conversor analogico digital
**
** parameters:			void
** Returned value:		void
**
*****************************************************************************/
void ADC_Inicializar(void)
{
	SYSCON->PDRUNCFG &= ~(1 << 4);				// Enciendo el modulo del ADC que esta apagado por default

	SYSCON->SYSAHBCLKCTRL0 |= (1 << 24);		// 24 = ADC

	SWM0->PINENABLE0 &= ~(1 << 23);				// ADC_0 enabled on pin PIO0_7 (14)

	ADC0->CTRL 	= (1)							// CLKDIV se recomienda el minimo si no hay una necesidad de velocidad
				| (0 << 8)						// ASYNCMODE
				| (0 << 10)						// LPWRMODE
				| (0 << 30);					// CALMODE

	// A/D Conversion Sequence A Control Register

	ADC0->SEQ_CTRL[0] 	= (0)					// CANAL, lo voy a seleccionar en el momento de disparar la conversion
						| (0 << 12)				// TRIGGER
												//	Input 	Source 					Description
												//	0 								- No hardware trigger.
												//	1 		PININT0_IRQ 			GPIO_INT interrupt 0.
												//	2 		PININT1_IRQ 			GPIO_INT interrupt 1.
												//	3 		SCT0_OUT3 				SCT output 3.
												//	4 		SCT0_OUT4 				SCT output 4.
												//	5 		T0_MAT3 				CTIMER match 3.
												//	6 		CMP0_OUT_ADC 			Analog comparator output.
												//	7 		GPIO_INT_BMAT 			GPIO_INT bmatch.
												//	8 		ARM_TXEV 				ARM core TXEV event.
						| (1 << 18)				// TRIGPOL					0=FLANCO NEGATIVO DE LOS TRIGGERS 1=FLANCO POSITIVO DE LOS TRIGGERS
						| (0 << 19)				// SYNCBYPASS
						| (0 << 26)				// START OF CONVERSION		1=SOC 0=STOP
						| (0 << 29)				// SINGLESTEP
						| (0 << 30)				// MODE						0
						| (1 << 31);			// SEQA_ENA					0=SECUENCIA A DESHABILITADA 1=HABILITADA

	ADC0->INTEN = (1 << 0);						// Sequence A interrupt enable

	NVIC->ISER[0] |= (1 << 16); 					/* enable interrupt ISE_ADC_SEQA*/

	TimerADC.Start(1,1, ADC_Start);		// Configuro el timerADC para llamar a la funcion ADC_Disparo cada 100ms
	ADC_on=true;
}

void ADC_Start(void){
	ADC_Disparo(9); //el canal es distinto en cada pin
}

void ADC_Disparo(uint32_t Canal)
{
	ADC0->SEQ_CTRL[0] 	= (1 << Canal)		// CANAL, seleccionar al momento de disparar la conversion
						| (0 << 12)		// TRIGGER
						| (1 << 18)		// TRIGPOL								0=FLANCO NEGATIVO DE LOS TRIGGERS 1=FLANCO POSITIVO DE LOS TRIGGERS
						| (0 << 19)		// SYNCBYPASS
						| (1 << 26)		// START OF CONVERSION	1=SOC 0=STOP
						| (0 << 29)		// SINGLESTEP
						| (0 << 30)		// MODE
						| (1 << 31);	// SEQA_ENA	0=SECUENCIA A DESHABILITADA 1=HABILITADA
}

void ADC_SEQA_IRQHandler(void)
{
	uint32_t Temporal = ADC0->SEQ_GDAT[0];

	//uint32_t Canal = (Temporal >> 26) & 0x0F;
	uint32_t Valor_Temporal = (Temporal >> 4) & 0xFFF;

	ADC_Cuentas = Valor_Temporal;

	ADC0->FLAGS = (1 << 9);  // *** IMPORTANTE: limpiar FLAG ***
}

void ADC_Stop(void){
	TimerADC.Stop();
	ADC_on=false;
}
void ADC_Reset(void){
	ADC_on=true;
	TimerADC.Start(1, 1, ADC_Start);
}


