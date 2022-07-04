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
#include "Gest_Modbus.h"


//#define DEBUG_TEMP

/*************************************************************************/
/***                   Constantes Control Persiana                     ***/
/*************************************************************************/

#define	STEP_CTRL_BLIND		5
#define PERCENTAGE(x)		((x % STEP_CTRL_BLIND) > 2) ? x + (STEP_CTRL_BLIND-(x % STEP_CTRL_BLIND)) : x - (x % STEP_CTRL_BLIND);
#define INCR_PERCENTAGE		1


const uint16_t	persianaTiempoParada 	= 1000;
const uint16_t	persianaTiempoAbierta 	= 5000;

//Retraso para compensar la detección de la fase
#define		DELAY_PHASE		10
#define 	BEFORE_PHASE	90

/******************************************************************/
/***                      Variables Locales                     ***/
/******************************************************************/
//Lista de acciones temporizadas
QueueList<TpmbSensor>		AccionesTemporizadas;
QueueList<TGestorTime>		basetimer_queue;

/*============================================*/
/*		 		  INTERRUPTOR                 */
/*============================================*/

void Interruptor(void *Sensor)
{
	if(((ptsSensor)Sensor)->valor_Df == ((ptsSensor)Sensor)->valor){
		DEBUGLOG("%s : Interruptor --> ", DEVICENAME(domoboard.BOTON1.name));
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

/*****************************************************************************************/
/***********************************  Leer Temperatura ***********************************/
/*****************************************************************************************/
//mas/menos 1 - 2ºC de exactitud para el TMP36, por lo que escribimos valores decimales
void Calc_Temperatura(void *Sensor){
	float valTMP;
	int   temp;

	TpmbSensor sensor = reinterpret_cast<TpmbSensor>(Sensor);

	valTMP = sensor->Sensor->valor*.004882812;	//Conviere resultado convertidor a voltios

	valTMP = (valTMP - .5)*100;          //Convierte Temperatura de 10 mV por grado con 500 mV de Offset


	temp = (valTMP - (int)valTMP)*100;

    if(temp < 50) temp = 0;
    //else if ((25 < temp)&&(temp < 75)) temp = 5;
    //else if ((75 < temp)&&(temp <= 99)){
    else{
    	temp = 0;
    	valTMP = (int)valTMP +1;
    }

	*(sensor->mbRegs) = (((int)valTMP & 0xff) << 8) | (temp & 0xff);

#ifdef DEBUG_TEMP
    static float vT = 0;

    if(vT != valTMP){
    	vT = valTMP;
    	/*
    	DEBUGF("Temperatura = " );
    	Serial.print((int)valTMP,DEC);
    	Serial.print(".");
    	Serial.println(temp,DEC);
    	*/
    	DEBUGLOG("Temperatura = %d.%d\n", (int)valTMP, temp);

    //	Serial.print("Temperatura F -> ");
    //	Serial.println(Sensor->Regs_App[Sensor->MBReg_App],BIN);
    }
#endif
}

/*============================================*/
/*			  INTERRUPTOR LUMINOSIDAD         */
/*============================================*/
/*
 * Interruptor por nivel de luminosidad. funcionará con una histeresis, es decir,
 * Si el interruptor está desactivado, se activará cuando alcance el "highlevel".
 *
 * Si el interruptor está ativado, se desactivará cuando alcanze el "lowlevel".
 */
void	interruptor_SRC(void *Sensor){
	TpmbSensor sensor = reinterpret_cast<TpmbSensor>(Sensor);

	int state = sensor->Aux;

	switch(state){
	case 1:
		if(sensor->Sensor->valor <= (int)*(sensor->ctrlLevelPtr.lInf)){
			state = 2;
			//Estado del sensor ha cambiado
			mbDomoboard.manager_mbActuators(&(sensor->mbActuators), (TStateDigitalDev)ON);
		}

		break;

	case 2:
		if(sensor->Sensor->valor >= (int)*(sensor->ctrlLevelPtr.lSup)){
			state = 1;
			mbDomoboard.manager_mbActuators(&(sensor->mbActuators), (TStateDigitalDev)OFF);
		}

		break;

	default:
		state = 2;
		mbDomoboard.set_coilRegister(MB_TRIAC, OFF);
	}

	sensor->Aux = state;
}

/*============================================*/
/*			  TRIGGER WITH LUMINOSIDAD        */
/*============================================*/
/*
 * tRIGGER por nivel de luminosidad. Dispara un evento cuando el nivel del sensor
 * se encuentra por debajo de un determinado nivel
 */
void	trigger_Level(void *Sensor){
	TpmbSensor sensor = reinterpret_cast<TpmbSensor>(Sensor);

	uint16_t limInfe = *(sensor->ctrlLevelPtr.lInf);

	if(((uint16_t) sensor->Sensor->valor <= limInfe)){
		if(sensor->Aux == 0){
			mbDomoboard.manager_mbActuators(&(sensor->mbActuators), (TStateDigitalDev)ON);
			sensor->Aux = 1;
		}
	} else sensor->Aux = 0;
}

/*============================================*/
/*			    CONTROL DE PERSIANA           */
/*============================================*/
/*============================================*/
/*			     Control Persiana             */
/*============================================*/
void UpDown_Persiana(){
	bool UpP,DownP;
	tsStaPer state = (tsStaPer)(Aregs[MB_STAPER]&0xFF);

	UpP = (bool)(*mbDomoboard.PERUP.mbRegs);
	DownP = (bool)(*mbDomoboard.PERDOWN.mbRegs);

    switch(state){
    	case PER_STOP: //Parada
    		if (UpP == ON) {
    			state = PER_UP; //Subiendo
    		}

    		if (DownP == ON) {
    			state = PER_DOWN; //Bajando
    		}

    		break;

    	case PER_UP: //Subiendo
    		if(!UpP && !DownP) {
    			state = PER_STOP;
    		}

    		if(!UpP && DownP){
    			state = PER_STOP2;
    		}
    		break;

    	case PER_DOWN: //Bajando
    		if(!UpP && !DownP) {
    			state = PER_STOP;
    		}

    		if(UpP && !DownP){
    			state = PER_STOP2;
    		}
    		break;

    	case PER_STOP2:
    		if(!UpP && !DownP) {
    			state = PER_STOP;
    		}
    		break;

    }

    if(Aregs[MB_STAPER] != state){
    	Aregs[MB_STAPER] = state;
    	writeholdingregister(MB_STAPER);
    }
}

void Persiana(void *Sensor){

	TpmbSensor sensor = reinterpret_cast<TpmbSensor>(Sensor);

	//Leemos estado actual de los actuadores
	for(int n = 0; n < sensor->mbActuators.count(); n++){
		*Actuator(sensor, n)->mbRegs = sensor->Sensor->valor != sensor->Sensor->valor_Df;
	}

	UpDown_Persiana();
}

void Ctrl_PosicionPersiana(TctrlTimePtr ctrlPosPer, tsStaPer staPer){
	static tsStaPer lastStePer = PER_STOP;

	// lambda function
	auto actualizeTime = [&](int8_t sign){

		*ctrlPosPer->actTime = *ctrlPosPer->actTime + sign*(millis() - ctrlPosPer->time_lastAct);

		if(*ctrlPosPer->actTime < 0){
			*ctrlPosPer->actTime = 0;
		}else if(uint16_t(*ctrlPosPer->actTime) > *ctrlPosPer->maxTime){
			*ctrlPosPer->actTime = *ctrlPosPer->maxTime;
		}

		uint8_t per = PERCENTAGE((*ctrlPosPer->actTime)*100/(*ctrlPosPer->maxTime));

		if(per != Iregs[MB_POSPER]){
			Iregs[MB_POSPER] = per;
		}
	};

	switch(staPer){
	case PER_STOP:
	case PER_STOP2:
		ctrlPosPer->activa = false;
		switch(lastStePer){
		case PER_DOWN:
			actualizeTime(-1);
			break;
		case PER_UP:
			actualizeTime(1);
			break;
		default:
			break;
		}
		break;

	case PER_DOWN:
		if(ctrlPosPer->activa){
			//Continúa bajado
			actualizeTime(-1);
		}else{
			//Comienza a bajada
			ctrlPosPer->activa = true;
		}
		ctrlPosPer->time_lastAct = millis();
		break;

	case PER_UP:
		if(ctrlPosPer->activa){
			//Continúa subiendo
			actualizeTime(1);
		}else{
			//Comienza a subir
			ctrlPosPer->activa = true;
		}
		ctrlPosPer->time_lastAct = millis();
		break;
	}

	lastStePer = staPer;
}

/*============================================*/
/*			  Control Puerta Garage           */
/*============================================*/
void PuertaGarage(void  *Sensor){
	TpmbSensor sensor = reinterpret_cast<TpmbSensor>(Sensor);

	static uint8_t state = 'A';
	tsStaPer statePer = (tsStaPer)(Aregs[MB_STAPER]&0xFF);

	TmbRelatedSensors *RelatedSensors = (TmbRelatedSensors *)sensor->RelatedSensors;

	// lambda function
	auto checkSensors = [&](){

		if(Cregs[MB_KEYGAR]){
			return true;
		}else if(sensor->ctrlTempPtr != NULL){
			//BTN1 Sensor
			if(sensor->Sensor->valor != sensor->Sensor->valor_Df){
				//Llave/Btn1 Accionado
				Cregs[MB_KEYGAR] = true;
				return true;
			}
		}else{
			//FhotoTTor Sensor

			uint16_t limInfe = *(sensor->ctrlLevelPtr.lInf);

			if(((uint16_t) sensor->Sensor->valor) <= limInfe){
				//Barrera de luz interrumpida
				return true;
			}
		}

		return false;
	};

	// lambda function
	auto Init_timelastAct = [&](uint16_t *tmpAct = NULL){
		if(sensor->ctrlTempPtr != NULL){
			sensor->ctrlTempPtr->time_lastAct = millis();
			if(tmpAct != NULL)
				sensor->ctrlTempPtr->time_activo = tmpAct;

		}else if(RelatedSensors->mbRelatedSensors.count() > 0){
			RelatedSensors->mbRelatedSensors.peek(0)->ctrlTempPtr->time_lastAct = millis();
			if(tmpAct != NULL)
				RelatedSensors->mbRelatedSensors.peek(0)->ctrlTempPtr->time_activo = tmpAct;

		}
	};


	switch(state){
	case 'A':
		//Sólo se debe abrir con llave/Btn1
		if(sensor->ctrlTempPtr != NULL){
			if(*sensor->mbRegs || Cregs[MB_KEYGAR]){
				Cregs[MB_KEYGAR] = false;
				state = 'B';		//llave accionada, abrimos puerta
				statePer = PER_UP;
			}
		}
		break;

	case 'B':
		if(Iregs[MB_POSPER] >= 100){
			//Puerta abierta
			state = 'C';
			statePer = PER_STOP;

			Init_timelastAct((uint16_t *)&persianaTiempoAbierta);
		}
		break;

	case 'C':
		//Comprobar estado BTN1 y FotoTTOR
		if(checkSensors()){
			Cregs[MB_KEYGAR] = false;
			Init_timelastAct();
		}else{
			if(sensor->ctrlTempPtr != NULL){
				if((millis() - sensor->ctrlTempPtr->time_lastAct) > *sensor->ctrlTempPtr->time_activo){
					state = 'D';
					statePer = PER_DOWN;
				}
			}
		}
		break;

	case 'D':
		//Comprobar estado BTN1 y FotoTTOR
		if(checkSensors()){
			//Llave/Btn1 Accionada, o Barrera de luz (fotottor) interrumpida
			Cregs[MB_KEYGAR] = false;
			state = 'E';
			statePer = PER_STOP;

			//Inicia temporizador para esperar 1 segundo
			Init_timelastAct((uint16_t *)&persianaTiempoParada);

		}else{
			if(Iregs[MB_POSPER] == 0){
				state = 'A';
				statePer = PER_STOP;
			}
		}
		break;

	case 'E':
		if(sensor->ctrlTempPtr != NULL){
			if((millis() - sensor->ctrlTempPtr->time_lastAct) > *sensor->ctrlTempPtr->time_activo){
				state = 'B';
				statePer = PER_UP;
			}
		}
		break;

	default:
		break;
	}

	if(Aregs[MB_STAPER] != statePer){
		Aregs[MB_STAPER] = statePer;
		writeholdingregister(MB_STAPER);
	}
}

/**********************************************************
 * 						DIMMERS
 **********************************************************/
/*
 * Timer usado por el sistema. Es llamado cada 0.1 mSeg. Configurado
 * por la librería TimerOne
 */
void BaseTiempo(){
	for(int i = 0; i<basetimer_queue.count(); i++){
		(basetimer_queue.peek(i))();
	}
}

void Gestor_Dimmer(TpmbActuator dimmer)
{
	TDimmerPtr	dim = (TDimmerPtr)dimmer->actuator;
/*
	DEBUG("GD : ");
	DEBUGLN(millis());
*/
	if(!dim->Pulse_Act){
//		DEBUGLOG("%s : %d\n", dim->name, *dimmer->mbRegs);


		if(dim->delay_timmer > BEFORE_PHASE){
			digitalWrite(dim->pin, LOW);
			//dim->Pulse_Act = false;
		} else if((dim->delay_timmer >= *(dimmer->mbRegs)) && (dim->delay_timmer > DELAY_PHASE)){
			digitalWrite(dim->pin, HIGH);
			dim->val_act = dim->delay_timmer;
			dim->Pulse_Act = true;
		}
	}

	if(dim->delay_timmer >= (dim->val_act + 3)){
		digitalWrite(dim->pin, LOW);
		//dim->Pulse_Act = false;
	}

	dim->delay_timmer++;
}

//Base tiempo Dimmer
void BaseTiempo_Dimmer(void){
	//Comprobamos el estado de dimmer cada 0.1 milisegundo.
	//De esta forma, podemos dividir el semiciclo de la señal de alterna en 100 partes, 0 - 100%
	if(lDimmers.count() > 0){

		TpmbActuator DIMMER;

		for(int n = 0; n < lDimmers.count(); n++){
			DIMMER = lDimmers.peek(n);

			if(DIMMER->actuator != NULL){
/*
				DEBUGLN("DA");
				//DEBUGLN(millis());
*/
				if(((TDimmerPtr)(DIMMER->actuator))->Enabled){
					Gestor_Dimmer(DIMMER);
				}
			}
		}
	}
}

void zero_crossing(void){
	if(lDimmers.count() > 0){
		TpmbActuator DIMMER;

		for(int n = 0; n < lDimmers.count(); n++){
			DIMMER = lDimmers.peek(n);

			if(DIMMER->actuator != NULL){
				if(((TDimmerPtr)(DIMMER->actuator))->Enabled){
					mbDomoboard.IniciaDimmer((TDimmerPtr)(DIMMER->actuator));
				}
			}
		}

		BaseTiempo_Dimmer();
	}
}

void Activa_Dimmers(){
	//Capturamos la interrupción del detector de fase usado por el dimmer
	attachInterrupt(0, zero_crossing, RISING);

	//Base de tiempo para gestionar el Dimmer
	basetimer_queue.push(BaseTiempo_Dimmer);

}

void Desactiva_Dimmers(){

	detachInterrupt(digitalPinToInterrupt(PZERO_P));	//Liberamos interrupción paso por zero

	//Base de tiempo para gestionar el Dimmer
	basetimer_queue.clear();

	lDimmers.clear();

	Timer1.detachInterrupt();
}

/*
 * Mapea la señal del Sensor analógico entre 0 y 100 % para ser usado por el dimmer
 */
int	mapSensorToDimmer(ptsSensor sensor){
	return map(sensor->valor, 0, 1023, 0, 100);
}


void	AnalogValueToDimmer(void  *Sensor){
	TpmbSensor sensor = reinterpret_cast<TpmbSensor>(Sensor);

	TpmbActuator mbActuator;
	int mapSensor;

	for(int n = 0; n < sensor->mbActuators.count(); n++){
		mbActuator = sensor->mbActuators.peek(n);

		if(!((TDimmerPtr)mbActuator->actuator)->Enabled)
			return;

		mapSensor = mapSensorToDimmer(sensor->Sensor);

		if(abs((int)(*(mbActuator->mbRegs) - mapSensor)) >= INCR_PERCENTAGE){
			*(mbActuator->mbRegs)  = mapSensor;
		}
	}
}

//*********************************************************
//*********************************************************

/*====================================================================================
 * Llamada regular usada para gestionar los temporizadores usados por la aplicación. =
 * es llamada cada LOOP_TIME (10 ms)
 *====================================================================================*/
void AccionesTemporizadas_Timer(void){
	for(int n=0; n<AccionesTemporizadas.count(); n++){
		AccionesTemporizadas.peek(n)->mbSensorEvent(AccionesTemporizadas.peek(n));
	}
}





