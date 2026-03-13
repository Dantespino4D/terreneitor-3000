#include <stdio.h>
#include "freertos/FreeRTOS.h" //Carga el nucleo del sistema operativo
#include "freertos/task.h"


#include "esp_mac.h"
#include "esp_now.h"
#include "nvs_flash.h"
#include "esp_wifi.h"
#include "string.h"

#include "LEDRGB.h"
#include "Datos.h"
#include "MI_ESPNOW.h"

LedRGB luces;
Mi_Espnow now;

Datos mensajeDatos;
bool nueva_configuracion_colores = false;


void recibirDatos_espnow(const esp_now_recv_info_t *info, const uint8_t *datos_entrantes, int longitud);
//metodo mac address
//prototipos de las funciones de las tareas
void desempaquetar(void* pvParameters);
void movimiento(void* pvParameters);




//APP MAIN


extern "C" void app_main(void){
    luces.begin();
    uint8_t mac[6];

    now.begin();
    esp_read_mac(mac, ESP_MAC_WIFI_STA);
	//se crean las tareas
	xTaskCreatePinnedToCore(desempaquetar, "recibir", 2048, NULL, 1, NULL, 0);
	xTaskCreatePinnedToCore(movimiento, "mover", 2048, NULL, 1, NULL, 1);
}


//RECIBIR


void desempaquetar(void* pvParameters){
	while(1){
		//logica para recibir datos del control
    	esp_now_register_recv_cb(recibirDatos_espnow);
	}
}


//MOVER


void movimiento(void* pvParameters){
	while(1){
		//logica para mover fisicamente al robot

		if(nueva_configuracion_colores == true)
        {

            printf("LED actualizado a R:%d G:%d B:%d\n", mensajeDatos.rojo, mensajeDatos.verde, mensajeDatos.azul);
            nueva_configuracion_colores = false;

            if (mensajeDatos.x > 4000) //arriba
            {
                luces.encenderLed(0, 0, mensajeDatos.azul);
            }
            else if (mensajeDatos.x < 100) //abajo
            {
                luces.encenderLed(0, mensajeDatos.verde, 0);
            }
            else if (mensajeDatos.y > 4000) //derecha
            {
                luces.encenderLed(mensajeDatos.rojo, 0, 0);
            }
            else if (mensajeDatos.y < 100) //izquierda
            {
                luces.encenderLed(mensajeDatos.rojo, mensajeDatos.verde, mensajeDatos.azul);
            }
            else if ((mensajeDatos.x > 1000 && mensajeDatos.x < 2000) && (mensajeDatos.y < 2000 && mensajeDatos.y > 1000))
            {
                luces.encenderLed(0, 0, 0);
            }
            vTaskDelay(50 / portTICK_PERIOD_MS);
        }
	}
}


void recibirDatos_espnow(const esp_now_recv_info_t *info, const uint8_t *datos_entrantes, int longitud){
    if (longitud == sizeof(Datos)){

        memcpy(&mensajeDatos, datos_entrantes, sizeof(Datos));

        printf("\nPaquete Recibido\n");
        printf("Orden recibida -> X: %d, Y: %d, Encendido: %d, Vel: %d, R: %d, G: %d, B: %d\n", mensajeDatos.x, mensajeDatos.y, mensajeDatos.encender, mensajeDatos.vel, mensajeDatos.rojo, mensajeDatos.verde, mensajeDatos.azul);

        nueva_configuracion_colores = true;
    }
    else
        printf("Errror al recibir paquete de datos\n");

}
