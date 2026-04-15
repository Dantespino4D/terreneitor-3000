#ifndef DATOS_H
#define DATOS_H

#include <stdint.h>

	struct Datos {
		uint8_t rojo;
  	 	uint8_t verde;
   		uint8_t azul;
		uint16_t x;
		uint16_t y;
		uint8_t encender;
		uint8_t cambioVel;
		uint8_t mantenerVel;
		uint8_t clackson;
		uint8_t ventiladores;
		uint8_t boton0;
		uint8_t botonJoystick;
	}__attribute__((packed)); 
#endif

