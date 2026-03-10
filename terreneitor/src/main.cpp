#include "freertos/FreeRTOS.h"

//metodo mac address
//prototipos de las funciones de las tareas
void desempaquetar(void* pvParameters);
void movimiento(void* pvParameters);




//APP MAIN


extern "C" void app_main(void){
	//se crean las tareas
	xTaskCreatePinnedToCore(desempaquetar, "recibir", 2048, NULL, 1, NULL, 0);
	xTaskCreatePinnedToCore(movimiento, "mover", 2048, NULL, 1, NULL, 1);
}


//RECIBIR


void desempaquetar(void* pvParameters){
	while(1){
		//logica para recibir datos del control
	}
}


//MOVER


void movimiento(void* pvParameters){
	while(1){
		//logica para mover fisicamente al robot
	}
}
