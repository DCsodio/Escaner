#include "Defines.h"
#include "Hardware.h"
#include "driver/PLL/dr_pll.h"
#include "Uart0.h"
#include "Systick.h"
#include "laserHl.h"
#include "convert.h"
#include "brujulaHl.h"
#include <string.h>
#include "protocoloPaquete.h"
#include "Vehiculo.h"
#include "motorstepper.h"
#include "gpioInterrupt.h"

Paquete pkt;

Vehiculo vehiculo(10,0,0,10,10,5,
		PINMOTA1,
		PINMOTA2,
		PINMOTB1,
		PINMOTB2);

motorstepper motor(PINSTPRDIR,
				PINSTPRPASO,
				PINSTPREN);

laserHl laser;

GpioInterrupt sensorOptico(PINSENSOROPTICO,0);

brujulaHl brujula;

//FUNCION ANALIZAR
volatile bool analisisTerminado=false;
volatile bool sentidoHorario=true;
void analizar(void);

//FUNCION POSICIONINICIAL
void posicionInicial(void);

//TIMERS
volatile bool timerEjecutado=false;
TIMER ctrlVelAnalizar(0,0,analizar);
TIMER ctrlVelPosInit(0,0,posicionInicial);

volatile uint32_t cantidadPasos=0;

int main(void)
{

	Inicializar_PLL();
	UART0_Init(115200);
	i2c_init();
	SYSTICK systick(1000);


	laser.configurarLaser(2, 50, 100);
	laser.iniciarLaser();

	brujula.inicializar(HMC5883_MAGGAIN_8_1);

	pkt.header[0]='{';
	pkt.header[1]='{';

	while (1){
		if(timerEjecutado==false && analisisTerminado==false){
			ctrlVelAnalizar.Start(500, 500, analizar);
			timerEjecutado=true;
		}
		if(timerEjecutado==false && analisisTerminado==true){
			ctrlVelPosInit.Start(500, 500, posicionInicial);
			timerEjecutado=true;
		}


    }
}

void analizar(void){
	if(cantidadPasos<CANTPASOS180 && analisisTerminado==false){
		//ETAPA TOMA DE DATOS
		pkt.analizando=true;
		pkt.distanciaMm=laser.getDistanciaMm();
		pkt.grados=brujula.getGrados();
		pkt.checksum=calcularChecksum(&pkt);
		UART0_SendBlocking((uint8_t*)&pkt,  sizeof(Paquete));
		//ETAPA MOTOR
		motor.setDireccion(sentidoHorario);
		motor.setEnable(true);
		motor.moverPaso();
		motor.setEnable(false);
		//for(uint8_t i=0; i<100; i++);
		cantidadPasos++;
	}else{
		analisisTerminado=true;
		timerEjecutado=false;
	}
}
void posicionInicial(void){
	if(sentidoHorario==true && cantidadPasos>0){
		motor.setDireccion(!sentidoHorario);
		motor.setEnable(true);
		motor.moverPaso();
		cantidadPasos--;
	}else if (sentidoHorario==false && cantidadPasos>0){
		motor.setDireccion(!sentidoHorario);
		motor.setEnable(true);
		motor.moverPaso();
		cantidadPasos--;
	}else if(cantidadPasos<=0){
		ctrlVelPosInit.Stop();
		motor.setEnable(false);
		sentidoHorario=!sentidoHorario;
		analisisTerminado=false;
		timerEjecutado=false;
	}
}
