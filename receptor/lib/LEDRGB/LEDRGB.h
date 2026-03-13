#ifndef LEDRGB_H
#define LEDRGB_H

#include "led_strip.h"
#include <stdint.h>

class LedRGB{

    private:
        led_strip_handle_t direccionLedRGB;


    public:
        LedRGB();

        void begin();
        void encenderLed(uint8_t r, uint8_t g, uint8_t b);
        void apagarLed();
};

#endif