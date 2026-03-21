#include "Motores.h"

#include "driver/ledc.h" // ¡La librería mágica para el PWM del ESP32!

Motores::Motores(int stby, int ain1, int ain2, int pwma, int bin1, int bin2, int pwmb){
    pin_stby = (gpio_num_t)stby; // Encendido: 1 (Alto), mandas encender el chip.  0 (Bajo) mandas a dormir el chip (ahorra bateria)
    pin_ain1 = (gpio_num_t)ain1; // Canal A, las cuales son las llantas izquierda (AIN1 = 1 y AIN2 = 0 -> Adelante), (AIN1 = 0 y AIN2 = 1 -> Atras) 
    pin_ain2 = (gpio_num_t)ain2; 
    pin_pwma = (gpio_num_t)pwma; // Acelerador: Canal A, va de 0 a 255, (Ejemplo: 127 velocidad media)
    pin_bin1 = (gpio_num_t)bin1; // Canal B, las cuales son las llantas derechas
    pin_bin2 = (gpio_num_t)bin2;
    pin_pwmb = (gpio_num_t)pwmb; // Acelerador: Canal B
    //(gpio_num_t) Se le llama casteo, cambias el tipo de dato de una variable a otra
}

void Motores::begin(){
    gpio_reset_pin(pin_stby); // Hacemos un Reset al pin para que este completamente limpio antes de su uso por cualquier cosa
    gpio_set_direction(pin_stby, GPIO_MODE_OUTPUT); //

    gpio_reset_pin(pin_ain1);
    gpio_set_direction(pin_ain1, GPIO_MODE_OUTPUT);
    
    gpio_reset_pin(pin_ain2);
    gpio_set_direction(pin_ain2, GPIO_MODE_OUTPUT);
    
    gpio_reset_pin(pin_bin1);
    gpio_set_direction(pin_bin1, GPIO_MODE_OUTPUT);
    
    gpio_reset_pin(pin_bin2);
    gpio_set_direction(pin_bin2, GPIO_MODE_OUTPUT);

    // Configuramos el reloj del motor pwm

    ledc_timer_config_t ledc_timer = {};
    ledc_timer.speed_mode       = LEDC_LOW_SPEED_MODE; // Modo estándar del ESP32
    ledc_timer.timer_num        = LEDC_TIMER_0;        // Usaremos el Reloj número 0
    ledc_timer.duty_resolution  = LEDC_TIMER_8_BIT;    // Resolución de 8 bits (Valores de 0 a 255)
    ledc_timer.freq_hz          = 5000;                // 5 kHz (Frecuencia excelente para motores DC)
    ledc_timer.clk_cfg          = LEDC_AUTO_CLK;       // Reloj automático

    ledc_timer_config(&ledc_timer);

    // Configuramos el Canal A (Velocidad Llanta Izquierda)
    ledc_channel_config_t ledc_channel_a = {};
    ledc_channel_a.speed_mode     = LEDC_LOW_SPEED_MODE;
    ledc_channel_a.channel        = LEDC_CHANNEL_0; // Canal 0
    ledc_channel_a.timer_sel      = LEDC_TIMER_0;   // Lo conectamos a nuestro reloj 0
    ledc_channel_a.intr_type      = LEDC_INTR_DISABLE;
    ledc_channel_a.gpio_num       = pin_pwma;       // ¡Lo conectamos a tu pin físico PWMA!
    ledc_channel_a.duty           = 0;              // Arranca con velocidad cero
    ledc_channel_a.hpoint         = 0;
    
    ledc_channel_config(&ledc_channel_a); // Aplicamos configuración

    // Configuramos el Canal B (Velocidad Llanta Derecha)
    ledc_channel_config_t ledc_channel_b = {};
    ledc_channel_b.speed_mode     = LEDC_LOW_SPEED_MODE;
    ledc_channel_b.channel        = LEDC_CHANNEL_1; // Canal 1 (Para que no choque con el A)
    ledc_channel_b.timer_sel      = LEDC_TIMER_0;   // Usa el mismo reloj (misma frecuencia)
    ledc_channel_b.intr_type      = LEDC_INTR_DISABLE;
    ledc_channel_b.gpio_num       = pin_pwmb;       // ¡Lo conectamos a tu pin físico PWMB!
    ledc_channel_b.duty           = 0;
    ledc_channel_b.hpoint         = 0;
    
    ledc_channel_config(&ledc_channel_b); // Aplicamos configuración

    // Encendemos el chip TB6612FNG

    gpio_set_level(pin_stby, 1);
}

void Motores::avanzar(uint8_t velocidad){
    // Recordemos que 1 y 0 es avanzar

    // Izquierda
    gpio_set_level(pin_ain1, 1);
    gpio_set_level(pin_ain2, 0);
    // Derecha
    gpio_set_level(pin_bin1, 1);
    gpio_set_level(pin_bin2, 0);

    // Acelerador
    ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0, velocidad); // Prepara el set (datos con el acelerador de 0 a 255)
    ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0); // Ejecutas para que el chip mande la señal electrica al pin
    
    ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_1, velocidad);
    ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_1);

}

void Motores::girarIzquierda(uint8_t velocidad){
    gpio_set_level(pin_ain1, 0);
    gpio_set_level(pin_ain2, 1);

    gpio_set_level(pin_bin1, 1);
    gpio_set_level(pin_bin2, 0);

    ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0, velocidad);
    ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0);
    
    ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_1, velocidad);
    ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_1);
}

void Motores::girarDerecha(uint8_t velocidad){
    gpio_set_level(pin_ain1, 1);
    gpio_set_level(pin_ain2, 0);

    gpio_set_level(pin_bin1, 0);
    gpio_set_level(pin_bin2, 1);

    ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0, velocidad);
    ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0);
    
    ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_1, velocidad);
    ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_1);
}

void Motores::retroceder(uint8_t velocidad){
    // Recordemos que 0 y 1 es reversa

    // Izquierda
    gpio_set_level(pin_ain1, 0);
    gpio_set_level(pin_ain2, 1);
    // Derecha
    gpio_set_level(pin_bin1, 0);
    gpio_set_level(pin_bin2, 1);

    // Acelerador
    ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0, velocidad);
    ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0);
    
    ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_1, velocidad);
    ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_1);
}

void Motores::frenar(){
    //Recordemos que 1 y 1 es:
    // Freno de motor (Cortocircuito interno para frenar en seco)

    gpio_set_level(pin_ain1, 1);
    gpio_set_level(pin_ain2, 1);
    
    gpio_set_level(pin_bin1, 1);
    gpio_set_level(pin_bin2, 1);

    // Acelerador a cero
    ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0, 0);
    ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0);
    
    ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_1, 0);
    ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_1);
}

void Motores::puntoMuerto() {
    // Corta la energía, las llantas giran libremente por inercia
    gpio_set_level(pin_ain1, 0);
    gpio_set_level(pin_ain2, 0);
    
    gpio_set_level(pin_bin1, 0);
    gpio_set_level(pin_bin2, 0);

    // Opcionalmente bajamos el acelerador a cero
    ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0, 0);
    ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0);
    
    ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_1, 0);
    ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_1);
}

void Motores::universal(int16_t x, int16_t y){
	velocidad(x, y);
	gpio_set_level(pin_ain1, direccion1a);
	gpio_set_level(pin_ain2, direccion2a);
	gpio_set_level(pin_bin1, direccion1b);
	gpio_set_level(pin_bin2, direccion2b);

    // IMPRIMIR LO QUE VA AL PWM PARA VERLO EN PANTALLA
    printf("Intentando enviar PWM -> Motor Izq: %d\n | Motor Der: %d\n", abs(velocidad1), abs(velocidad2));

	ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0, abs(velocidad1));
	ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0);
	ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_1, abs(velocidad2));
	ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_1);
}

void Motores::velocidad(int16_t x_raw, int16_t y_raw){
    long x_center = 0;
    long y_center = 0;

    // 1. ZONA MUERTA (Usando tus datos de centro 1800-1900)
    // Si está entre 1750 y 1950, asumimos CERO perfecto.
    if (x_raw > 1950) x_center = x_raw - 1950;
    else if (x_raw < 1750) x_center = x_raw - 1750;

    if (y_raw > 1950) y_center = y_raw - 1950;
    else if (y_raw < 1750) y_center = y_raw - 1750;

    // 2. MEZCLA ARCADE ESTÁNDAR
    long vel_izq_cruda = y_center + x_center;
    long vel_der_cruda = y_center - x_center;

    // 3. ESCALADO A PWM 11 bits (Capacidad max: 0 a 2047)
    // El rango restante max del joystick es ~2145 (4095 - 1950).
    // Usamos int16_t/long velocidad1 para cálculos intermedios.
    // Multiplicamos por la resolución del timer (2047) y dividimos entre el max joystick (2145).
    long vel_izq_final = (vel_izq_cruda * 2047) / 2145;
    long vel_der_final = (vel_der_cruda * 2047) / 2145;

    // 4. TOPES DE SEGURIDAD (No desbordar el timer)
    if(vel_izq_final > 2047) vel_izq_final = 2047;
    if(vel_izq_final < -2047) vel_izq_final = -2047;
    if(vel_der_final > 2047) vel_der_final = 2047;
    if(vel_der_final < -2047) vel_der_final = -2047;

    // Guardamos los valores absolutos para el PWM de 11 bits.
    // ¡Asegúrate de que velocidad1 y velocidad2 en Motores.h sean int16_t o long!
    velocidad1 = vel_izq_final; 
    velocidad2 = vel_der_final;

    // 5. ASIGNAR DIRECCIONES - Motor Izquierdo (Canal A)
    if(velocidad1 == 0){
        direccion1a = 0; direccion2a = 0; // Freno/Libre
    }else if(velocidad1 > 0){
        direccion1a = 1; direccion2a = 0; // Adelante
    }else{
        direccion1a = 0; direccion2a = 1; // Atras
    }

    // 6. ASIGNAR DIRECCIONES - Motor Derecho (Canal B)
    if(velocidad2 == 0){
        direccion1b = 0; direccion2b = 0; // Freno/Libre
    }else if(velocidad2 > 0){
        direccion1b = 1; direccion2b = 0; // Adelante
    }else{
        direccion1b = 0; direccion2b = 1; // Atras
    }
}