#ifndef MOTORES_H
#define MOTORES_H

#include <stdint.h>
#include "driver/gpio.h"

class Motores{
    private:
        // Variables para guardar los números de pin que le pasaremos
        gpio_num_t pin_stby;
        gpio_num_t pin_ain1;
        gpio_num_t pin_ain2;
        gpio_num_t pin_pwma;
        gpio_num_t pin_bin1;
        gpio_num_t pin_bin2;
        gpio_num_t pin_pwmb;

		int16_t velocidad1;
		int16_t velocidad2;
		uint8_t direccion1a;
		uint8_t direccion2a;
		uint8_t direccion1b;
		uint8_t	direccion2b;
    public:
        // El Constructor: Aquí le inyectaremos los pines cuando creemos el objeto
        Motores(int stby, int ain1, int ain2, int pwma, int bin1, int bin2, int pwmb);

        // Método para configurar los pines (nuestro pinMode avanzado)
        void begin();

        // Acciones del carrito (piden una velocidad de 0 a 255)
        void avanzar(uint16_t velocidad);
        void retroceder(uint16_t velocidad);
        void girarIzquierda(uint16_t velocidad);
        void girarDerecha(uint16_t velocidad);
        void frenar(); // Detiene el coche de golpe
        void puntoMuerto(); // Deja los motores al aire para que se vayan frenando solos
		void universal(int16_t y, int16_t y);
		void velocidad(int16_t x, int16_t y);
};

#endif
