// Do not remove the include below
#include "P06_EntradasDigitales.h"

#include 	"Gest_Modbus.h"
#include	"config_practicas.h"


#define 	LOOP_TMP		10	//miliseconds

//The setup function is called once at startup of the sketch
void setup()
{
	//Iniciamos Modbus en Modo RTU
	Init_RTU_Modbus();

	//Configura práctica 6 apartado 3
	config_practica6_apt_3();

	Serial.println("P06 Entradas Digitales ModBus - Solución");

}

// The loop function is called in an endless loop
void loop()
{
	uint32_t tmp_loop = 0;

	if((millis()-tmp_loop) > LOOP_TMP){
		tmp_loop = millis();

		mbDomoboard.leerAllSensor();

		RTU_ModBus();
	}
}
