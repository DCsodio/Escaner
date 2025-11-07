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
#include "stdlib.h"

#define ENABLE_TEST_MOVILIDAD true

volatile Paquete pkt;

//Led para verificar transmision
Gpio LED_TX(1,0,1,1);
bool state=false;

//Creacion del vehiculo
Vehiculo vehiculo(10,0,0,10,10,5,
		PINMOTA1,
		PINMOTA2,
		PINMOTB1,
		PINMOTB2);

//Creacion del motor
motorstepper motor(PINSTPRDIR,
				PINSTPRPASO,
				PINSTPREN);

//Creacion del laser
laserHl laser;

//Creacion de la brujula
brujulaHl brujula;
bool anguloInicial=false;

//FUNCION ANALISIS SECTORIAL
volatile bool analisisTerminado=true;
volatile bool sentidoHorario=true;
void analizar(void);

//FUNCION POSICION INICIAL
void posicionInicial(void);
volatile uint32_t cantidadPasos=0;

//TIMER PARA VELOCIDAD LIDAR
volatile bool timerEjecutado=false;
TIMER ctrlVelAnalizar(0,0,analizar);
uint16_t velocidadAnalisis=210;
void moverMotor(bool sentido,  bool tipoPaso=1);
uint8_t maquinaEstado=0;

//TIMER PARA VELOCIDAD DE POSICION INICIAL
TIMER ctrlVelPosInit(0,0,posicionInicial);

//TIMEOUT
#define MAX_TIMEOUT 400
void reenviar(void);
TIMER timeOut(0,0,nullptr);
volatile bool timerTimeOut=false;

//CREACION PROTOCOLO DE COMUNICACION
protocoloComu proto('>','<');


char valor[20];
bool rum=false;

bool estado_actual=false;
bool estado_anterior=false;

uint32_t cantPulsos=0;

int main(void)
{

	Inicializar_PLL();
	vehiculo.detener();
	SYSTICK systick(1000);
	UART0_Init(115200);

	i2c_init();

	laser.iniciarLaser();
	laser.configurarLaser(2, 33, 50);

	brujula.inicializar(HMC5883_MAGGAIN_4_0);

	bool init=false;

	while(!init){
		proto.leerDatos();
		if(proto.nuevoMensaje()){
			if(strcmp((char*)proto.getDatos(),"init")==0){
				init=true;
			}
		}
	}
	for (int i=0; i<10000; i++);
	pkt.header[0]='{';
	pkt.header[1]='{';

	if(ENABLE_TEST_MOVILIDAD){ //test movilidad
		pkt.analizando=0;
		pkt.distanciaMm=0;
		pkt.grados=0;
		pkt.checksum=calcularChecksum(&pkt);
		UART0_Send((uint8_t*)&pkt, sizeof(Paquete));
	}

	ADC_Inicializar();
	ADC_Stop();

	while (1){
		if(ADC_on){
			// Detecta si hay luz o sombra
			    if (ADC_Cuentas > 3950) estado_actual = false;     // sombra
			    else if (ADC_Cuentas < 3800) estado_actual = true; // luz

			    // Detecta flanco
			    if (!estado_anterior && estado_actual) {
			        cantPulsos++;  // flanco ascendente
			    }

			    estado_anterior = estado_actual;
		}



		proto.leerDatos();
		if(proto.nuevoMensaje()){
			const char* datos = (const char*)proto.getDatos();
			if(analisisTerminado){
				if(strcmp(datos,"p")==0){
					vehiculo.detener();
					ADC_Stop();
					pkt.analizando=false;

					uint32_t _valorPromedio=0;
					for(uint8_t veces=0;veces<4; veces++){
						_valorPromedio=_valorPromedio+brujula.getGrados();
					}
					_valorPromedio=_valorPromedio/4;
					pkt.grados=_valorPromedio;

					pkt.pulsos=cantPulsos;
					pkt.distanciaMm=laser.getDistanciaMm();
					pkt.checksum=calcularChecksum(&pkt);
					UART0_Send((uint8_t*)&pkt,  sizeof(Paquete));
					cantPulsos=0;

				}else if(strcmp(datos,"w")==0){
					ADC_Reset();
					pkt.direccionAdelante=true;
					vehiculo.adelante();
				}else if(strcmp(datos,"s")==0){
					pkt.direccionAdelante=false;
					ADC_Reset();
					vehiculo.atras();
				}else if(strcmp(datos,"a")==0){
					vehiculo.izquierda();
				}else if(strcmp(datos,"d")==0){
					vehiculo.derecha();
				}else if(strcmp(datos,"analizar")==0){
					analisisTerminado=false;
					anguloInicial=false;
					if(timerEjecutado==false){
						ctrlVelAnalizar.Start(velocidadAnalisis, velocidadAnalisis, analizar);
						timerEjecutado=true;
					}
				}

			}else{
				if(strcmp(datos,"enMov")==0){
					ctrlVelAnalizar.Stop();
					timerEjecutado=false;
					if(timerEjecutado==false){
						ctrlVelPosInit.Start(100,100, posicionInicial);
						timerEjecutado=true;
					}
					//enmov
				}else if(strcmp(datos,"disMov")==0){
					analisisTerminado=true;
				}
			}
			if(strncmp(datos,"motor:",6)==0){
				uint8_t *p=proto.getDatos();
				uint8_t valor=atoi((const char*)&(p[6]));
				vehiculo.cambiarVelocidad(valor);
			}else if(strncmp(datos,"periodo:",8)==0){
				uint8_t *p=proto.getDatos();
				uint16_t valor=atoi((const char*)&(p[8]));
				velocidadAnalisis=valor;
			}/*else if(strcmp((char*)proto.getDatos(),"reset")==0){
				//reset
			}*/
		}
    }
}

void analizar(void){
	if(cantidadPasos<CANTPASOS180 && analisisTerminado==false){
		//ETAPA TOMA DE DATOS
			uint32_t _valorPromedio=0;
			pkt.analizando=true;
			for(uint8_t veces=0;veces<4; veces++){
				_valorPromedio=_valorPromedio+laser.getDistanciaMm();
			}
			_valorPromedio=_valorPromedio/4;
			pkt.distanciaMm=_valorPromedio;

			_valorPromedio=0;
			if(!anguloInicial){
				for(uint8_t veces=0;veces<4; veces++){
					_valorPromedio=_valorPromedio+brujula.getGrados();
				}
				_valorPromedio=_valorPromedio/4;
				pkt.grados=_valorPromedio;
				anguloInicial=true;
			}
			pkt.cantidadPasos=cantidadPasos;
			pkt.checksum=calcularChecksum(&pkt);
			UART0_Send((uint8_t*)&pkt, sizeof(Paquete)); //hay que sacar esto
			LED_TX.Set(state);
			state=!state;

			moverMotor(sentidoHorario);

	}else if (cantidadPasos>=CANTPASOS180){
			if(maquinaEstado==0 || maquinaEstado==2 ){
				sentidoHorario=!sentidoHorario;
			}
			maquinaEstado++;
			cantidadPasos=0;
			if(maquinaEstado==4){
				//enviamos que termino el analisis
				pkt.analizando=false;
				pkt.checksum=calcularChecksum(&pkt);
				UART0_Send((uint8_t*)&pkt,  sizeof(Paquete));
				LED_TX.Set(false);
				analisisTerminado=true;
				ctrlVelAnalizar.Stop();
				timerEjecutado=false;
				maquinaEstado=0;
			}
	}
}


void posicionInicial(void){
	if((maquinaEstado==0 ||maquinaEstado==2) && cantidadPasos>0){
		moverMotor(!sentidoHorario,false);
	}else if ((maquinaEstado==1||maquinaEstado==3) && cantidadPasos<180){
		moverMotor(sentidoHorario,true);
	}else{
		ctrlVelPosInit.Stop();
		motor.setEnable(false);
		sentidoHorario=true;
		timerEjecutado=false;
		maquinaEstado=0;
	}
}


void moverMotor(bool sentido, bool tipoPaso){
	motor.setDireccion(sentido);
	motor.setEnable(true);
	motor.moverPaso();
	//motor.setEnable(false);
	if(tipoPaso){
		cantidadPasos++;
	}else{
		cantidadPasos--;
	}

}

/*
 * conversor valores:
 * val<3600 abierto
 * val>4000 cerrado
 */
