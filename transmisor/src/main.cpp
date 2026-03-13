#include "freertos/FreeRTOS.h"
#include "esp_mac.h"
#include "esp_now.h"
#include "nvs_flash.h"
#include "esp_wifi.h"
#include "esp_netif.h"
#include "esp_event.h"
#include "Controles/Controles.h"
#include "Datos.h"
#include "MI_ESPNOW.h"

//objeto que recopila las lecturas de los controles
Controles controles(ADC1_CHANNEL_0, ADC1_CHANNEL_3);
Mi_Espnow paqueteEnviar;

//estructura de datos
Datos estructuraControl = {2048, 2048, false, false/*,false,false*/};
uint8_t mac[6] = {0x1C, 0xDB, 0xD4, 0x47, 0X01, 0xD4};

//prototipo de la funcion de la tarea
//objeto para inicializar
//void procesardorAnalogico(void* pvParameters);
void enviar(void* pvParameters);//y empaquetar


//APP MAIN


extern "C" void app_main() {
	//se inicializan pines y otras cosas de los controles
	controles.begin();

    paqueteEnviar.begin();
    paqueteEnviar.agregarMacAddress(mac);
    paqueteEnviar.expediente();

	//se crea la tarea de enviar
	xTaskCreatePinnedToCore(enviar, "enviar", 2048, NULL, 1, NULL, 1);
}


//ENVIAR


void enviar(void* pvParameters) {
	while (true) {
		//logica para enviar datos al terrneitorior

		//se llena el struct
        vTaskDelay(50 / portTICK_PERIOD_MS);
		controles.empaquetar(&estructuraControl);
		vTaskDelay(pdMS_TO_TICKS(10));
        printf("X: %d, Y: %d, Encendido: %d, Vel: %d, R: %d, G: %d, B: %d\n", estructuraControl.x, estructuraControl.y, estructuraControl.encender, estructuraControl.vel, estructuraControl.rojo, estructuraControl.verde, estructuraControl.azul);
        paqueteEnviar.empaquetar(&estructuraControl);
	}
}
