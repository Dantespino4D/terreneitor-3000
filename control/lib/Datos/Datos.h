#ifndef DATOS_H
#define DATOS_H
#include <cstdint>

struct Datos {
		uint16_t x;
		uint16_t y;
		uint8_t encender;
		uint8_t vel;
		//bool continuar;
		//bool modo;
	}__attribute__((packed));
#endif
