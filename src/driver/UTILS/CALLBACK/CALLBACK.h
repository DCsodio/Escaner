/*
 * CALLBACK.h
 *
 *  Created on: 23 ago. 2023
 *      Author: gusta
 */

#ifndef UTILS_CALLBACK_CALLBACK_H_
#define UTILS_CALLBACK_CALLBACK_H_
#define MAX_CALLBACK 10

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

#endif /* UTILS_CALLBACK_CALLBACK_H_ */
