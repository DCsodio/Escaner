#include "GPIO.h"

Gpio::Gpio(uint8_t _port, uint8_t _pin, uint8_t _dir, uint8_t state_init){
	port=_port;
	pin=_pin;
	modo=_dir;


	SYSCON->SYSAHBCLKCTRL0 |= (1 << 7);							// 7 = SWM, ENBL asignacion de pines para uart
	SYSCON->SYSAHBCLKCTRL0 |= (1 << 6) | (1 << 20) | (1 << 18); //enciende los puerto

	Dir(modo);
	if(modo){
		Set(state_init);
	}
}


void Gpio::Set(uint8_t state){

	if(state==1){
		GPIO->SET[port]|=(1<<pin); // unsigned int *SET=(unsigned int*)0xA0002204; *SET|= (1<<1)
	}else{
		GPIO->CLR[port]|=(1<<pin); //unsigned int *CLEAR=(unsigned int*) 0xa0002284; *CLEAR |= (1<<1);
	}
} //Estado del pin

void Gpio::Dir(uint8_t modo){ //seteo de salida o entrada
	if(modo==1){
		GPIO->DIR[port]|=(1<<pin);
	}else{
		GPIO->DIR[port]&=~(1<<pin); //GPIO->DIRCLR[port] = (1 << pin);
	}
} //modo de pin

uint32_t Gpio::Get(){ //Devuelvo uint32_T porque es mas rapido

	if( GPIO->PIN[port] & (1<<pin) ) //compara unicamente el bit seleccionado en pin, retorna un 0 o un numero 2 elevado a la pin
		return 1;
	else
		return 0;
}
//OPTIZMIZAR
