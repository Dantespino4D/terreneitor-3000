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
#include "mi_antena.h"
#include "Motores.h"

#define PIN_STBY 4
#define PIN_AIN1 5
#define PIN_AIN2 6
#define PIN_PWMA 7
#define PIN_BIN1 15
#define PIN_BIN2 16
#define PIN_PWMB 17

LedRGB luces;
MiAntena now;
Motores motor(PIN_STBY, PIN_AIN1, PIN_AIN2, PIN_PWMA, PIN_BIN1, PIN_BIN2, PIN_PWMB);

uint8_t mac[6];

Datos mensajeDatos;
bool nueva_configuracion_colores = false;

//metodo mac address
void recibirDatos_espnow(const esp_now_recv_info_t *info, const uint8_t *datos_entrantes, int longitud);

//prototipos de las funciones de las tareas
void movimiento(void* pvParameters);

//declaracion funcion para escalar valores
long mapear(long x, long in_min, long in_max, long out_min, long out_max);

//APP MAIN

extern "C" void app_main(void){
    luces.begin();
    
    now.begin();
    now.encenderWiFi(true);

    motor.begin();

    esp_read_mac(mac, ESP_MAC_WIFI_STA);
    printf("Mac Address: %02X, %02X, %02X, %02X, %02X, %02X\n", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);

    esp_now_register_recv_cb(recibirDatos_espnow);

	//se crean las tareas
	xTaskCreatePinnedToCore(movimiento, "mover", 4096, NULL, 1, NULL, 1);
}

//MOVER


void movimiento(void* pvParameters){
	while(1){
		//logica para mover fisicamente al robot

		if(nueva_configuracion_colores == true)
        {

            printf("LED actualizado a R:%d G:%d B:%d\n", mensajeDatos.rojo, mensajeDatos.verde, mensajeDatos.azul);
            nueva_configuracion_colores = false;

            //bool frenoDeMano = (mensajeDatos.boton_joystick == 0);
            bool frenoDeMano = false;

            // ... (Adentro del while)
            if (frenoDeMano) {
                motor.frenar();
            } 
            else {
                // 1. Prioridad 1: Acelerar o Retroceder (Eje Y)
                if (mensajeDatos.y > 1900) {
                    int vel = mapear(mensajeDatos.y, 1900, 5000, 0, 150);
                    if (vel > 150) vel = 150; 
                    motor.avanzar(vel);
                    printf("Velocidad: %d", vel);
                    luces.encenderLed(0, 0, mensajeDatos.azul);
                } 
                else if (mensajeDatos.y < 1700) {
                    int vel = mapear(mensajeDatos.y, 1700, 0, 0, 150);
                    if (vel > 150) vel = 150;
                    motor.retroceder(vel);
                    printf("Velocidad: %d", vel);
                    luces.encenderLed(0, mensajeDatos.verde, 0);
                } 
                // 2. Prioridad 2: Si no estamos acelerando, revisamos si queremos Girar (Eje X)
                else if (mensajeDatos.x > 1900) {
                    // Girar a la DERECHA
                    int vel = mapear(mensajeDatos.x, 1900, 5000, 0, 150);
                    if (vel > 150) vel = 150;
                    motor.girarDerecha(vel);
                    printf("Velocidad: %d", vel);
                    luces.encenderLed(mensajeDatos.verde, 0, 0);
                }
                else if (mensajeDatos.x < 1700) {
                    // Girar a la IZQUIERDA
                    int vel = mapear(mensajeDatos.x, 1700, 0, 0, 150);
                    if (vel > 150) vel = 150;
                    motor.girarIzquierda(vel);
                    printf("Velocidad: %d", vel);
                    luces.encenderLed(mensajeDatos.rojo, mensajeDatos.verde, mensajeDatos.azul);
                }
                // 3. Prioridad 3: Soltamos por completo el control
                else {
                    motor.puntoMuerto();
                }
            }
           //motor.universal(mensajeDatos.x, mensajeDatos.y);
        }
        vTaskDelay(20 / portTICK_PERIOD_MS);
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
    {
        printf("Errror al recibir paquete de datos\n");
        nueva_configuracion_colores = false;
    }
}

long mapear(long x, long in_min, long in_max, long out_min, long out_max) {
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

