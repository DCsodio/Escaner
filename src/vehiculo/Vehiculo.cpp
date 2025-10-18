/*
 * Vehiculo.cpp
 *
 *  Created on: 5 jul. 2025
 *      Author: Ezequiel
 */

#include "Vehiculo.h"

Vehiculo::Vehiculo(uint32_t _diametroRuedaCm, uint32_t _posX, uint32_t _posY,uint32_t _largo,uint32_t _ancho,uint32_t _altura){
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


void Vehiculo::adelante(uint32_t distanciaCm){
	control.adelante(distanciaCm);
	posX=posX+distanciaCm;
}
void Vehiculo::atras(uint32_t distanciaCm){
	control.atras(distanciaCm);
	posX=posX-distanciaCm;
}
void Vehiculo::izquierda(uint32_t angulo){
	control.izquierda(angulo);
}
void Vehiculo::derecha(uint32_t angulo){
	control.derecha(angulo);

}
