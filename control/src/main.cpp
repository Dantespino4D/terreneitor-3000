#include "freertos/FreeRTOS.h"

//prototipo de la funcion de la tarea
//objeto para inicializar
//void procesardorAnalogico(void* pvParameters);
void enviar(void* pvParameters);//y empaquetar


//APP MAIN


extern "C" void app_main() {
	//se crea la tarea de enviar
	xTaskCreatePinnedToCore(enviar, "enviar", 2048, NULL, 1, NULL, 1);
}


//ENVIAR


void enviar(void* pvParameters) {
	while (true) {
		//logica para enviar datos al terrneitorior
	}
}
