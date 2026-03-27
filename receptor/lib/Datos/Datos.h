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
		uint8_t continuar;
		uint8_t modo;
		uint8_t boton1;
		uint8_t boton2;
	}__attribute__((packed)); 
#endif
