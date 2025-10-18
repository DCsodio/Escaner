/*
 * CALLBACK.cpp
 *
 *  Created on: 23 ago. 2023
 *      Author: gusta
 */

#include "Defines.h"

CALLBACK* vCallBack[MAX_CALLBACK];
static uint8_t cant_call=0;

void CALLBACK::Callback( void )
{
	// Hay que crearla vacia, es la que luego se carga en la herencia
}

CALLBACK::CALLBACK()
{
	if(cant_call<MAX_CALLBACK){
		vCallBack[cant_call]=this;
		cant_call++;
	}
}

void CALLBACK::SysTickCall( void )
{
	for (uint8_t i=0; i<cant_call; i++ ){
		vCallBack[i]->Callback();
	}

}
