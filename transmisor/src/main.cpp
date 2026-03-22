#include "freertos/FreeRTOS.h"
#include "esp_mac.h"
#include "esp_now.h"
#include "nvs_flash.h"
#include "esp_wifi.h"
#include "esp_netif.h"
#include "esp_event.h"
#include "Controles/Controles.h"
#include "Datos.h"
#include "Mi_Antena.h"
#include "Mqtt.h"

//objeto que recopila las lecturas de los controles
Controles controles(ADC1_CHANNEL_6, ADC1_CHANNEL_7);
MiAntena now;
Mqtt mqtt;

//estructura de datos
Datos estructuraControl = {2048, 2048, 0, 0, 255, 255, 255, 0, 0, 0, 0};
uint8_t mac[6] = {0x1C, 0xDB, 0xD4, 0x47, 0X01, 0xD4};

//prototipo de la funcion de la tarea
//objeto para inicializar
//void procesardorAnalogico(void* pvParameters);
void enviar(void* pvParameters);//y empaquetar


//APP MAIN

extern "C" void app_main() { //se inicializan pines y otras cosas de los controles

	controles.begin();
	now.begin();
	now.encenderWiFi(true); // true = usar ESP-NOW
	//mqtt.begin();
	now.agregarMacAddress(mac);
	now.expediente();

	//se crea la tarea de enviar
	xTaskCreatePinnedToCore(enviar, "enviar", 2048, NULL, 1, NULL, 1);
}

//ENVIAR


void enviar(void* pvParameters) {
	Datos valoresAnteriores = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};

	uint32_t ultimoMovimiento = 0;
	const uint32_t TIEMPO_ESPERA_MS = 10000; // 10 segundos de inactividad para apagar
    bool antenaPrendida = true; // Empieza prendida porque la encendimos en app_main

	bool cambioX;
	bool cambioY;
	bool cambioBotones;

	while (true) {
		//logica para enviar datos al terrneitorior

		//se llena el struct
		controles.empaquetar(&estructuraControl);

		cambioX = (abs(estructuraControl.x - 2048) > 100);
        cambioY = (abs(estructuraControl.y - 2048) > 100);
		cambioBotones = (estructuraControl.encender != valoresAnteriores.encender) ||
                        (estructuraControl.vel != valoresAnteriores.vel) ||
                        (estructuraControl.continuar != valoresAnteriores.continuar) ||
                        (estructuraControl.modo != valoresAnteriores.modo) ||
                        (estructuraControl.boton1 != valoresAnteriores.boton1) ||
                        (estructuraControl.boton2 != valoresAnteriores.boton2);

		if (cambioX || cambioY || cambioBotones)
        {
            // Actualizamos el reloj porque nos acabamos de mover
            ultimoMovimiento = xTaskGetTickCount() * portTICK_PERIOD_MS;

            // Si la antena estaba dormida, la despertamos
            if (!antenaPrendida) {
                now.encenderWiFi(true); // true = usar ESP-NOW

                // Como bien dijiste, organizamos esto por separado:
                now.agregarMacAddress(mac);
                now.expediente();

                antenaPrendida = true;
            }

            // Enviamos y actualizamos estado
            printf("Datos X: %d, Y: %d, encender: %d, vel: %d, R: %d, G: %d, B: %d, Cont: %d, Modo: %d, B1: %d, B2: %d\n",
                   estructuraControl.x, estructuraControl.y, estructuraControl.encender, estructuraControl.vel,
                   estructuraControl.rojo, estructuraControl.verde, estructuraControl.azul,
                   estructuraControl.continuar, estructuraControl.modo, estructuraControl.boton1, estructuraControl.boton2);
            now.empaquetar(&estructuraControl);
            valoresAnteriores = estructuraControl;
        }

        // 2. ¿LLEVAMOS MUCHO TIEMPO SIN MOVERNOS?
        uint32_t tiempoActual = xTaskGetTickCount() * portTICK_PERIOD_MS;

        if (antenaPrendida && ((tiempoActual - ultimoMovimiento) > TIEMPO_ESPERA_MS))
		{
            // Pasaron los 10 segundos sin actividad. ¡A dormir!
            now.apagarWiFi(); // true = apagar también ESP-NOW
            antenaPrendida = false;
        }
        vTaskDelay(20 / portTICK_PERIOD_MS);
    }
}

