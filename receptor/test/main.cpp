#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "Motores.h" // ¡Tu nueva y flamante librería!

// =========================================================
// 1. DEFINICIÓN DE PINES Y OBJETOS GLOBALES
// =========================================================

// Usamos las macros #define para que sea fácil cambiarlos en el futuro
#define PIN_STBY 4
#define PIN_AIN1 5
#define PIN_AIN2 6
#define PIN_PWMA 7
#define PIN_BIN1 15
#define PIN_BIN2 16
#define PIN_PWMB 17

// ¡Nace el Ferrari! Le inyectamos los pines al constructor
Motores motor(PIN_STBY, PIN_AIN1, PIN_AIN2, PIN_PWMA, PIN_BIN1, PIN_BIN2, PIN_PWMB);

// =========================================================
// 2. ESTRUCTURA DE DATOS (Simulación de tu Radio)
// =========================================================

// Esta es la "caja" donde guardas lo que llega del control remoto
typedef struct {
    int eje_x;          // ¡Nuevo! Rango de 0 a 5000 (Izquierda a Derecha)
    int eje_y;          // Rango de 0 a 5000 (Atrás hacia Adelante)
    int boton_joystick; // 0 = Presionado, 1 = Suelto
} DatosControl;

DatosControl datos_recibidos; // Variable global para guardar los datos en tiempo real

// =========================================================
// 3. FUNCIONES AUXILIARES
// =========================================================

// Función matemática para escalar valores (El map() de C++)
long mapear(long x, long in_min, long in_max, long out_min, long out_max) {
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

// =========================================================
// 4. EL PROGRAMA PRINCIPAL
// =========================================================

extern "C" void app_main(void) {
    printf("Arrancando sistema del Ferrari...\n");

    // Inicializamos el hardware de los motores (AQUÍ despierta el ESP32)
    motor.begin();

    // Damos valores iniciales seguros para evitar que arranque a lo loco
    datos_recibidos.eje_y = 1800;       // Centro absoluto (Zona muerta)
    datos_recibidos.boton_joystick = 1; // Botón sin presionar

    // Ciclo infinito (FreeRTOS)
    while (true) {
        
        // -----------------------------------------------------------------
        // AQUÍ IRÍA TU CÓDIGO DE ESP-NOW PARA ACTUALIZAR 'datos_recibidos'
        // -----------------------------------------------------------------

        // Leemos si el piloto activó el freno de mano
        bool frenoDeMano = (datos_recibidos.boton_joystick == 0);

        // ... (Adentro del while)
        if (frenoDeMano) {
            motor.frenar();
        } 
        else {
            // 1. Prioridad 1: Acelerar o Retroceder (Eje Y)
            if (datos_recibidos.eje_y > 1900) {
                int vel = mapear(datos_recibidos.eje_y, 1900, 5000, 0, 255);
                if (vel > 255) vel = 255; 
                motor.avanzar(vel);
            } 
            else if (datos_recibidos.eje_y < 1700) {
                int vel = mapear(datos_recibidos.eje_y, 1700, 0, 0, 255);
                if (vel > 255) vel = 255;
                motor.retroceder(vel);
            } 
            // 2. Prioridad 2: Si no estamos acelerando, revisamos si queremos Girar (Eje X)
            else if (datos_recibidos.eje_x > 1900) {
                // Girar a la DERECHA
                int vel = mapear(datos_recibidos.eje_x, 1900, 5000, 0, 255);
                if (vel > 255) vel = 255;
                motor.girarDerecha(vel);
            }
            else if (datos_recibidos.eje_x < 1700) {
                // Girar a la IZQUIERDA
                int vel = mapear(datos_recibidos.eje_x, 1700, 0, 0, 255);
                if (vel > 255) vel = 255;
                motor.girarIzquierda(vel);
            }
            // 3. Prioridad 3: Soltamos por completo el control
            else {
                motor.puntoMuerto();
            }
        }
        // ¡SÚPER IMPORTANTE! El respiro del microcontrolador
        // Le damos 20 milisegundos a FreeRTOS para que atienda el WiFi y otros procesos
        vTaskDelay(20 / portTICK_PERIOD_MS); 
    }
}