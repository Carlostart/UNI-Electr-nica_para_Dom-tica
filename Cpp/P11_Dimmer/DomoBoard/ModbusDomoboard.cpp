/*
 * ModbusDomoboard.cpp
 *
 *  Created on: 11 mar. 2020
 *      Author: jctejero
 *
 *  Modified: 03/04/2021
 *  Modified on: 16/03/2022
 */

#include <ModbusDomoboard.h>
#include "utils_domoBoard.h"
#include "debuglog.h"

/****************************************************************************/
/***        Variables Locales                                             ***/
/****************************************************************************/

/***************************************************
 * Definición Bancos de registros usados en ModBus *
 ***************************************************/
uint16_t	Cregs[MB_O_COILS];		//Registros para "Dicrete Output Coils"
uint16_t	Dregs[MB_I_CONTATCS];	//Registros para "Dicrete Input Contacts"
uint16_t	Aregs[MB_A_REGS];		//Registros para "Analog Output Holding Registers"
uint16_t	Iregs[MB_I_REGS];		//Registros para "Analog Input Registers"

ModbusDomoboard mbDomoboard;
QueueList<TpmbActuator>	lDimmers;

ModbusDomoboard::ModbusDomoboard():DomoBoard() {
	//*****  Initialize ModBus Sensors  ****

	//Initialize BOTON1 for ModBus
	BOTON1.Sensor 			= &(DomoBoard::BOTON1);
	BOTON1.mbRegs 			= &Dregs[MB_BOTON1];
	BOTON1.Aux 				= LOW;

	//Initialize BOTON1 for ModBus
	BOTON2.Sensor 			= &(DomoBoard::BOTON2);
	BOTON2.mbRegs 			= &Dregs[MB_BOTON2];
	BOTON2.Aux 				= LOW;

	//Initialize BOTON1 for ModBus
	BTN_OPT.Sensor 			= &(DomoBoard::BTN_OPT);
	BTN_OPT.mbRegs 			= &Dregs[MB_BTNOPT];
	Dregs[MB_BTNOPT] 		= DomoBoard::BTN_OPT.valor;

	//Inicializamos el registros ModBus Sensor PIR
	PIR_MOV.Sensor			= &(DomoBoard::PIR_MOV);
	PIR_MOV.mbRegs			= &Dregs[MB_PIRMOV];
	Dregs[MB_PIRMOV] 		= DomoBoard::PIR_MOV.valor;
	Cregs[MB_ACTPIR]		= 0x01;  						//Inicialmente, el PIR es considerado activo

	//Initialize ModBus Analog sensors
	POT1.Sensor				=   &(DomoBoard::POT1);
	POT1.mbRegs				=	&Iregs[MB_POT1];;

	POT2.Sensor				=   &(DomoBoard::POT2);
	POT2.mbRegs				=	&Iregs[MB_POT2];

	PHOTORES.Sensor			=   &(DomoBoard::PHOTORES);
	PHOTORES.mbRegs			=	&Iregs[MB_PHOTORES];

	TEMPSEN.Sensor			=   &(DomoBoard::TEMPSEN);
	TEMPSEN.mbRegs			=	&Iregs[MB_TEMPSEN];
	TEMPSEN.mbSensorEvent	=   Calc_Temperatura;

	PHOTOTTOR.Sensor		=   &(DomoBoard::PHOTOTTOR);
	PHOTOTTOR.mbRegs		=	&Iregs[MB_PHOTOTTOR];

	GASMQ2.Sensor			=   &(DomoBoard::GASMQ2);
	GASMQ2.mbRegs			=	&Iregs[MB_GASMQ2];

	RELE.actuator 			= 	&(DomoBoard::RELE);
	RELE.mbRegs	 			= 	&Cregs[MB_RELE];

	TRIAC.actuator 			= 	&(DomoBoard::TRIAC);
	TRIAC.mbRegs 			= 	&Cregs[MB_TRIAC];

	PERUP.mbRegs			=	&Cregs[MB_PERUP];
	Cregs[MB_PERUP] 		=	OFF;

	PERDOWN.mbRegs			=	&Cregs[MB_PERDOWN];
	Cregs[MB_PERDOWN] 		=	OFF;

	DIMMER1.actuator		=	(tpsActuator)(&(DomoBoard::DIMMER1));
	DIMMER1.mbRegs			=	&Aregs[MB_DIMMER1];

	DIMMER2.actuator		= 	(tpsActuator)(&(DomoBoard::DIMMER2));
	DIMMER2.mbRegs			=	&Aregs[MB_DIMMER2];
/*
	lDimmers.clear();
	lDimmers.push(&DIMMER1);
	lDimmers.push(&DIMMER1);
*/
}

void ModbusDomoboard::leerAllSensor(void){
	leerSensor(&BOTON1);
	leerSensor(&BOTON2);
	leerSensor(&BTN_OPT);
	leerSensor(&PIR_MOV);
	leerSensor(&POT1);
	leerSensor(&POT2);
	leerSensor(&PHOTORES);
	leerSensor(&TEMPSEN);
	leerSensor(&PHOTOTTOR);
	leerSensor(&GASMQ2);
}

void ModbusDomoboard::set_holdingRegister(uint16_t addReg, uint16_t value){
	Aregs[addReg] = value;

	if(mbs.writeholdingregister != NULL)
		mbs.writeholdingregister(addReg);
}

void ModbusDomoboard::set_coilRegister(uint16_t addReg, uint16_t value){
	Cregs[addReg] = value;
	if(mbs.writecoil != NULL)
		mbs.writecoil(addReg);
}

void ModbusDomoboard::leerSensor(TpmbSensor Sensor){

	DomoBoard::leerSensor(Sensor->Sensor);

	//compueba si el valor leído por el sensor difiere del valor almacenado en el registro correspondiente
	//del banco de registros
	if((int16_t)(*(Sensor->mbRegs)) != Sensor->Sensor->valor){
		//Estado Sensor ha cambiado
		//Se actualiza el registro correspondiente con el nuevo valor leído en el sensor.
		*(Sensor->mbRegs) = Sensor->Sensor->valor;

		//Se inícia el evento asociado a la actualización del banco de registro correpondiente
		if(Sensor->mbSensorEvent != NULL){
			Sensor->mbSensorEvent(Sensor);
		}
	}
}

void	ModbusDomoboard::clear_Actuators(){
	DomoBoard::clear_Actuators();

	BOTON1.mbSensorEvent = NULL;
	BOTON1.mbActuators.clear();
	BOTON2.mbSensorEvent = NULL;
	BOTON2.mbActuators.clear();
	BTN_OPT.mbSensorEvent = NULL;
	BTN_OPT.mbActuators.clear();
	PIR_MOV.mbSensorEvent = NULL;
	PIR_MOV.mbActuators.clear();
	POT1.mbSensorEvent = NULL;
	POT1.mbActuators.clear();
	POT2.mbSensorEvent = NULL;
	POT2.mbActuators.clear();

	PHOTORES.mbSensorEvent = NULL;
	PHOTORES.mbActuators.clear();
	PHOTOTTOR.mbSensorEvent = NULL;
	PHOTOTTOR.mbActuators.clear();
	GASMQ2.mbSensorEvent = NULL;
	GASMQ2.mbActuators.clear();
}


void  ModbusDomoboard::setmbActuator(TmbActuator *Actuator, TStateDigitalDev val){
	bool newVal = (bool)val;

	if(val == TOGGLE){

		newVal = !(*Actuator->mbRegs);
	}

	if(*(Actuator->mbRegs) != newVal){
		*(Actuator->mbRegs) = newVal;

		setActuator(Actuator->actuator, *(Actuator->mbRegs));

	}
}

void ModbusDomoboard::manager_mbActuators(TmbActuators *Actuators, TStateDigitalDev val){

	for(int n = 0; n < Actuators->count(); n++){
		setmbActuator(Actuators->peek(n), val);
	}

}
