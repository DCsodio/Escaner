/*
 * protocoloComu.cpp
 *
 *  Created on: 5 oct. 2025
 *      Author: Eze
 */

#include "protocoloComu.h"

protocoloComu::protocoloComu(uint8_t _caracterFinal){
	caracterFinal=_caracterFinal;
}
void protocoloComu::cambiarCaracterFinal(uint8_t nuevoCaracter){
	caracterFinal=nuevoCaracter;
}
void protocoloComu::enviar(const char* _datos, uint32_t tamanio){
	UART0_Send((uint8_t*)_datos,tamanio);
}
bool protocoloComu::leerDatos(void){
	while(UART0_RxAvailable()){
		if(indice>=MAX_DATOS){
			indice=0;
			return ERROR_DESBORDE;
		}

		datos[indice]=(int8_t)UART0_PopRx();
		if(datos[indice]==caracterFinal){
			datos[indice]='\0';
			indice=0;
			nuevosDatos=true;
			return ESTABLE;
		}
		indice++;
	}
	return ESTABLE;
}
int8_t* protocoloComu::getDatos(void){
	nuevosDatos=false;
	return datos;
}
bool protocoloComu::nuevoMensaje(void){
	return nuevosDatos;
}
