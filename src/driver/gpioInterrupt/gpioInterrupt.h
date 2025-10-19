#ifndef GPIOINTERRUPT_H_
#define GPIOINTERRUPT_H_

#include "GPIO.h"
#include "LPC845.h"
#include "LPC845v2.h"

class GpioInterrupt : public Gpio {
public:
	static GpioInterrupt* instancia;
private:
    uint32_t pint_num;   // canal PINT (0..7)
    volatile uint32_t contador = 0;
public:
    GpioInterrupt(uint32_t port, uint32_t pin, uint32_t pint);
    void setContador(uint32_t _contador);
    void incrementar(void);
    void clearFlag();  // limpiar flag de interrupción
};

#endif /* GPIOINTERRUPT_H_ */


