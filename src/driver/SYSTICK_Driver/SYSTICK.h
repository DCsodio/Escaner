#ifndef SYSTICK_DRIVER_SYSTICK_H_
#define SYSTICK_DRIVER_SYSTICK_H_


#include "../../../inc/tipos.h"
#include "../../../inc/LPC845v2.h"
#include "../../../inc/LPC845.h"
#include "Callback.h"
#define MAX_TICKS		0xFFFFFF
#define FREQ_PRINCIPAL	(24000000UL)

extern "C" {
void SysTick_Handler(void);
}

class SYSTICK {
public:
	SYSTICK(uint32_t ticks);
};

#endif /* SYSTICK_DRIVER_SYSTICK_H_ */
