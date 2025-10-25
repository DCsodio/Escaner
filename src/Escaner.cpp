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
#include "DRIVER_ADC.h"
#include "protocoloComu.h"

#define RECEPCION_PKT_OK "OK"

volatile Paquete pkt;

Vehiculo vehiculo(10,0,0,10,10,5,
		PINMOTA1,
		PINMOTA2,
		PINMOTB1,
		PINMOTB2);

motorstepper motor(PINSTPRDIR,
				PINSTPRPASO,
				PINSTPREN);

laserHl laser;

brujulaHl brujula;

//FUNCION ANALIZAR
volatile bool analisisTerminado=false;
volatile bool sentidoHorario=true;
void analizar(void);

//FUNCION POSICIONINICIAL
void posicionInicial(void);
volatile uint32_t cantidadPasos=0;

//TIMERS PARA LIDAR
volatile bool timerEjecutado=false;
TIMER ctrlVelAnalizar(0,0,analizar);
TIMER ctrlVelPosInit(0,0,posicionInicial);
void moverMotor(void);

//TIMEOUT
#define MAX_TIMEOUT 10
void reenviar(void);
TIMER timeOut(0,0,nullptr);
volatile bool timerTimeOut=false;
protocoloComu proto('>','<');

//CONTROL DE VECES ANALIZADAS DEL SECTOR
uint8_t cantidadAnalisis=0;

//ADC
//uint32_t	ADC_Cuentas;
//uint32_t	ADC_Medicion;

void Print(void);
char valor[20];
bool rum=false;
int main(void)
{

	Inicializar_PLL();
	SYSTICK systick(1000);
	UART0_Init(115200);

	i2c_init();

	laser.iniciarLaser();
	laser.configurarLaser(2, 50, 100);

	brujula.inicializar(HMC5883_MAGGAIN_8_1);

	pkt.header[0]='{';
	pkt.header[1]='{';
	pkt.posX=vehiculo.getPosX();
	pkt.posY=vehiculo.getPosY();

	ADC_Inicializar();

	while (1){
		proto.leerDatos();
		if(timerTimeOut){
			proto.leerDatos();
			if(proto.nuevoMensaje()){
				if(strcmp((char*)proto.getDatos(),RECEPCION_PKT_OK)==0){
					timeOut.Stop();
					moverMotor();
					timerTimeOut=false;
				}
			}
		}

		if(timerEjecutado==false && cantidadAnalisis<2){
			ctrlVelAnalizar.Start(500, 500, analizar);
			timerEjecutado=true;
		}else if(timerEjecutado==false && analisisTerminado==true){
			ctrlVelPosInit.Start(500, 500, posicionInicial);
			cantidadAnalisis++;
			timerEjecutado=true;
		}

		if(cantidadAnalisis==2){
			//aca empieza a aparece la comunicacion con QT
		}

    }
}

void analizar(void){
	if(rum==false){


	if(cantidadPasos<CANTPASOS180 && analisisTerminado==false){
		//ETAPA TOMA DE DATOS
		if(timerTimeOut==false){
			pkt.analizando=true;
			pkt.distanciaMm=laser.getDistanciaMm();
			pkt.grados=brujula.getGrados();
			pkt.checksum=calcularChecksum(&pkt);
			UART0_SendBlocking((uint8_t*)&pkt,  sizeof(Paquete)); //hay que sacar esto

			//timerTimeOut=true;
			//timeOut.Start(MAX_TIMEOUT, 0, reenviar);
		}
	}else if (cantidadPasos>=CANTPASOS180){
		analisisTerminado=true;
		ctrlVelAnalizar.Stop();
		timerEjecutado=false;
	}
	rum=true;
	}


}


void posicionInicial(void){
	if(cantidadPasos>0){
		if(sentidoHorario==true){
			motor.setDireccion(!sentidoHorario);
			motor.setEnable(true);
			motor.moverPaso();
			cantidadPasos--;
		}else if (sentidoHorario==false){
			motor.setDireccion(!sentidoHorario);
			motor.setEnable(true);
			motor.moverPaso();
			cantidadPasos--;
		}
	}else{
		ctrlVelPosInit.Stop();
		motor.setEnable(false);
		sentidoHorario=!sentidoHorario;
		analisisTerminado=false;
		timerEjecutado=false;
	}
}


void moverMotor(void){
	motor.setDireccion(sentidoHorario);
	motor.setEnable(true);
	for(uint8_t i=0; i<20; i++);
	motor.moverPaso();
	motor.setEnable(false);
	cantidadPasos++;
}

void reenviar(void){
	UART0_SendBlocking((uint8_t*)&pkt, sizeof(Paquete));
}

/*
 * conversor valores:
 * val<3600 abierto
 * val>4000 cerrado
 */
