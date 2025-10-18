#include "Defines.h"
#include "driver/PLL/dr_pll.h"
#include "Uart0.h"
#include "Systick.h"
#include "laserHl.h"
#include "convert.h"
#include "hmc5883l.h"
#include <string.h>


#pragma pack(push, 1)
typedef struct {
	uint8_t header[2];
	bool analizando=true;
	uint16_t distanciaMm=0;
	float grados=0;
	uint32_t checksum;

} Paquete;
#pragma pack(pop)

uint32_t calcularChecksum(Paquete* pkt);
Paquete pkt;

HMC5883L_LPC845 mag;



int main(void)
{
	Inicializar_PLL();
	UART0_Init(115200);
	i2c_init();

	laserHl laser;
	laser.configurarLaser(2, 50, 100);
	laser.iniciarLaser();

	if (!mag.begin()) {
	        while (1);
	 }

	 mag.setMagGain(HMC5883_MAGGAIN_8_1);

		//UART0_SendBlocking((uint8_t*)&pers,  sizeof(Persona));
	while (1){

		pkt.header[0]='{';
		pkt.header[0]='{';
		pkt.analizando=true;
    	laser.leerDistanciaMm();
    	pkt.distanciaMm=laser.getDistanciaMm();
    	pkt.grados=mag.getHeading();
    	pkt.checksum=calcularChecksum(&pkt);
    	for (volatile int i = 0; i < 70000; i++);
    	UART0_SendBlocking((uint8_t*)&pkt,  sizeof(Paquete));
    }
}


uint32_t calcularChecksum(Paquete* pkt) {
    uint32_t sum = 0;

    for (size_t i = 0; i < sizeof(Paquete) - sizeof(pkt->checksum); i++) {
        sum += ((uint8_t*)pkt)[i];  // convertimos pkt a bytes directamente
    }

    return sum;
}
