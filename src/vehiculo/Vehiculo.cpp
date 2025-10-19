/*
 * Vehiculo.cpp
 *
 *  Created on: 5 jul. 2025
 *      Author: Ezequiel
 */

#include "Vehiculo.h"

Vehiculo::Vehiculo(uint32_t _diametroRuedaCm, uint32_t _posX, uint32_t _posY,
		  	  	  uint32_t _largo,uint32_t _ancho,uint32_t _altura,
				  uint32_t portA1, uint32_t pinA1,
				  uint32_t portA2, uint32_t pinA2,
				  uint32_t portB1, uint32_t pinB1,
				  uint32_t portB2, uint32_t pinB2,
				  uint32_t _anchoPulso, uint32_t _periodo)
:control(portA1,pinA1,
		portA2,pinA2,
		portB1,pinB1,
		portB2,pinB2)
{
    diametroRuedaCm=_diametroRuedaCm;
    largoCm=_largo;
    anchoCm=_ancho;
    alturaCm=_altura;
    posX=_posX;
    posY=_posY;

}

        //esto no cambia no necesita setters
uint32_t Vehiculo::getLargoCm(void){
    return largoCm;
}
uint32_t Vehiculo::getAnchoCm(void){
    return anchoCm;
}
uint32_t Vehiculo::getAlturaCm(void){
    return alturaCm;
}
//esto si puede cambiar
uint32_t Vehiculo::getPosY(void){
    return posY;
}
uint32_t Vehiculo::getPosX(void){
    return posX;
}
uint32_t Vehiculo::getDiametroRuedaCm(void){
    return diametroRuedaCm;
}
void Vehiculo::SetPosY(uint32_t _posY){
    posY=_posY;
}
void Vehiculo::SetPosX(uint32_t _posX){
    posX=_posX;
}
void Vehiculo::SetDiametroRuedaCm(uint32_t _nuevoDiametro){
    diametroRuedaCm=_nuevoDiametro;
}


void Vehiculo::adelante(){
	control.adelante();
	//posX=posX+distanciaCm;
}
void Vehiculo::atras(){
	control.atras();
	//posX=posX-distanciaCm;
}
void Vehiculo::izquierda(){
	control.izquierda();
}
void Vehiculo::derecha(){
	control.derecha();

}
void Vehiculo::detener(){
	control.detener();

}
