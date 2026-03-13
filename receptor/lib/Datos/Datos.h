#ifndef DATOS_H
#define DATOS_H

#include <stdint.h>

	struct Datos {
		uint16_t x;
		uint16_t y;
		uint8_t encender;
		uint8_t vel;
		uint8_t rojo;
  	 	uint8_t verde;
   		uint8_t azul;
		//bool continuar;
		//bool modo;
	}__attribute__((packed)); 
#endif
