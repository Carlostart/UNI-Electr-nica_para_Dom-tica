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

#define		CTE_NORMA	1000		//1000 ms = 1 Sec.

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
	static TctrlTemp	ctrlTempPIR;  //La declaramos static para que al volver de la funci�n,

	//Inicialmente, habilitamos el sensor PIR
	domoboard.enableSensor(mbDomoboard.PIR_MOV.Sensor, true);
	Cregs[MB_ACTPIR] = 0x01;  //Actualiza registro que monitoriza la habilitaci�n del sensor


	//Borramos los actuadores previos asignado a cada sensor de entrada
//	mbDomoboard.clear_Actuators();


	//Inicializamos control de temporizaci�n
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


void SelectionConfiguration(uint8_t selConf){
	mbDomoboard.clear_Actuators();
	AccionesTemporizadas.clear();


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
	}
}

