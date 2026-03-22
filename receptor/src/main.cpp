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
#include "Mi_Antena.h"
#include "Mqtt.h"

LedRGB luces;
MiAntena now;
Mqtt mqtt;

uint8_t mac[6];

Datos mensajeDatos;
bool nueva_configuracion_colores = false;

//metodo mac address
void recibirDatos_espnow(const esp_now_recv_info_t *info, const uint8_t *datos_entrantes, int longitud);

//prototipos de las funciones de las tareas
void movimiento(void* pvParameters);

//APP MAIN

extern "C" void app_main(void){
    luces.begin();

    now.begin();
    now.encenderWiFi(true);
	//mqtt.begin();

    esp_read_mac(mac, ESP_MAC_WIFI_STA);
    printf("Mac Address: %02X, %02X, %02X, %02X, %02X, %02X\n", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);

    esp_now_register_recv_cb(recibirDatos_espnow);

	//se crean las tareas
	xTaskCreatePinnedToCore(movimiento, "mover", 2048, NULL, 1, NULL, 1);
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
        }
        vTaskDelay(20 / portTICK_PERIOD_MS);
	}
}


void recibirDatos_espnow(const esp_now_recv_info_t *info, const uint8_t *datos_entrantes, int longitud){
    if (longitud == sizeof(Datos)){

        memcpy(&mensajeDatos, datos_entrantes, sizeof(Datos));

        printf("\nPaquete Recibido\n");
        printf("Orden recibida -> X: %d, Y: %d, Encendido: %d, Vel: %d, R: %d, G: %d, B: %d, Cont: %d, Modo: %d, B1: %d, B2: %d\n",
               mensajeDatos.x, mensajeDatos.y, mensajeDatos.encender, mensajeDatos.vel,
               mensajeDatos.rojo, mensajeDatos.verde, mensajeDatos.azul,
               mensajeDatos.continuar, mensajeDatos.modo, mensajeDatos.boton1, mensajeDatos.boton2);

        nueva_configuracion_colores = true;
    }
    else
    {
        printf("Errror al recibir paquete de datos\n");
        nueva_configuracion_colores = false;
    }
}

