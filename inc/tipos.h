#ifndef TIPOS_H_
#define TIPOS_H_

#define  	__I    				volatile const  /*Defines 'read only' permission */
#define  	__O    				volatile        /*Defines 'write only' permission */
#define  	__IO   				volatile        /*Defines 'read / write' permission */


//#include <cstdint>  ESTO SE USA EN C++ ENO EN MCUS

#include <stdint.h> //esto sumplanta a lo de abajo es std
/*
typedef		unsigned char 		uint8_t;
typedef		char 				int8_t;
typedef		short unsigned int 	uint16_t;
typedef		short  int 			int16_t;
typedef		unsigned int 		uint32_t;
typedef		int 				int32_t;
*/

#endif /* TIPOS_H_ */
