#include "Boton.h"

Boton::Boton(gpio_num_t pin_asignado) {
    pin = pin_asignado;
}

void Boton::begin() {
    gpio_reset_pin(pin);
    gpio_set_direction(pin, GPIO_MODE_INPUT);
    // Activamos la resistencia interna hacia 3.3V
    // Así, cuando no lo presionas lee 1, y al presionarlo (conecta a GND) lee 0.
    gpio_set_pull_mode(pin, GPIO_PULLUP_ONLY); 
}

bool Boton::presionado() {
    // Retorna true si el pin está en 0V (GND)
    return gpio_get_level(pin) == 0;
}