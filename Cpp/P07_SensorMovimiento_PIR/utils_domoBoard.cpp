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
#include "HardwareSerial.h"
#include "config_practicas.h"
#include "utils_domoBoard.h"

/******************************************************************/
/***                      Variables Locales                     ***/
/******************************************************************/
//Lista de acciones temporizadas
QueueList<TpmbSensor>		AccionesTemporizadas;


/*============================================*/
/*		 		  INTERRUPTOR                 */
/*============================================*/

void Interruptor(void *Sensor)
{
	if(((ptsSensor)Sensor)->valor_Df == ((ptsSensor)Sensor)->valor){
		DEBUGLOG("%s : Interruptor --> ", ((ptsSensor)Sensor)->name);
		if(((ptsSensor)Sensor)->Aux == OFF){
			((ptsSensor)Sensor)->Aux = ON;
			DEBUGLNF("ON");
		}else{
			((ptsSensor)Sensor)->Aux = OFF;
			DEBUGLNF("OFF");
		}
	}

	DomoBoard::manageSensorActuators(&((ptsSensor)Sensor)->managerActuators, ((ptsSensor)Sensor)->Aux);
}

/*============================================*/
/*		 		    PULSADOR                  */
/*============================================*/

void Pulsado_Soltado(void *Sensor){
	ptsSensor sensor = reinterpret_cast<ptsSensor>(Sensor);

	DEBUG(sensor->name);
	if(sensor->valor_Df == sensor->valor){
		DEBUGLNF(" --> Soltado");
	}else{
		DEBUGLNF(" --> Pulsado");
	}

	DomoBoard::manageSensorActuators(&(sensor->managerActuators), !(sensor->valor_Df == sensor->valor));
}


/*============================================*/
/*		     	   CONMUTADOR                 */
/*============================================*/

void conmutador(void *Sensor)
{
	static int valor = OFF;

	ptsSensor sensor = reinterpret_cast<ptsSensor>(Sensor);

	if(sensor->valor_Df == sensor->valor){
		DEBUGF("Conmutador --> ");

		if(valor == OFF){
			valor = ON;
			DEBUGLNF("ON");
		}else{
			valor = OFF;
			DEBUGLNF("OFF");
		}

		//Actualiza Actuadores
		switch(Aregs[MB_SELPRACT]){
		case P3_CONMUTADOR:
			DomoBoard::setActuator(&domoboard.RELE, valor);
			break;

		case P4_CONMUTADOR:
			DomoBoard::manageSensorActuators(&(sensor->managerActuators), valor);
			break;
		};
	}
}

/*============================================*/
/*		      INTERRUPTOR MODBUS              */
/*============================================*/

void mbInterruptor(void *mbSensor)
{
	TpmbSensor sensor = reinterpret_cast<TpmbSensor>(mbSensor);

	if(sensor->Sensor->valor_Df == sensor->Sensor->valor){
		mbDomoboard.manager_mbActuators(&(sensor->mbActuators), TOGGLE);
	}
}

/*============================================*/
/*		 	   CONMUTADOR MODBUS              */
/*============================================*/
void mbConmutador(void *mbSensor)
{
	TpmbSensor sensor = reinterpret_cast<TpmbSensor>(mbSensor);

	if(sensor->Sensor->valor_Df == sensor->Sensor->valor){
		//Estado del sensor ha cambiado
		mbDomoboard.manager_mbActuators(&(sensor->mbActuators), TOGGLE);
	}
}

/*========================================================*/
/*		 		  INTERRUPTOR TEMPORIZADO                 */
/*========================================================*/

/*
 * Este sensor será activado mediante Sensor (Variable de control) y aunque se vuelva inactivo,
 * el interruptor, permanecerá activo durante el tiempo que indique el temporizador.
 * Si durante el tiempo que el interruptor está activo, la variable de control permanece inactiva,
 * o se activa de nuevo, el tiempo que el interruptor permanece activo se
 * irá actualizando, de tal forma que el tiempo que el interruptor permanece ativo,
 * siempre se cuenta desde la última vez que el sensor se activo.
 *
 */

void	Interruptor_Temporizado(void *Sensor){

	TpmbSensor sensor = reinterpret_cast<TpmbSensor>(Sensor);

	if(sensor->Sensor->valor_Df != sensor->Sensor->valor){

		sensor->ctrlTempPtr->time_lastAct = millis();				//Actualiza el instante de activación

		if(!sensor->ctrlTempPtr->estado){
			//Se actualiza el estado de activación del temporizador y los actuadores asciados
			//ON Actuators
			sensor->ctrlTempPtr->estado = true;
			mbDomoboard.manager_mbActuators(&(sensor->mbActuators), (TStateDigitalDev)ON);
		}

	}
	else{

		//Actualizamos Temporizador

		uint32_t tempo = (millis() - sensor->ctrlTempPtr->time_lastAct);
		uint16_t maxtempo = *(sensor->ctrlTempPtr->time_activo);
		uint16_t Cte_Norma = sensor->ctrlTempPtr->Cte_Norma;

		if(tempo >= (maxtempo)*(Cte_Norma)){
			if(sensor->ctrlTempPtr->estado){
				//OFF Actuators
				sensor->ctrlTempPtr->estado = false;
				mbDomoboard.manager_mbActuators(&(sensor->mbActuators), (TStateDigitalDev)OFF);
			}
		}
	}
}

/*====================================================================================
 * Llamada regular usada para gestionar los temporizadores usados por la aplicación. =
 * es llamada cada LOOP_TIME (10 ms)
 *====================================================================================*/
void AccionesTemporizadas_Timer(void){
	for(int n=0; n<AccionesTemporizadas.count(); n++){
		AccionesTemporizadas.peek(n)->mbSensorEvent(AccionesTemporizadas.peek(n));
	}
}





