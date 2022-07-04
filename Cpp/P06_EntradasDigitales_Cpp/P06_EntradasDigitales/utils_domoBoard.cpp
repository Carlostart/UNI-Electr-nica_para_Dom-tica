/*
 * utils_domoBoard.cpp
 *
 *  Created on: 10/03/2015
 *      Author: jctejero
 *
 *  Modified on: 16/03/2022
 */

/****************************************************************************/
/***        Include files                                                 ***/
/****************************************************************************/
#include "utils_domoBoard.h"
#include "HardwareSerial.h"

void Interruptor(void *Sensor)
{
	if(((ptsSensor)Sensor)->valor_Df == ((ptsSensor)Sensor)->valor){
		Serial.print(((ptsSensor)Sensor)->name);
		Serial.print(" : Interruptor --> ");
		if(((ptsSensor)Sensor)->Aux == OFF){
			((ptsSensor)Sensor)->Aux = ON;
			Serial.println("ON");
		}else{
			((ptsSensor)Sensor)->Aux = OFF;
			Serial.println("OFF");
		}
	}

	DomoBoard::manageSensorActuators(&((ptsSensor)Sensor)->managerActuators, ((ptsSensor)Sensor)->Aux);
}

void Pulsado_Soltado(void *Sensor){
	ptsSensor sensor = reinterpret_cast<ptsSensor>(Sensor);

	Serial.print(sensor->name);
	if(sensor->valor_Df == sensor->valor){
		Serial.println(" --> Soltado");
	}else{
		Serial.println(" --> Pulsado");
	}

	DomoBoard::manageSensorActuators(&(sensor->managerActuators), !(sensor->valor_Df == sensor->valor));
}

void conmutador(void *Sensor)
{
	static int valor = OFF;

	ptsSensor sensor = reinterpret_cast<ptsSensor>(Sensor);

	if(sensor->valor_Df == sensor->valor){
		Serial.print("Conmutador --> ");
		if(valor == OFF){
			valor = ON;
			Serial.println("ON");
		}else{
			valor = OFF;
			Serial.println("OFF");
		}

		//Actualiza Actuadores
		//DomoBoard::setActuator(&domoboard.RELE, valor);

		DomoBoard::manageSensorActuators(&(sensor->managerActuators), valor);
	}
}

void mbInterruptor(void *mbSensor)
{
	TpmbSensor sensor = reinterpret_cast<TpmbSensor>(mbSensor);

	if(sensor->Sensor->valor_Df == sensor->Sensor->valor){
		mbDomoboard.manager_mbActuators(&(sensor->mbActuators), TOGGLE);
	}
}





