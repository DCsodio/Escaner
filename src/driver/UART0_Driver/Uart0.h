/*
 * Uart0.h
 *
 *  Created on: 19 set. 2023
 *      Author: gusta
 */

#ifndef UART0_DRIVER_UART0_H_
#define UART0_DRIVER_UART0_H_

#include "../../../inc/tipos.h"
#include "../../../inc/LPC845v2.h"
#include "../../../inc/LPC845.h"

#define FREQ_PRINCIPAL (24000000UL)
#define		UART0_TAMANIO_COLA_RX					100
#define		UART0_TAMANIO_COLA_TX					100


typedef struct
{
	uint8_t						Datos_Validos;
	uint16_t					Tamanio;
	uint8_t						Buffer[UART0_TAMANIO_COLA_RX];
	uint16_t					Indice_in, Indice_out;
}STRUCT_RX0;

typedef struct
{
	volatile uint8_t						Datos_Validos;
	volatile uint16_t					Tamanio;
	volatile uint8_t						Buffer[UART0_TAMANIO_COLA_TX];
	volatile uint16_t					Indice_in, Indice_out;
}STRUCT_TX0;

typedef struct
{
	STRUCT_RX0					RX;
	STRUCT_TX0					TX;
}UART0_Struct;

void UART0_Init(uint32_t baudrate);
void UART0_Send(uint8_t *Datos, uint32_t Tamanio);
void UART0_PushTx(uint8_t Dato);
int32_t UART0_PopTx( void );
void UART0_PushRx(uint8_t Dato);
int32_t UART0_PopRx( void );
int32_t UART0_RxAvailable( void );
uint32_t strlen(uint8_t *Datos);
void UART0_SendBlocking(uint8_t *Datos, uint32_t Tamanio);

extern "C" {
	void UART0_IRQHandler (void);
}

#endif /* UART0_DRIVER_UART0_H_ */
