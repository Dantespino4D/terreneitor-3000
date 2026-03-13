#include "freertos/FreeRTOS.h"
#include "Controles/Controles.h"
#include "Datos.h"

//objeto que recopila las lecturas de los controles
Controles controles(ADC1_CHANNEL_0, ADC1_CHANNEL_3);

//estructura de datos
Datos datos = {2048, 2048, false, false/*,false,false*/};

//prototipo de la funcion de la tarea
//objeto para inicializar
//void procesardorAnalogico(void* pvParameters);
void enviar(void* pvParameters);//y empaquetar


//APP MAIN


extern "C" void app_main() {
	//se inicializan pines y otras cosas de los controles
	controles.begin();

	//se crea la tarea de enviar
	xTaskCreatePinnedToCore(enviar, "enviar", 2048, NULL, 1, NULL, 1);
}


//ENVIAR


void enviar(void* pvParameters) {
	while (true) {
		//logica para enviar datos al terrneitorior

		//se llena el struct
		controles.empaquetar(&datos);
		vTaskDelay(pdMS_TO_TICKS(10));
	}
}
