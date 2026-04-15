#include "Controles.h"
#include "Datos.h"
#include "driver/gpio.h"

//constructor
Controles::Controles(adc1_channel_t chX, adc1_channel_t chY) :
    x(2048),
	y(2048),
	pin_encender(GPIO_NUM_13), //PIN PROHIBIDOS: TX Y RX, D5, D4, D12, D15 (GPIO_NUM_3 Y GPIO_NUM_1 equivalen a RX0 y TX0, estos ahogan el monitor serial)
	pin_cambiarVel(GPIO_NUM_14),
 	pin_mantenerVel(GPIO_NUM_16),
	pin_clackson(GPIO_NUM_17),
	pin_ventiladores(GPIO_NUM_18),
	pin_boton0(GPIO_NUM_19),
	pin_botonJoystick(GPIO_NUM_21),
	encender(false),
	cambiarVel(false),
	mantenerVel(false),
	clackson(false),
	ventiladores(false),
	boton0(false),
	botonJoystick(false),
	channelX(chX),
	channelY(chY)
{}

void Controles::begin() {
	//inicializacion de los canales del joystick
    adc1_config_width(ADC_WIDTH_BIT_12);
    adc1_config_channel_atten(channelX, ADC_ATTEN_DB_11);
    adc1_config_channel_atten(channelY, ADC_ATTEN_DB_11);

	//inicializacion de los pines de los botones
	gpio_config_t conf;
	conf.pin_bit_mask = (1ULL << pin_encender) | (1ULL << pin_cambiarVel) | (1ULL << pin_mantenerVel) | (1ULL << pin_clackson) | (1ULL << pin_ventiladores) | (1ULL << pin_boton0) | (1ULL << pin_botonJoystick);
	conf.mode = GPIO_MODE_INPUT;
	conf.pull_up_en = GPIO_PULLUP_DISABLE;
	conf.pull_down_en = GPIO_PULLDOWN_ENABLE;
	conf.intr_type = GPIO_INTR_DISABLE;
	gpio_config(&conf);
}

//actualiza las posiciones del joystick
void Controles::pos() {
	//variables temporales
	int tempX, tempY;

	//se lee la posicion del joystick
    tempX = adc1_get_raw(channelX);
    tempY = adc1_get_raw(channelY);
	//se calibra para evitar errores en x
	if(abs(tempX - 2048) < 100){
		//valor del centro
		x = 2048;
	} else {
		//valor leido
		x = tempX;
	}
	//se calibra para evitar errores en y
	if(abs(tempY - 2048) < 100){
		//valor del centro
		y = 2048;
	} else {
		//valor leido
		y = tempY;
	}
}

//actualizar el estado de los botones
void Controles::botones(){
	if((encender == false) && (gpio_get_level(pin_encender) == 1)){
		encender = true;
	}else if ((encender == true) && (gpio_get_level(pin_encender) == 1)) {
		encender = false;
	}
	if((cambiarVel == false) && (gpio_get_level(pin_cambiarVel) == 1)){
		cambiarVel = true;
	}else if((cambiarVel == true) && (gpio_get_level(pin_cambiarVel) == 1)){
		cambiarVel = false;
	}
	if(!mantenerVel && (gpio_get_level(pin_mantenerVel) == 1)){
		mantenerVel = true;
	}else if(mantenerVel && (gpio_get_level(pin_mantenerVel) == 1)){
		mantenerVel = false;
	}
	if(!clackson && (gpio_get_level(pin_clackson) == 1)){
	clackson = true;
	}else if(clackson && (gpio_get_level(pin_clackson) == 1)){
		clackson = false;
	}
	if(!ventiladores && (gpio_get_level(pin_ventiladores) == 1)){
		ventiladores = true;
	}else if(ventiladores && (gpio_get_level(pin_ventiladores) == 1)){
		ventiladores = false;
	}
	if(!boton0 && (gpio_get_level(pin_boton0) == 1)){
		boton0 = true;
	}else if(boton0 && (gpio_get_level(pin_boton0) == 1)){
		boton0 = false;
	}
	if(!botonJoystick && (gpio_get_level(pin_botonJoystick) == 1)){
		botonJoystick = true;
	}else if(botonJoystick && (gpio_get_level(pin_botonJoystick) == 1)){
		botonJoystick = false;
	}
}

//se llena el struct con las lecturas de los controles
void Controles::empaquetar(Datos* datos) {
	//actualiza las posiciones del joystick
	pos();
	botones();
	//se llena el struct con las posiciones del joystick
	datos->x = x;
	datos->y = y;
	datos->encender = encender;
	datos->cambioVel = cambiarVel;
	datos->mantenerVel = mantenerVel;
	datos->clackson = clackson;
	datos->ventiladores = ventiladores;
	datos->boton0 = boton0;
	datos->botonJoystick = botonJoystick;
}
