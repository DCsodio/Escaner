#include "../../../inc/LPC845.h"
#include "dr_pll.h"

void Inicializar_PLL( void )
{
	SYSCON->SYSAHBCLKDIV             = 0x1;       //set clock divider for core to 1
	SYSCON->MAINCLKSEL               &= ~(0x03);  //set "main clock" to IRC oscillator, if not system will lock up when PLL turns off!(sec. 3.5.11)

	SYSCON->MAINCLKUEN               &= ~(1);     //write a zero to the MAINCLKUEN register (sec. 3.5.12), necessary for MAINCLKSEL to update
	SYSCON->MAINCLKUEN               |= 1;        //write a one to the MAINCLKUEN register (sec. 3.5.12), necessary for MAINCLKSEL to update

	SYSCON->SYSPLLCLKSEL             = 0;      	  //connect IRC oscillator to SYSTEM PLL (sec. 3.5.9)

	SYSCON->SYSPLLCLKUEN             &= ~(1);     //write a zero to SYSPLLUEN register (sec. 3.5.10), necessary for SYSPLLCLKSEL to update
	SYSCON->SYSPLLCLKUEN             |= 1;        //write a one to SYSPLLUEN register (sec. 3.5.10), necessary for SYSPLLCLKSEL to update

	SYSCON->PDRUNCFG                 |= (1<<7);   //power down the PLL before changing divider values (sec 3.5.35)
	SYSCON->SYSPLLCTRL               = 0x01;      //24mhz


	//main_clk = fro * (M + 1) / P
	//main_clk = 24Mhz

	SYSCON->PDRUNCFG                 &= ~(1<<7);  //power up PLL after divider values changed (sec. 3.5.35)
	while((SYSCON->SYSPLLSTAT & 1) == 0);         //wait for PLL to lock

	SYSCON->MAINCLKPLLSEL            = 0x01;      //set system oscillator to the output of the PLL (sec. 3.5.11)
	SYSCON->MAINCLKPLLUEN            &= ~(1);     //write a zero to the MAINCLKUEN register (sec. 3.5.12), necessary for MAINCLKSEL to update
	SYSCON->MAINCLKPLLUEN            |= 1;        //write a one to the MAINCLKUEN register (sec. 3.5.12)
}

