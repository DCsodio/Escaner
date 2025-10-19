/*
 * GpioPwm.cpp
 *
 *  Created on: 30 jun. 2025
 *      Author: Ezequiel
 */
/*
 * _anchoPulso 50
 * Perido=100
 */
#include "GpioPwm.h"

GpioPwm::GpioPwm(uint32_t port, uint32_t pin, uint32_t _anchoPulso, uint32_t _periodo):Gpio(port,pin,SALIDA,0){ //_anchode pulso en % y el periodo en ms
	anchoPulso=_anchoPulso;
	periodo=_periodo;
	contPeriodo=0;
	limiteOn=(anchoPulso*periodo)/100; //(anchoPulso/100)*periodo
	Gpio::Set(0);
}

void GpioPwm::Set(uint32_t _anchoPulso){
	anchoPulso=(_anchoPulso>100)?100:_anchoPulso; //si es mayor que 100 es 100 sino _ancho de pulso
	limiteOn = (anchoPulso * periodo) / 100;;
}

uint32_t GpioPwm::Get(void){
	return anchoPulso;
}

void GpioPwm::Callback(void){
	PWM();
}

void GpioPwm::PWM(void){
	if(contPeriodo<periodo){	//el periodo es en ms
			if(contPeriodo<limiteOn){
				Gpio::Set(1);
			}else{
				Gpio::Set(0);
			}
			contPeriodo++;
		}else{
			contPeriodo=0;
		}
}

 GpioPwm& GpioPwm::operator=(uint32_t _anchoPulso){
	Set(_anchoPulso);
	return *this;
}
