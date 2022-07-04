/*
 * domoBoard.cpp
 *
 *  Created on: 09/03/2015
 *      Author: jctejero
 *
 *  Modified on: 16/03/2022
 */

/****************************************************************************/
/***        Include files                                                 ***/
/****************************************************************************/
#include "domoBoard.h"

/****************************************************************************/
/***                	Constantes memoria de programa                    ***/
/****************************************************************************/
const char BTN1_NAME[] 	PROGMEM = "BOTÓN 1";
const char BTN2_NAME[] 	PROGMEM = "BOTÓN 2";
const char BTNOP_NAME[] PROGMEM = "Pulsador Optocoplado";
const char PIR_NAME[]	PROGMEM = "PIR (Sensor de Movimiento)";
const char POT1_NAME[]	PROGMEM = "POTENCIOMETRO 1";
const char POT2_NAME[]	PROGMEM = "POTENCIOMETRO 2";
const char PRES_NAME[]	PROGMEM = "PHOTORESISTOR";
const char PTTOR_NAME[]	PROGMEM = "PHOTO TRANSISTOR (Op800WSL)";
const char STEMP_NAME[]	PROGMEM = "SENSOR TEMPERATURA (TMP36)";
const char GAS_NAME[]	PROGMEM = "DETECTOR DE GAS MQ-2";


// Constructors ////////////////////////////////////////////////////////////////

DomoBoard::DomoBoard()
{
	//Definimos pin's DomoBoard.
	pinMode(BUTTON1_P, INPUT);
	pinMode(BUTTON2_P, INPUT);
	pinMode(BTN_OPT_P, INPUT);      	//Pin Entrada Optocoplada
	pinMode(PIR_P, INPUT);				//Pin PIR como entrada

	pinMode(RELE_P, OUTPUT);
	pinMode(TRIAC_P, OUTPUT);

	BOTON1.pin 			= BUTTON1_P;
	BOTON1.valor 		= LOW;
	BOTON1.valor_Df 	= LOW;
	BOTON1.Activo 		= true;
	BOTON1.eSensor  	= S_DIGITAL;
	BOTON1.name			= BTN1_NAME;
	BOTON1.SensorEvent  = NULL;

	BOTON2.pin 			= BUTTON2_P;
	BOTON2.valor 		= LOW;
	BOTON2.valor_Df 	= LOW;
	BOTON2.Activo 		= true;
	BOTON2.eSensor  	= S_DIGITAL;
	BOTON2.name			= BTN2_NAME;
	BOTON2.SensorEvent	= NULL;

	BTN_OPT.pin 		= BTN_OPT_P;
	BTN_OPT.valor 		= HIGH;
	BTN_OPT.valor_Df	= HIGH;
	BTN_OPT.Activo 		= true;
	BTN_OPT.eSensor 	= S_DIGITAL;
	BTN_OPT.name		= BTNOP_NAME;
	BTN_OPT.SensorEvent = NULL;

	//Inicialización del sensor de movimiento
	PIR_MOV.pin			= PIR_P;
	PIR_MOV.valor		= LOW;
	PIR_MOV.valor_Df	= LOW;  //HIGH; para el sensor original
	PIR_MOV.Activo 		= true;
	PIR_MOV.eSensor 	= S_DIGITAL;
	PIR_MOV.name		= PIR_NAME;

	//==================================
	//Inicialización Sensores Analógicos
	//==================================
	POT1.pin			= POT1_P;
	POT1.Activo 		= true;
	POT1.eSensor 		= S_ANALOGICO;
	POT1.name			= POT1_NAME;

	POT2.pin			= POT2_P;
	POT2.Activo 		= true;
	POT2.eSensor 		= S_ANALOGICO;
	POT2.name			= POT2_NAME;

	PHOTORES.pin		= PHOTORES_P;
	PHOTORES.Activo 	= true;
	PHOTORES.eSensor 	= S_ANALOGICO;
	PHOTORES.name		= PRES_NAME;

	TEMPSEN.pin 		= TEMPSEN_P;
	TEMPSEN.Aux			= -1;
	TEMPSEN.Activo 		= true;
	TEMPSEN.eSensor 	= S_ANALOGICO;
	TEMPSEN.name		= STEMP_NAME;

	PHOTOTTOR.pin		= PHOTOTTOR_P;
	PHOTOTTOR.Activo 	= true;
	PHOTOTTOR.eSensor 	= S_ANALOGICO;
	PHOTOTTOR.name		= PTTOR_NAME;

	GASMQ2.pin			= GASMQ2_P;
	GASMQ2.Activo 		= false;
	GASMQ2.eSensor 		= S_ANALOGICO;
	GASMQ2.name			= GAS_NAME;

	//==========================
	//Inicialización Actuadores
	//==========================
	RELE.pin 			= RELE_P;
	RELE.estado 		= LOW;

	TRIAC.pin 			= TRIAC_P;
	TRIAC.estado 		= LOW;

	PER_ONOFF.pin		= PER_ONOFF_P;
	PER_ONOFF.estado	= LOW;
	pinMode(PER_ONOFF_P, OUTPUT);

	PER_UPDOWN.pin		= PER_UPDOWN_P;
	PER_UPDOWN.estado	= LOW;
	pinMode(PER_UPDOWN_P, OUTPUT);
}

void DomoBoard::enableSensor(ptsSensor Sensor, bool enabled){
	Sensor->Activo = enabled;

	if(!enabled)
		Sensor->valor = Sensor->valor_Df;
}

void DomoBoard::leerAllSensor(void){
	leerSensor(&BOTON1);
	leerSensor(&BOTON2);
	leerSensor(&BTN_OPT);
	leerSensor(&PIR_MOV);
}


void  DomoBoard::leerSensor(ptsSensor Sensor){
	int valor = 0;

	if(Sensor->Activo){
		switch (Sensor->eSensor)
		{
		case S_DIGITAL:
			valor = digitalRead(Sensor->pin);
			break;

		case S_ANALOGICO:
			valor = analogRead(Sensor->pin);
			break;
		}

		if(Sensor->valor != valor)
		{
			Sensor->valor = valor;

			//Si hay un cambio en el estado del sensor llamamos a la aplicación asociada
			if(Sensor->SensorEvent != NULL)
				Sensor->SensorEvent(Sensor);
		}
	}
}

void  DomoBoard::setActuator(tsActuator *Actuator, bool val){
	digitalWrite(Actuator->pin, val);
	Actuator->estado = val;
}

void DomoBoard::manageSensorActuators(TManagerActuators *managerActuators, bool val){
	if(managerActuators->count() > 0){
		for(uint8_t i = 0; i < managerActuators->count(); i++){
			DomoBoard::setActuator(managerActuators->peek(i), val);
		}
	}
}

void 	DomoBoard::SetPersiana(tsStaPer staPer)
{
	switch(staPer){
	case PER_STOP:
	case PER_STOP2:
		setActuator(&PER_ONOFF, false);
		setActuator(&PER_UPDOWN, false);
		break;

	case PER_DOWN:
		setActuator(&PER_UPDOWN, false);
		setActuator(&PER_ONOFF, true);
		break;

	case PER_UP:
		setActuator(&PER_UPDOWN, true);
		setActuator(&PER_ONOFF, true);
		break;
	}
}

void DomoBoard::clear_Actuators(){
	BOTON1.SensorEvent = NULL;
	BOTON1.managerActuators.clear();
	BOTON2.SensorEvent = NULL;
	BOTON2.managerActuators.clear();
	BTN_OPT.SensorEvent = NULL;
	BTN_OPT.managerActuators.clear();
	PIR_MOV.SensorEvent = NULL;
	PIR_MOV.managerActuators.clear();
	POT1.SensorEvent = NULL;
	POT1.managerActuators.clear();
	POT2.SensorEvent = NULL;
	POT2.managerActuators.clear();

	PHOTORES.SensorEvent = NULL;
	PHOTORES.managerActuators.clear();
	TEMPSEN.SensorEvent = NULL;
	TEMPSEN.managerActuators.clear();
	PHOTOTTOR.SensorEvent = NULL;
	PHOTOTTOR.managerActuators.clear();
	GASMQ2.SensorEvent = NULL;
	GASMQ2.managerActuators.clear();
}


DomoBoard domoboard;

