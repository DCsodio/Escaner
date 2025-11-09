#include "TIMER.h"

TIMER::TIMER(uint32_t timeStart, uint32_t timeReload, void (*func)(void)){
	Start(timeStart, timeReload, func);
}

void TIMER::Start(uint32_t timeStart, uint32_t timeReload, void (*func)(void)){
	this->time = timeStart;
	this->timeReload = timeReload;
	this->func = func;
}

void TIMER::Stop(void)
{
	this->time = 0;
}

void TIMER::Discount(void){
	if(this->time > 0){
		this->time--;
		if(0 == this->time){
			this->time = this->timeReload;
			this->func();
		}
	}
}

void TIMER::setTimeReload(uint32_t timeReload) {
	this->timeReload = timeReload;
}

void TIMER::Callback(void){
	Discount();
}

uint32_t TIMER::getTime(void){
	return time;
}
