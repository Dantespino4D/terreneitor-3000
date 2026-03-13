#include "LEDRGB.h"

#define LED_RGB 48

LedRGB::LedRGB(){
}

void LedRGB::begin(){
    //1. Configuramos el PIN y cantidad de LEDs
    led_strip_config_t strip_cfg = {.strip_gpio_num = LED_RGB, .max_leds = 1};

    //2. Configuramos el motor de datos (RMT)
    led_strip_rmt_config_t rmt_cfg = {.resolution_hz = 10 * 1000 * 1000}; //10MHz para que sea preciso /*o escribir ¨10'000'000*/

    //3. Inicializamos el hardware
    led_strip_new_rmt_device(&strip_cfg, &rmt_cfg, &direccionLedRGB);

}

void LedRGB::encenderLed(uint8_t r, uint8_t g, uint8_t b){
    led_strip_set_pixel(direccionLedRGB, 0, r, g, b); //El cero aqui significa donde comienza a brillar (como un arreglo, 0 = 1er led) si escribimos 2, enciende el 3er led, preparamos la informacion antes de ejecutar
    led_strip_refresh(direccionLedRGB); //"Refrescar" es lo quye realmente manda la señal
}

void LedRGB::apagarLed(){
    led_strip_clear(direccionLedRGB);
}