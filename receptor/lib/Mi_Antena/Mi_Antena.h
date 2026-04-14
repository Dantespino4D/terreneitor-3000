#ifndef MI_ANTENA_H
#define MI_ANTENA_H

#include "Datos.h"
#include <stdint.h>
#include <esp_now.h> // Libreria para utilizar esp-now

class  MiAntena{
    private:
        uint8_t macReceptor[6];
        esp_now_peer_info_t expedienteReceptor;


    public:
        MiAntena();

        void begin();
        void encenderWiFi(bool activareEspnow);
        void agregarMacAddress(const uint8_t* nuevaMac);
        void expediente();
        void empaquetar(Datos* paquete);
        void apagarWiFi();
		void conectarWiFi();
};

#endif


