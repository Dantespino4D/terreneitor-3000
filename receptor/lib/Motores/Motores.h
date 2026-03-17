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

    public:
        // El Constructor: Aquí le inyectaremos los pines cuando creemos el objeto
        Motores(int stby, int ain1, int ain2, int pwma, int bin1, int bin2, int pwmb);

        // Método para configurar los pines (nuestro pinMode avanzado)
        void begin();

        // Acciones del carrito (piden una velocidad de 0 a 255)
        void avanzar(uint8_t velocidad);
        void retroceder(uint8_t velocidad);
        void girarIzquierda(uint8_t velocidad);
        void girarDerecha(uint8_t velocidad);
        void frenar(); // Detiene el coche de golpe
        void puntoMuerto(); // Deja los motores al aire para que se vayan frenando solos
};

#endif