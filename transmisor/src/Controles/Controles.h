#ifndef CONTROLES_H
#define CONTROLES_H

#include "driver/adc.h"
#include "Datos.h"

class Controles {
public:
	//constructor
    Controles(adc1_channel_t chX, adc1_channel_t chY);

	//inicializacion
    void begin();

	//se llena el struct con las lecturas de los controles
	void empaquetar(Datos* datos);

private:
	//posision del joystick
    int x;
    int y;
	
	//pines de los botones
	gpio_num_t pin_encender;
	gpio_num_t pin_cambiarVel;
	gpio_num_t pin_mantenerVel;
	gpio_num_t pin_clackson;
	gpio_num_t pin_ventiladores;
	gpio_num_t pin_boton0;
	gpio_num_t pin_botonJoystick;

	//estado de los botones
	bool encender;
	bool cambiarVel;
	bool mantenerVel;
	bool clackson;
	bool ventiladores;
	bool boton0;
	bool botonJoystick;


	//metodo para actualizar las posiciones
    void pos();
	void botones();

	//canales del joystick
    adc1_channel_t channelX;
    adc1_channel_t channelY;
};

#endif
