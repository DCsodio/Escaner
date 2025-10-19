#ifndef GPIOINTERRUPT_H_
#define GPIOINTERRUPT_H_

#include "GPIO.h"
#include "LPC845.h"
#include "LPC845v2.h"

class GpioInterrupt : public Gpio {
private:
    uint32_t pint_num;   // canal PINT (0..7)

public:
    GpioInterrupt(uint32_t port, uint32_t pin, uint32_t pint);

    void clearFlag();  // limpiar flag de interrupción
};

#endif /* GPIOINTERRUPT_H_ */


