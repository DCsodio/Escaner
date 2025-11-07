/*
 * protocoloComu.cpp
 *
 *  Created on: 5 oct. 2025
 *      Author: Eze
 */

#include "protocoloComu.h"

protocoloComu::protocoloComu(uint8_t _caracterInicial, uint8_t _caracterFinal){
	caracterFinal=_caracterFinal;
	caracterInicial=_caracterInicial;
	deteccionDatos=false;
	nuevosDatos=false;
}
void protocoloComu::cambiarCaracterFinal(uint8_t nuevoCaracter){
	caracterFinal=nuevoCaracter;
}
void protocoloComu::cambiarCaracterInicial(uint8_t nuevoCaracter){
	caracterInicial=nuevoCaracter;
}
void protocoloComu::enviar(const char* _datos, uint32_t tamanio){
	UART0_Send((uint8_t*)_datos,tamanio);
}
bool protocoloComu::leerDatos(void){
	uint8_t dato;
	if(UART0_RxAvailable() && nuevosDatos==false){

		dato=UART0_PopRx();

		if(dato==caracterInicial && deteccionDatos==false){
			deteccionDatos=true;
		}else if(deteccionDatos==true){
			if(dato==caracterFinal){
				datos[indice]='\0';
				indice=0;
				deteccionDatos=false;
				nuevosDatos=true;
				return ESTABLE;
			}
			if(indice>=MAX_DATOS){
				deteccionDatos=false;
				indice=0;
				return ERROR_DESBORDE;
			}
			datos[indice]=dato;
			indice++;
		}
	}
	return ESTABLE;
}
uint8_t* protocoloComu::getDatos(void){
	nuevosDatos=false;
	return datos;
}
bool protocoloComu::nuevoMensaje(void){
	return nuevosDatos;
}
