#include "freertos/FreeRTOS.h"
#include "esp_mac.h"
#include "esp_now.h"
#include "nvs_flash.h"
#include "esp_wifi.h"
#include "esp_netif.h"
#include "esp_event.h"
#include "Controles/Controles.h"
#include "Datos.h"
#include "mi_antena.h"

//objeto que recopila las lecturas de los controles
Controles controles(ADC1_CHANNEL_6, ADC1_CHANNEL_7);
MiAntena paqueteEnviar;
//Boton btnAdelante(GPIO_NUM_18); 
//Boton btnAtras(GPIO_NUM_19);

//estructura de datos
Datos estructuraControl = {2048, 2048, 0, 0, 255, 255, 255};
//uint8_t mac[6] = {0x1C, 0xDB, 0xD4, 0x47, 0X01, 0xD4}; //ESP-S3 Alex
//uint8_t mac[6] = {0xDC, 0xB4, 0xD9, 0x14, 0X60, 0x70}; //ESP-S3 DANTE

//prototipo de la funcion de la tarea
//objeto para inicializar
//void procesardorAnalogico(void* pvParameters);
void enviar(void* pvParameters);//y empaquetar


//APP MAIN

extern "C" void app_main() { //se inicializan pines y otras cosas de los controles 
	controles.begin();
	paqueteEnviar.begin(); 
    paqueteEnviar.encenderWiFi(true);
	paqueteEnviar.agregarMacAddress(mac);
	paqueteEnviar.expediente(); 

    //btnAdelante.begin();
    //btnAtras.begin();

	//se crea la tarea de enviar 
	xTaskCreatePinnedToCore(enviar, "enviar", 2048, NULL, 1, NULL, 1); 
}

//ENVIAR


void enviar(void* pvParameters) {
	Datos valoresAnteriores = {1, 1, 1, 1, 1, 1, 1};

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

        // 4. INYECTAMOS LOS DATOS FALSOS DEL JOYSTICK
        //estructuraControl.x = 1850; // Eje X siempre en el centro (sin girar)

        /*if (btnAdelante.presionado()) {
            estructuraControl.y = 0;    // Valor máximo hacia un lado
        } 
        else if (btnAtras.presionado()) {
            estructuraControl.y = 4095; // Valor máximo hacia el otro
        } 
        else {
            estructuraControl.y = 2048; // Si no hay nada presionado, se queda quieto
        }
        */

		cambioX = (abs(estructuraControl.x - valoresAnteriores.x) > 50);
        cambioY = (abs(estructuraControl.y - valoresAnteriores.y) > 50);
		cambioBotones = (estructuraControl.encender != valoresAnteriores.encender) || (estructuraControl.vel != valoresAnteriores.vel);

		if (cambioX || cambioY || cambioBotones) 
        {   
            // Actualizamos el reloj porque nos acabamos de mover
            ultimoMovimiento = xTaskGetTickCount() * portTICK_PERIOD_MS;
            
            // Si la antena estaba dormida, la despertamos
            if (!antenaPrendida) {
                paqueteEnviar.encenderWiFi(true); // true = usar ESP-NOW
                
                // Como bien dijiste, organizamos esto por separado:
                paqueteEnviar.agregarMacAddress(mac);
                paqueteEnviar.expediente();
                
                antenaPrendida = true;
            }

            // Enviamos y actualizamos estado
            printf("Datos X: %d, Y: %d, encender: %d, vel: %d, R: %d, G: %d, B: %d\n", estructuraControl.x, estructuraControl.y, estructuraControl.encender, estructuraControl.vel, estructuraControl.rojo, estructuraControl.verde, estructuraControl.azul);
            paqueteEnviar.empaquetar(&estructuraControl);
            valoresAnteriores = estructuraControl;
        }

        // 2. ¿LLEVAMOS MUCHO TIEMPO SIN MOVERNOS?
        uint32_t tiempoActual = xTaskGetTickCount() * portTICK_PERIOD_MS;
        
        if (antenaPrendida && ((tiempoActual - ultimoMovimiento) > TIEMPO_ESPERA_MS)) 
		{
            // Pasaron los 10 segundos sin actividad. ¡A dormir!
            paqueteEnviar.apagarWiFi(); // true = apagar también ESP-NOW
            antenaPrendida = false;
        }
        vTaskDelay(20 / portTICK_PERIOD_MS);
    }
}

