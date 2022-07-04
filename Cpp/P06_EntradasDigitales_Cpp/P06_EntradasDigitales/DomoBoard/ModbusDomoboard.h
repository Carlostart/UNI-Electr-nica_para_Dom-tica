/*
 * ModbusDomoboard.h
 *
 *  Created on: 11 mar. 2020
 *      Author: jctej
 *
 *  Modified: 03/04/2021
 *  Modified on: 16/03/2022
 */

#ifndef DOMOBOARD_MODBUSDOMOBOARD_H_
#define DOMOBOARD_MODBUSDOMOBOARD_H_

/**********************************************************************/
/***                         Include Files                          ***/
/**********************************************************************/
#include <domoBoard.h>
#include "ModBusSlave.h"

/**********************************************************************/
/***                         enums Definitions                      ***/
/**********************************************************************/
/* slave registers */

//Discrete Output Coils
enum {
	MB_RELE,
	MB_TRIAC,
	MB_O_COILS
};

//Discrete Input Contacts
enum {
	MB_BOTON1,
	MB_BOTON2,
	MB_BTNOPT,
	MB_I_CONTATCS
};

/****************************************************************************/
/***        Type Definitions                                              ***/
/****************************************************************************/
//State Actuator
typedef enum{
	OFF,
	ON,
	TOGGLE
}TStateDigitalDev;

typedef struct
{
	tpsActuator		actuator;
	uint16_t 		*mbRegs;			//Registro asociado para comunicaciones ModBus
}TmbActuator, *TpmbActuator;

typedef 	QueueList<TpmbActuator>		TmbActuators;

typedef struct{
	ptsSensor			Sensor;			//Sensor asociado
	int					Aux;			//Variable auxiliar, a la espera de definir el banco de registros digitales de entrada
	uint16_t 			*mbRegs;		//Registro asociado para comunicaciones ModBus
	TmbActuators		mbActuators;	//Listado de actuadores manejados por el sensor
	TNotifyEvent		mbSensorEvent;	//Evento para aplicación asociada
}TmbSensor, *TpmbSensor;


/*************************************************************/
/***                         CLASS                         ***/
/*************************************************************/
class ModbusDomoboard: public DomoBoard {
private:
	ModbusSlave	*mbs = NULL;

public:

	TmbSensor	BOTON1;
	TmbSensor	BOTON2;
	TmbSensor	BTN_OPT;

	TmbActuator RELE;
	TmbActuator TRIAC;

	ModbusDomoboard();
	virtual ~ModbusDomoboard(){};

	void 	leerAllSensor(void);
	void 	leerSensor(TpmbSensor Sensor);

	void 	setmbActuator(TmbActuator *Actuator, TStateDigitalDev val);
	void 	manager_mbActuators(TmbActuators *Actuators, TStateDigitalDev val);
	void 	setModBusSlave(ModbusSlave *mbSlave);
	void    clear_Actuators();
};

extern ModbusDomoboard mbDomoboard;
extern uint16_t	Cregs[MB_O_COILS];
extern uint16_t	Dregs[MB_I_CONTATCS];	//Registros para "Dicrete Input Contacts"

#endif /* DOMOBOARD_MODBUSDOMOBOARD_H_ */
