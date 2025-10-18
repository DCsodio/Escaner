#include "Uart0.h"

UART0_Struct		UART0;

void UART0_Init(uint32_t baudrate)
{
	SYSCON->SYSAHBCLKCTRL0 |= (1 << 7);
	//Seleccióno la UART que deseo activar (Ej UART0):
	SYSCON->SYSAHBCLKCTRL0 |= (1 << 14);

	//Seleccióno los pines TX y RX para la UART
	//Ejemplo: Tx0 = P0.9 y Rx0 = P0.8
	SWM0->PINASSIGN.PINASSIGN0 = (9 << 0) | (8 << 8);

	// CONFIGURACION EXTRA
	USART0->CTL = 0;

	// Habilito interrupciones
	USART0->INTENSET = (1 << 0);	//RX INTERRUPT

	NVIC->ISER[0] = (1 << 3);

	// CONFIGURACION DEL BAUDRATE
	UART0CLKSEL = 1;

	USART0->BRG =(FREQ_PRINCIPAL / (16 * baudrate)) - 1;
	//USART0->BRG = (FREQ_PRINCIPAL / baudrate) / 16;
	// CONFIGURACION GENERAL
	USART0->CFG = 	(1 << 0)		// 0=DISABLE 1=ENABLE
					| (1 << 2)		// 0=7BITS 1=8BITS 2=9BITS
					| (0 << 4)		// 0=NOPARITY 2=PAR 3=IMPAR
					| (0 << 6)		// 0=1BITSTOP 1=2BITSTOP
					| (0 << 9)		// 0=NOFLOWCONTROL 1=FLOWCONTROL
					| (0 << 11)		// 0=ASINCRONICA 1=SINCRONICA
					| (0 << 14);	// 0=SLAVE 1=MASTER

}

void UART0_IRQHandler (void)
{
	int32_t Temporal;

	uint32_t status = USART0->STAT;

	if(status & (1 << 0))
	{
		//RX
		Temporal = (int32_t)USART0->RXDAT;

		UART0_PushRx((uint8_t)Temporal);
	}

	if(status & (1 << 2))
	{
		//TX
		Temporal = UART0_PopTx();
		if(Temporal >= 0)
			USART0->TXDAT = (uint8_t)Temporal;
		else
		{
			USART0->INTENCLR = (1 << 2); //disable int TX
			UART0.TX.Datos_Validos = 0;
		}
	}
}

void UART0_Send(uint8_t *Datos, uint32_t Tamanio)
{
	uint32_t i=0;

	if(0 == Tamanio)
		Tamanio = strlen(Datos);

	for(i = 0 ; i < Tamanio ; i++)
		UART0_PushTx(Datos[i]);

	return;
}

uint32_t strlen(uint8_t *Datos)
{
	int i;

	for(i = 0 ; Datos[i] != 0 ; i++);

	return i;
}


void UART0_PushTx(uint8_t dato)
{
	UART0.TX.Buffer[UART0.TX.Indice_in] = dato;

	UART0.TX.Indice_in ++;
	UART0.TX.Indice_in %= UART0_TAMANIO_COLA_TX;

	if ( UART0.TX.Datos_Validos == 0 )
	{
		UART0.TX.Datos_Validos = 1;

		USART0->TXDAT = (uint8_t)UART0_PopTx();

		USART0->INTENSET = (1 << 2); // int tx
	}
}

int32_t UART0_PopTx( void )
{
	int32_t dato = -1;

	if ( UART0.TX.Indice_in != UART0.TX.Indice_out )
	{
		dato = (int32_t) UART0.TX.Buffer[UART0.TX.Indice_out];
//		UART0.TX.Buffer[UART0.TX.Indice_out] = 0;

		UART0.TX.Indice_out ++;
		UART0.TX.Indice_out %= UART0_TAMANIO_COLA_TX;
	}
	return dato;
}

void UART0_PushRx(uint8_t dato)
{
	UART0.RX.Buffer[UART0.RX.Indice_in] = dato;

	UART0.RX.Indice_in ++;
	UART0.RX.Indice_in %= UART0_TAMANIO_COLA_RX;
}

int32_t UART0_PopRx( void )
{
	int32_t dato = -1;

	if ( UART0.RX.Indice_in != UART0.RX.Indice_out )
	{
		dato = (int32_t) UART0.RX.Buffer[UART0.RX.Indice_out];
		UART0.RX.Indice_out ++;
		UART0.RX.Indice_out %= UART0_TAMANIO_COLA_RX;
	}
	return dato;
}
int32_t UART0_RxAvailable( void ){
	return UART0.RX.Indice_in != UART0.RX.Indice_out;
}
void UART0_SendBlocking(uint8_t *Datos, uint32_t Tamanio)
{
	//USART0->INTENSET = (1 << 2);
    for (uint32_t i = 0; i < Tamanio; i++) {
        while(!(USART0->STAT & (1 << 2))); // TXRDY
        USART0->TXDAT = Datos[i];
    }
    //USART0->INTENCLR = (1 << 2);
}
