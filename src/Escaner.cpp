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


Paquete pkt;
int main(void)
{

	Inicializar_PLL();
	UART0_Init(115200);
	i2c_init();
	SYSTICK systick(1000);

	Vehiculo vehiculo(10,0,0,10,10,5,
			PINMOTA1,
			PINMOTA2,
			PINMOTB1,
			PINMOTB2);


	laserHl laser;
	laser.configurarLaser(2, 50, 100);
	laser.iniciarLaser();

	brujulaHl brujula;
	brujula.inicializar(HMC5883_MAGGAIN_8_1);



	pkt.header[0]='{';
	pkt.header[1]='{';
	pkt.analizando=true;
	pkt.distanciaMm=laser.getDistanciaMm();
	pkt.grados=brujula.getGrados();
	pkt.checksum=calcularChecksum(&pkt);
	for (volatile int i = 0; i < 70000; i++);
	UART0_SendBlocking((uint8_t*)&pkt,  sizeof(Paquete));
	while (1){






















    }
}


