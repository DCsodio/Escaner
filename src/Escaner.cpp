#include "Defines.h"
#include "driver/PLL/dr_pll.h"
#include "Uart0.h"
#include "Systick.h"
#include "laserHl.h"
#include "convert.h"
#include "hmc5883l.h"
#include "protocoloComu.h"
#include <string.h>



#pragma pack(push, 1)
typedef struct {
	uint8_t header[2];
    char nombre[50];
    uint32_t edad;
    uint32_t altura;
} Persona;
#pragma pack(pop)

/*
#pragma pack(push, 1)
typedef struct {
    uint16_t header;
    Persona person;
    uint32_t CHECKSUM;
} Paquete;
#pragma pack(pop)*/

Persona pers;
//Paquete paq;
uint16_t distanciaMm=0;

char cadena[10];

HMC5883L_LPC845 mag;
hmc5883_data_t data;

char angulo[20];
protocoloComu proto('}');
void enviar (void);
SYSTICK systick(1000);
TIMER temporizador (1000, 1000, enviar);

int main(void)
{

	Inicializar_PLL();

	UART0_Init(115200);
	i2c_init();
	pers.header[0]='{';
	pers.header[1]='{';
	pers.altura=10;
	pers.edad=23;
	strcpy(pers.nombre,"copied");

	//laserHl laser;
	//laser.configurarLaser(2, 50, 100);
	//laser.iniciarLaser();

	 //if (!mag.begin()) {

	     //   while (1);
	   // }
	 //mag.setMagGain(HMC5883_MAGGAIN_8_1);
	//paq.person=pers;
	 //  paq.header=0xAA55;
	//    	paq.CHECKSUM=32;
	for (volatile int i = 0; i < 70000; i++);
	UART0_Send((uint8_t*)&pers,  sizeof(Persona));

	//UART0_SendBlocking((uint8_t*)&pers,  sizeof(Persona));

	pers.header[0]='{';
		pers.header[1]='A';
		pers.altura=10;
		pers.edad=23;
		strcpy(pers.nombre,"copied");
		UART0_Send((uint8_t*)&pers,  sizeof(Persona));

//		UART0_SendBlocking((uint8_t*)&pers,  sizeof(Persona));
		for (volatile int i = 0; i < 70000; i++);
		pers.header[0]='{';
				pers.header[1]='{';
				pers.altura=30;
				pers.edad=23;
				strcpy(pers.nombre,"cSSSSSopied");
	//			UART0_SendBlocking((uint8_t*)&pers,  sizeof(Persona));
				UART0_Send((uint8_t*)&pers,  sizeof(Persona));

				while (1)
    {

    	//laser.leerDistanciaMm();
    	//distanciaMm=laser.getDistanciaMm();
    	//int_to_str(distanciaMm,cadena);

    	//for (volatile int i = 0; i < 1000; i++);

    	//if (mag.readData(&data)) {
			//int_to_str((uint16_t)data.heading_deg,angulo);

			//UART0_Send((uint8_t*)"Angulo: ",0);
			//UART0_Send((uint8_t*)angulo,0);
			//UART0_Send((uint8_t*)"\r\n",0);
    	//}
    	//UART0_Send((uint8_t*)"Distancia: ", 0);
    	    //	UART0_Send((uint8_t*)cadena, 0);
    	    //	UART0_Send((uint8_t*)"\r\n", 0);


    	//UART0_Send((uint8_t*)&pers, sizeof(Persona));
    	//UART0_Send((uint8_t*)"Hola\n", 6);

    	for (volatile int i = 0; i < 70000; i++);

    }
}

void enviar (void){
	//char buffer[128];
	//int idx = 0;

	//idx +s= sprintf(buffer + idx, "{");           // abre JSON
	//idx += sprintf(buffer + idx, "\"temp\":%d,", 25); // valor entero
	//idx += sprintf(buffer + idx, "\"volt\":%.2f", 3.14); // valor float
	//idx += sprintf(buffer + idx, "}");
	//proto.enviar(buffer);

}

