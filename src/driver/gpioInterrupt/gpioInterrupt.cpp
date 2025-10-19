/*
 * gpioInterrupt.cpp
 *
 *  Created on: 3 oct. 2025
 *      Author: Eze
 */

#include "GpioInterrupt.h"
GpioInterrupt* GpioInterrupt::instancia = nullptr;

GpioInterrupt::GpioInterrupt(uint32_t port, uint32_t pin, uint32_t pint)
    : Gpio(port, pin, 0, 0)   // entrada
{
    pint_num = pint;

    // Habilitar clock del bloque de interrupciones GPIO (bit 28 en SYSAHBCLKCTRL0)
    SYSCON->SYSAHBCLKCTRL0 |= (1 << 28);

    // Selección del pin físico en PINTSEL
    SYSCON->PINTSEL[pint_num] = (port << 5) | (pin & 0x1F);

    // Configurar en modo flanco
    PINT->ISEL &= ~(1 << pint_num);

    // Habilitar flanco ascendente
    PINT->SIENR = (1 << pint_num);

    // Habilitar IRQ en NVIC (IRQ base 24 para PIN_INT0)
    NVIC->ISER[0] |= (1 << (24 + pint_num));

    instancia=this;
}

void GpioInterrupt::clearFlag() {
    // limpiar flag de interrupción (rising)
    PINT->RISE = (1 << pint_num);
}
void GpioInterrupt::incrementar(void){
	contador++;
}
void GpioInterrupt::setContador(uint32_t _contador){
	contador=_contador;
}
extern "C" void PININT0_IRQHandler(void) {
	if (GpioInterrupt::instancia != nullptr) {
		GpioInterrupt::instancia->incrementar();
	}
    PINT->RISE = (1 << 0);
}
