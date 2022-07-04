/*
 * utils_domoBoard.h
 *
 *  Created on: 10/03/2015
 *      Author: jctejero
 *
 *  Modified on: 16/03/2022
 */

#ifndef UTILS_DOMOBOARD_H_
#define UTILS_DOMOBOARD_H_

/****************************************************************************/
/***        Include files                                                 ***/
/****************************************************************************/
#include	"Arduino.h"
#include	"domoBoard.h"
#include	"ModbusDomoboard.h"

/****************************************************************************/
/***        Macro Definitions                                             ***/
/****************************************************************************/
#define 	ON   				HIGH
#define     OFF					LOW

/****************************************************************************/
/***        Exported Functions                                            ***/
/****************************************************************************/
void Interruptor(void *Sensor);
void Pulsado_Soltado(void *Sensor);
void conmutador(void *Sensor);

void mbInterruptor(void *mbSensor);


#endif /* UTILS_DOMOBOARD_H_ */
