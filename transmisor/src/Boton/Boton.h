#ifndef BOTON_H
#define BOTON_H

#include "driver/gpio.h"

class Boton {
private:
    gpio_num_t pin;

public:
    // Constructor
    Boton(gpio_num_t pin_asignado);
    
    // Configura el pin
    void begin();
    
    // Lee el estado
    bool presionado();
};

#endif