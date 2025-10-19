/*
 * GpioPwm.h
 *
 *  Created on: 30 jun. 2025
 *      Author: Ezequiel
 */

#ifndef GPIOPWM_DRIVER_GPIOPWM_H_
#define GPIOPWM_DRIVER_GPIOPWM_H_

#include "Gpio.h"
#include "CALLBACK.h"

class GpioPwm : private Gpio, private CALLBACK  {
private:
	uint32_t anchoPulso;
	uint32_t periodo;
	uint32_t contPeriodo;
	uint32_t limiteOn;
public:
	GpioPwm(uint32_t port, uint32_t pin, uint32_t _anchoPulso, uint32_t _periodo);
	void Set(uint32_t _anchoPulso);
	void Callback(void);
    uint32_t Get(void);
    void PWM(void);
    GpioPwm& operator=(uint32_t _anchoPulso);
};

#endif /* GPIOPWM_DRIVER_GPIOPWM_H_ */
