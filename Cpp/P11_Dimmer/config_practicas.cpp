/*
 * config_practicas.cpp
 *
 *  Created on: 10/03/2015
 *      Author: jctejero
 *
 *  Modified on: 16/03/2022
 */

/****************************************************************************/
/***        Include files                                                 ***/
/****************************************************************************/
#include	"config_practicas.h"

#define		CTE_NORMA			1000		//1000 ms = 1 Sec.
#define		BASETIEMPO_DIMMERS	100			//micro seconds

const uint16_t	persianaTiempoSubida 	= 7200;  //milisegundos

TctrlTime 	ctrlPorPer;			//Control del tanto porciento de apertura de la parsiana

/****************************************************************************/
/***                 Functions                                            ***/
/****************************************************************************/

void config_practica1_apt_1(){

//	mbDomoboard.clear_Actuators();

	domoboard.BOTON1.SensorEvent = Pulsado_Soltado;

	domoboard.BOTON2.SensorEvent = Pulsado_Soltado;

	domoboard.BTN_OPT.SensorEvent = Pulsado_Soltado;
}

void config_practica1_apt_2(){

//	DEBUGLN("pt. 1");
//	mbDomoboard.clear_Actuators();
//	DEBUGLN("pt. 2");

	domoboard.BOTON1.SensorEvent = Interruptor;
	domoboard.BOTON1.Aux = OFF;

	domoboard.BOTON2.SensorEvent = Interruptor;
	domoboard.BOTON2.Aux = OFF;

	domoboard.BTN_OPT.SensorEvent = Interruptor;
	domoboard.BTN_OPT.Aux = OFF;
//	DEBUGLN("pt. 3");
}

void config_practica1_apt_3(){

//	mbDomoboard.clear_Actuators();

	domoboard.BOTON1.SensorEvent = conmutador;

	domoboard.BOTON2.SensorEvent = conmutador;

	domoboard.BTN_OPT.SensorEvent = conmutador;
}

void config_practica3_apt_2(){

//	mbDomoboard.clear_Actuators();

	domoboard.BOTON1.SensorEvent = conmutador;

	domoboard.BOTON2.SensorEvent = conmutador;

	domoboard.BTN_OPT.SensorEvent = conmutador;
}

void config_practica4_apt_1(){

//	mbDomoboard.clear_Actuators();

	domoboard.BOTON1.SensorEvent = Pulsado_Soltado;
	domoboard.BOTON1.managerActuators.push(&(domoboard.RELE));

	domoboard.BOTON2.SensorEvent = Pulsado_Soltado;
	domoboard.BOTON2.managerActuators.push(&(domoboard.TRIAC));

	domoboard.BTN_OPT.SensorEvent = Pulsado_Soltado;
}

void config_practica4_apt_2(){

//	mbDomoboard.clear_Actuators();

	domoboard.BOTON1.managerActuators.clear();
	domoboard.BOTON1.SensorEvent = Interruptor;
	domoboard.BOTON1.managerActuators.push(&(domoboard.RELE));


	domoboard.BOTON2.SensorEvent = Interruptor;
	domoboard.BOTON2.managerActuators.push(&(domoboard.TRIAC));

	domoboard.BTN_OPT.SensorEvent = Interruptor;
}

void config_practica4_apt_3(){

//	mbDomoboard.clear_Actuators();

	domoboard.BOTON1.SensorEvent = conmutador;
	domoboard.BOTON1.managerActuators.push(&(domoboard.RELE));
	domoboard.BOTON1.managerActuators.push(&(domoboard.TRIAC));

	domoboard.BOTON2.SensorEvent = conmutador;
	domoboard.BOTON2.managerActuators.push(&(domoboard.RELE));
	domoboard.BOTON2.managerActuators.push(&(domoboard.TRIAC));

	domoboard.BTN_OPT.SensorEvent = conmutador;
	domoboard.BTN_OPT.managerActuators.push(&(domoboard.RELE));
	domoboard.BTN_OPT.managerActuators.push(&(domoboard.TRIAC));
}


void config_practica5_apt_4(){
//	mbDomoboard.clear_Actuators();

	mbDomoboard.BOTON1.mbSensorEvent = mbInterruptor;
	mbDomoboard.BOTON1.mbActuators.push(&mbDomoboard.RELE);

	mbDomoboard.BOTON2.mbSensorEvent = mbInterruptor;
	mbDomoboard.BOTON2.mbActuators.push(&mbDomoboard.TRIAC);

}

void config_practica6_apt_3(){
//	mbDomoboard.clear_Actuators();

	mbDomoboard.BOTON1.mbSensorEvent = mbInterruptor;
	mbDomoboard.BOTON1.mbActuators.push(&(mbDomoboard.TRIAC));

	mbDomoboard.BOTON2.mbSensorEvent = mbInterruptor;
	mbDomoboard.BOTON2.mbActuators.push(&(mbDomoboard.RELE));

	mbDomoboard.BTN_OPT.mbSensorEvent = mbInterruptor;
}

void Config_P7_SensorMovimiento(){
	static TctrlTemp	ctrlTempPIR;  //La declaramos static para que al volver de la función,

	//Inicialmente, habilitamos el sensor PIR
	domoboard.enableSensor(mbDomoboard.PIR_MOV.Sensor, true);

	mbDomoboard.set_coilRegister(MB_ACTPIR, 0x01);		//Enable PIR


	//Borramos los actuadores previos asignado a cada sensor de entrada
	mbDomoboard.clear_Actuators();


	//Inicializamos control de temporización
	ctrlTempPIR.estado = false;
	ctrlTempPIR.Cte_Norma = CTE_NORMA;
	ctrlTempPIR.time_lastAct = millis();
	ctrlTempPIR.time_activo = &(Aregs[MB_TMP_PIR]);

	mbDomoboard.PIR_MOV.mbSensorEvent = Interruptor_Temporizado;
	mbDomoboard.PIR_MOV.ctrlTempPtr = &ctrlTempPIR;
	mbDomoboard.PIR_MOV.mbActuators.push(&(mbDomoboard.TRIAC));

	mbDomoboard.BOTON1.mbSensorEvent = mbConmutador;
	mbDomoboard.BOTON1.mbActuators.push(&(mbDomoboard.RELE));

	mbDomoboard.BOTON2.mbSensorEvent = mbConmutador;
	mbDomoboard.BOTON2.mbActuators.push(&(mbDomoboard.RELE));

	mbDomoboard.BTN_OPT.mbSensorEvent = mbConmutador;
	mbDomoboard.BTN_OPT.mbActuators.push(&(mbDomoboard.RELE));

//	AccionesTemporizadas.clear();
	AccionesTemporizadas.push(&(mbDomoboard.PIR_MOV));

}

void Config_P9_SRC_Interruptor(void){

	AccionesTemporizadas.clear();
	mbDomoboard.set_coilRegister(MB_ACTPIR, 0x00);		//Disable PIR

	//Borramos los actuadores previos asignado a cada sensor de entrada
	mbDomoboard.clear_Actuators();

	//Config Photoresistor
	mbDomoboard.PHOTORES.ctrlLevelPtr.lInf = &(Aregs[MB_SRC_LL]);
	mbDomoboard.PHOTORES.ctrlLevelPtr.lSup = &(Aregs[MB_SRC_HL]);
	mbDomoboard.PHOTORES.mbSensorEvent = interruptor_SRC;
	mbDomoboard.PHOTORES.mbActuators.push(&(mbDomoboard.TRIAC));

	//Config PhotoTtor
	mbDomoboard.PHOTOTTOR.ctrlLevelPtr.lInf = &(Aregs[MB_TTOR_LL]);
	mbDomoboard.PHOTOTTOR.Aux = 0;
	mbDomoboard.PHOTOTTOR.mbSensorEvent = trigger_Level;
	mbDomoboard.PHOTOTTOR.mbActuators.push(&(mbDomoboard.RELE));

	mbDomoboard.BOTON1.mbSensorEvent = mbConmutador;
	mbDomoboard.BOTON1.mbActuators.push(&(mbDomoboard.RELE));

	mbDomoboard.BOTON2.mbSensorEvent = mbConmutador;
	mbDomoboard.BOTON2.mbActuators.push(&(mbDomoboard.RELE));

	mbDomoboard.BTN_OPT.mbSensorEvent = mbConmutador;
	mbDomoboard.BTN_OPT.mbActuators.push(&(mbDomoboard.RELE));
}

void Config_P10_CtrlPersianas(){
	static int32_t		TiempoSubidaActual = 0;

	ctrlPorPer.activa		= false;
	ctrlPorPer.maxTime		= (uint16_t *)&persianaTiempoSubida;
	ctrlPorPer.actTime		= &TiempoSubidaActual;

	AccionesTemporizadas.clear();

	//Borramos los actuadores previos asignado a cada sensor de entrada
	mbDomoboard.clear_Actuators();

	mbDomoboard.BOTON1.mbSensorEvent = Persiana;
	mbDomoboard.BOTON1.mbActuators.push(&(mbDomoboard.PERUP));

	mbDomoboard.BOTON2.mbSensorEvent = Persiana;
	mbDomoboard.BOTON2.mbActuators.push(&(mbDomoboard.PERDOWN));
}

void Config_P10_CtrlGarage(){
	static int32_t				TiempoSubidaActual = 0;
	static TmbRelatedSensors 	RelatedSensors;			//Lista de sensores relacionados/implicados en la utilidad
	static TctrlTemp			ctrlTempGar;  			//La declaramos static para que al volver de la función,

	//Inicializamos control de temporización
	ctrlTempGar.estado = false;
	ctrlTempGar.Cte_Norma = 1000;
	ctrlTempGar.time_lastAct = millis();

	ctrlPorPer.activa		= false;
	ctrlPorPer.maxTime		= (uint16_t *)&persianaTiempoSubida;
	ctrlPorPer.actTime		= &TiempoSubidaActual;

	AccionesTemporizadas.clear();

	//Borramos los actuadores previos asignado a cada sensor de entrada
	mbDomoboard.clear_Actuators();

	RelatedSensors.mbRelatedSensors.clear();
	RelatedSensors.mbRelatedSensors.push(&(mbDomoboard.BOTON1));

	mbDomoboard.BOTON1.mbSensorEvent = PuertaGarage;
	mbDomoboard.BOTON1.ctrlTempPtr = &ctrlTempGar;

	mbDomoboard.PHOTOTTOR.ctrlLevelPtr.lInf = &(Aregs[MB_TTOR_LL]);
	mbDomoboard.PHOTOTTOR.ctrlTempPtr = NULL;
	mbDomoboard.PHOTOTTOR.mbSensorEvent = PuertaGarage;
	mbDomoboard.PHOTOTTOR.RelatedSensors = &RelatedSensors;

	//Para comprobar regularmente el estado de la puerta del garage
	AccionesTemporizadas.clear();
	AccionesTemporizadas.push(&(mbDomoboard.BOTON1));
}

void Config_P11_CtrlDimmer(){
	lDimmers.push(&(mbDomoboard.DIMMER1));
	lDimmers.push(&(mbDomoboard.DIMMER2));

	mbs.writecoil(MB_POT1DIM1);
	mbDomoboard.POT1.mbActuators.push(&(mbDomoboard.DIMMER1));

	mbs.writecoil(MB_POT2DIM2);
	mbDomoboard.POT2.mbActuators.push(&(mbDomoboard.DIMMER2));

	//***
	//* Iniciamos Timer
	//***
	Timer1.initialize(BASETIEMPO_DIMMERS);        	// initialize timer1, and set a 0.1 second period
	Timer1.attachInterrupt(BaseTiempo);     		// attaches callback() as a timer overflow interrupt


	Activa_Dimmers();
}

void Config_P11_ILUM_CONST(){
	lDimmers.push(&(mbDomoboard.DIMMER1));

	mbDomoboard.PHOTORES.mbSensorEvent = AnalogValueToDimmer;
	mbDomoboard.PHOTORES.mbActuators.push(&(mbDomoboard.DIMMER1));

	//***
	//* Iniciamos Timer
	//***
	Timer1.initialize(BASETIEMPO_DIMMERS);        	// initialize timer1, and set a 0.1 second period
	Timer1.attachInterrupt(BaseTiempo);     		// attaches callback() as a timer overflow interrupt


	Activa_Dimmers();
}


void SelectionConfiguration(uint8_t selConf){
	mbDomoboard.clear_Actuators();
	AccionesTemporizadas.clear();

	Desactiva_Dimmers();


	switch(selConf){
		case P1_PULSADORES:
			DEBUGLNF("P01 PULSADORES Seleccionado");
			config_practica1_apt_1();
			break;

		case P1_INTERRUPTOR:
			DEBUGLNF("P01 INTERRUPTOR Seleccionado");
			config_practica1_apt_2();
			break;

		case P1_CONMUTADOR:
			DEBUGLNF("P01 CONMUTADOR Seleccionado");
			config_practica1_apt_3();
			break;

		case P3_CONMUTADOR:
			DEBUGLNF("P03 CONMUTADOR Seleccionado");
			config_practica3_apt_2();
			break;

		case P4_PULSADORES:
			DEBUGLNF("P04 PULSADORES Seleccionado ");
			config_practica4_apt_1();
			break;

		case P4_INTERRUPTOR:
			DEBUGLNF("P04 INTERRUPTOR Seleccionado");
			config_practica4_apt_2();
			break;

		case P4_CONMUTADOR:
			DEBUGLNF("P04 CONMUTADOR Seleccionado");
			config_practica4_apt_3();
			break;

		case P5_INTERRUPTOR:
			DEBUGLNF("P05 INTERRUPTOR Seleccionado");
			config_practica5_apt_4();
			break;

		case P6_INTERRUPTOR:
			DEBUGLNF("P06 INTERRUPTOR Seleccionado");
			config_practica6_apt_3();
			break;

		case P7_PIR:
			DEBUGLNF("P07 Seleccionada --> Sensor Movimiento (PIR)");
			Config_P7_SensorMovimiento();
			break;

		case P9_All:
			DEBUGLNF("P09 Seleccionada --> Sensores Analógicos (All)");
			Config_P9_SRC_Interruptor();
			break;

		case P10_PER:
			DEBUGLNF("P10-1 Control de persianas");
			Config_P10_CtrlPersianas();
			break;

		case P10_GAR:
			DEBUGLNF("P10-2 Control de Puerta Garage");
			Config_P10_CtrlGarage();
			break;

		case P11_DIMMER:
			DEBUGLNF("P11 Control de Dimmer");

			Config_P11_CtrlDimmer();
			break;

		case P11_ILUM_CONST:
			DEBUGLNF("P11 Simulador iluminación Constante");
			Config_P11_ILUM_CONST();
			break;
	}
}

