/*
 * TIMER.h
 *
 *  Created on: 10 jun. 2025
 *      Author: gusta
 */

#ifndef TIMERS_DRIVER_TIMER_H_
#define TIMERS_DRIVER_TIMER_H_

#include "tipos.h"
#include "CALLBACK.h"
class TIMER : public CALLBACK{
private:
	uint32_t time;
	uint32_t timeReload;

	void (*func)(void);

public:
	TIMER(uint32_t timeStart, uint32_t timeReload, void (*func)(void));
	uint32_t getTime(void);
	void Start(uint32_t timeStart, uint32_t timeReload, void (*func)(void));
	void Discount(void);
	void Stop(void);
	void Callback( void );
};

#endif /* TIMERS_DRIVER_TIMER_H_ */
