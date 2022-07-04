/*
 * domoBoard.h
 *
 *  Created on: 09/03/2015
 *      Author: jctejero
 *
 *  Modified on: 16/03/2022
 */

#ifndef DOMOBOARD_H_
#define DOMOBOARD_H_

/****************************************************************************/
/***        Include files                                                 ***/
/****************************************************************************/
#include	"Arduino.h"
#include	"QueueList.h"

/****************************************************************************/
/***        Type Definitions                                              ***/
/****************************************************************************/
typedef void (*TNotifyEvent)(void *);

typedef enum
{
	S_DIGITAL,
	S_ANALOGICO
}teSensor;

typedef struct
{
	byte	pin;
	bool	estado;
}tsActuator, *tpsActuator;

typedef 	QueueList<tpsActuator>		TManagerActuators;

typedef struct
{
	byte					pin;			//Pin asignado al sensor
	int	    				valor;			//Valor leido
	int						valor_Df;		//Valor Sensor por defecto
	bool					Activo;			//Activar/Desactivas sensor
	teSensor				eSensor;		//Tipos de sensor ANALÓGICO/DIGITAL
	int						Aux;			//Variables para ser usadas en la gestión del sensor
	String					name;
	TNotifyEvent			SensorEvent;
	TManagerActuators		managerActuators;
}tsSensor, *ptsSensor;

/*
typedef void (*TSensorEvent)(tsSensor *);

typedef struct
{
	tsSensor			sSensor;
	TSensorEvent		SensorEvent;	//Evento para aplicación asociada
}ptsSensor;
*/

/****************************************************************************/
/***        Macro Definitions                                             ***/
/****************************************************************************/
#define 	BUTTON1_P  				3   	   	// Pulsador 1
#define 	BUTTON2_P  				10        	// Pulsador 2
#define 	RELE_P					4			// RELE 1
#define		TRIAC_P					7			// ON/OFF Solid State Triac
#define 	BTN_OPT_P		    	11 	    	// Entrada Optocoplada
#define		EN_485					12			// Bit to enable RS485 Conmunications
#define     PIR_P					13			// Sensor de Movimiento

#define		POT1_P					A3			// Potenciómetro 1
#define		POT2_P					A4			// Potenciómetro 2

/****************************************************************************/
/***        Exported Class                                                ***/
/****************************************************************************/
class DomoBoard
{
private:


public:

	DomoBoard(); 						//Constructor
	virtual ~DomoBoard(){};

	tsSensor		BOTON1;
	tsSensor		BOTON2;
	tsSensor 		BTN_OPT;
	tsSensor		PIR_MOV;

	//Sensores Analógicos
	tsSensor		POT1;				// Potenciómetro 1
	tsSensor		POT2;				// Potenciómetro 2

	//Salidas
	tsActuator		RELE;
	tsActuator		TRIAC;

	void 			leerAllSensor(void);
	void 			leerSensor(ptsSensor Sensor);
	static 	void	setActuator(tsActuator *Actuator, bool val);
	static 	void 	manageSensorActuators(TManagerActuators *managerActuators, bool val);
	virtual void    clear_Actuators();
	void 			enableSensor(ptsSensor Sensor, bool enabled);
};

extern DomoBoard domoboard;


#endif /* DOMOBOARD_H_ */
