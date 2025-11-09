#pragma once

#define MAX_CALLBACK 50

class CALLBACK {

public:
	CALLBACK();

	/*Este método debe ser implementado por las clases
	derivadas cada una resolverá que hacer con su Irq
	enganchada al  systick del sistema	*/
	virtual void Callback( void );
	/*
	 * Virtual: cuando otra clase hereda CALLBACK puede usar el mismo nombre del metodo (Callback)
	 */

	static void SysTickCall(void);
	/*
	 * Static puedo llamar a este metodo sin declarar un objeto
	 */
};


