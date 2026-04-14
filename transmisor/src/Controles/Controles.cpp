#include "Controles.h"
#include "Datos.h"
#include "driver/gpio.h"

//constructor
Controles::Controles(adc1_channel_t chX, adc1_channel_t chY) :
    x(2048),
	y(2048),
	pin_encender(GPIO_NUM_13), //PIN PROHIBIDOS: TX Y RX, D5, D4, D12, D15 (GPIO_NUM_3 Y GPIO_NUM_1 equivalen a RX0 y TX0, estos ahogan el monitor serial)
	pin_vel(GPIO_NUM_14),
	pin_continuar(GPIO_NUM_27),
	pin_modo(GPIO_NUM_26),
	pin_boton1(GPIO_NUM_25),
	pin_boton2(GPIO_NUM_33),
	encender(0),
	vel(0),
	continuar(false),
	modo(false),
	boton1(false),
	boton2(false),
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
	conf.pin_bit_mask = (1ULL << pin_encender) | (1ULL << pin_vel) | (1ULL << pin_continuar) | (1ULL << pin_modo) | (1ULL << pin_boton1) | (1ULL << pin_boton2);
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
	if(abs(tempX - 2048) < 250){
		//valor del centro
		x = 2048;
	} else {
		//valor leido
		x = tempX;
	}
	//se calibra para evitar errores en y
	if(abs(tempY - 2048) < 250){
		//valor del centro
		y = 2048;
	} else {
		//valor leido
		y = tempY;
	}
}

//actualizar el estado de los botones
void Controles::botones(){
	if((encender == 0) && (gpio_get_level(pin_encender) == 1)){
		encender = 1;
	}else if ((encender == 1) && (gpio_get_level(pin_encender) == 1)) {
		encender = 0;
	}
	if((vel == 0) && (gpio_get_level(pin_vel) == 1)){
		vel = 1;
	}else if((vel == 1) && (gpio_get_level(pin_vel) == 1)){
		vel = 0;
	}
	if(!continuar && (gpio_get_level(pin_continuar) == 1)){
		continuar = true;
	}else if(continuar && (gpio_get_level(pin_continuar) == 1)){
		continuar = false;
	}
	if(!modo && (gpio_get_level(pin_modo) == 1)){
	modo = true;
	}else if(modo && (gpio_get_level(pin_modo) == 1)){
		modo = false;
	}
	if(!boton1 && (gpio_get_level(pin_boton1) == 1)){
		boton1 = true;
	}else if(boton1 && (gpio_get_level(pin_boton1) == 1)){
		boton1 = false;
	}
	if(!boton2 && (gpio_get_level(pin_boton2) == 1)){
		boton2 = true;
	}else if(boton2 && (gpio_get_level(pin_boton2) == 1)){
		boton2 = false;
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
	datos->vel = vel;
	datos->continuar = continuar;
	datos->modo = modo;
	datos->boton1 = boton1;
	datos->boton2 = boton2;
}
