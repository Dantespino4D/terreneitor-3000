#ifndef MI_ESPNOW_H
#define MI_ESPNOW_H

#include "Datos.h"
#include <stdint.h>
#include <esp_now.h> // Libreria para utilizar esp-now

class  Mi_Espnow{
    private:
        uint8_t macReceptor[6];
        esp_now_peer_info_t expedienteReceptor;
    
    
    public:
        Mi_Espnow();

        void begin();
        void agregarMacAddress(const uint8_t* nuevaMac);
        void expediente();
        void empaquetar(Datos* paquete);
};

#endif


