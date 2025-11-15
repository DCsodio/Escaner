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
#define MAX_MUESTRAS 4

Paquete pkt;

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
bool enStepper=false;

//Creacion del laser
laserHl laser;

//Creacion de la brujula
brujulaHl brujula;
volatile bool anguloInicial=false;

//FUNCION POSICION INICIAL
void posicionInicial(void);

//FUNCION ANALISIS SECTORIAL
volatile bool analisisTerminado=true;
volatile bool sentidoHorario=true;
volatile bool flagAnalizar=false;
volatile uint8_t maquinaEstado=0;
void analizar(void);
void enAnalizar(void);

//TIMER PARA VELOCIDAD LIDAR
TIMER ctrlVelAnalizar(0,0,enAnalizar);
volatile uint16_t velocidadAnalisis=210;

//CREACION PROTOCOLO DE COMUNICACION
protocoloComu proto('>','<');

bool estado_actual=true;
bool estado_anterior=true;

uint32_t cantPulsos=0;

int main(void)
{

	Inicializar_PLL();
	vehiculo.detener();
	motor.setEnable(false);
	SYSTICK systick(1000);
	UART0_Init(115200);
	i2c_init();

	//Laser
	laser.iniciarLaser();
	laser.configurarLaser(2, 33, 50);

	//Brujuila
	brujula.inicializar(HMC5883_MAGGAIN_4_0);

//	brujula.Calibrar(-17.5, -62.5, 1.0066, 0.9934);

	//detecta que el esp32 se haya conectado
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

	//habilitamos los botones && preparamos el header
	pkt.header[0]='{';
	pkt.header[1]='{';
	pkt.analizando=false;
	pkt.checksum=calcularChecksum(&pkt);
	UART0_Send((uint8_t*)&pkt, sizeof(Paquete));

	//configuramos el ADC
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
				if(proto.getLargoCadena()==1){
					if(datos[0]=='p'){
						vehiculo.detener();
						for(int i=0; i<10000; i++){
						}
						ADC_Stop();
						uint32_t _valorPromedio=0;
						for(uint8_t veces=0;veces<MAX_MUESTRAS; veces++){
							_valorPromedio=_valorPromedio+brujula.getGrados();
						}
						_valorPromedio=_valorPromedio/MAX_MUESTRAS;
						pkt.grados=_valorPromedio;
						pkt.pulsos=cantPulsos;
						pkt.checksum=calcularChecksum(&pkt);
						UART0_Send((uint8_t*)&pkt,  sizeof(Paquete));
						cantPulsos=0;

					}else if(datos[0]=='w'){
						ADC_Reset();
						pkt.direccionAdelante=true;
						vehiculo.adelante();

					}else if(datos[0]=='s'){
						ADC_Reset();
						pkt.direccionAdelante=false;
						vehiculo.atras();

					}else if(datos[0]=='a'){
						vehiculo.izquierda();

					}else if(datos[0]=='d'){
						vehiculo.derecha();

					}
				}else{
					if(enStepper){
						if(strcmp(datos,"mD")==0){
							motor.moverPaso(true);
							motor.setCantidadPasos(0);
						}else if(strcmp(datos,"mI")==0){
							motor.moverPaso(false);
							motor.setCantidadPasos(0);
						}
					}
					if(strcmp(datos,"analizar")==0){
						analisisTerminado=false;
						if(flagAnalizar==false){
							ctrlVelAnalizar.Start(velocidadAnalisis, velocidadAnalisis, enAnalizar);
						}
					}else if(strcmp(datos,"enStepper")==0){
						ctrlVelAnalizar.Stop();
						motor.setCantidadPasos(0);
						motor.setEnable(true);
						LED_TX.Set(false);
						maquinaEstado=0;

						sentidoHorario=true;

						enStepper=true;
					}else if(strcmp(datos,"disStepper")==0){
						analisisTerminado=true;
						motor.setEnable(false);
						enStepper=false;
					}
				}
			}else{
				if(strcmp(datos,"stop")==0){
					ctrlVelAnalizar.Stop();
					motor.setCantidadPasos(0);
					analisisTerminado=true;
					maquinaEstado=0;
					sentidoHorario=true;
				}
			}


			if(strncmp(datos,"motor:",6)==0){
				uint8_t valor=atoi((const char*)(&datos[6]));
				vehiculo.cambiarVelocidad(valor);
			}else if(strncmp(datos,"T:",2)==0){
				uint16_t valor=atoi((const char*)(&datos[2]));
				velocidadAnalisis=valor;
				ctrlVelAnalizar.setTimeReload(velocidadAnalisis);

			}
		}

		if(flagAnalizar){
			flagAnalizar=false;
			analizar();
		}

    }
}













/*
 * FUNCIONES IMPLEMENTADAS
 */


void analizar(void){
	uint32_t promedio=0;
	if(motor.getCantidadPasos()<=CANTPASOS180 && analisisTerminado==false){
		//ETAPA TOMA DE DATOS
		pkt.analizando=true;


		for(uint8_t veces=0; veces<MAX_MUESTRAS; veces++){
			promedio=promedio+laser.getDistanciaMm();
		}
		promedio=promedio/MAX_MUESTRAS;
		pkt.distanciaMm=promedio;

		promedio=0;
		if(!anguloInicial){
			for(uint8_t veces=0;veces<MAX_MUESTRAS; veces++){
				promedio=promedio+brujula.getGrados();
			}
			promedio=promedio/MAX_MUESTRAS;
			pkt.grados=promedio;
			anguloInicial=true;
		}

		pkt.cantidadPasos=motor.getCantidadPasos();
		pkt.checksum=calcularChecksum(&pkt);
		UART0_Send((uint8_t*)&pkt, sizeof(Paquete));
		LED_TX.Set(state);
		state=!state;
		motor.moverPaso(sentidoHorario);

	}else if (motor.getCantidadPasos()>CANTPASOS180){
		maquinaEstado++;
		if(maquinaEstado==1 || maquinaEstado==3 ){
			sentidoHorario=!sentidoHorario;
		}

		motor.setCantidadPasos(0);
		if(maquinaEstado==4){
			//enviamos que termino el analisis

			ctrlVelAnalizar.Stop();
			pkt.analizando=false;
			pkt.cantidadPasos=0;
			pkt.checksum=calcularChecksum(&pkt);
			UART0_Send((uint8_t*)&pkt,  sizeof(Paquete));
			LED_TX.Set(false);
			sentidoHorario=true;
			analisisTerminado=true;
			anguloInicial=false;
			maquinaEstado=0;
		}
	}
}


/*
 * FLAGS PARA ACTIVAR FUNCIONES
 */
void enAnalizar(void){
	flagAnalizar=true;
}

/*
 * conversor valores:
 * val<3600 abierto
 * val>4000 cerrado
 */
