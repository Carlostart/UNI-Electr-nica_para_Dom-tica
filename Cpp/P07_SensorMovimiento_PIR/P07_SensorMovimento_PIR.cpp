// Do not remove the include below
#include 	"Gest_Modbus.h"
#include	"config_practicas.h"
#include 	"P07_SensorMovimiento_PIR.h"

//The setup function is called once at startup of the sketch
void setup()
{
	//Iniciamos Modbus en Modo RTU
	Init_RTU_Modbus();

	//Inicializa los registros necesarios para configurar la práctica 7
	mbDomoboard.set_holdingRegister(MB_SELPRACT, P7_PIR);
	mbDomoboard.set_holdingRegister(MB_TMP_PIR, 0x03);

	DEBUGLNF("P07 Sensor Movimiento PIR");
}

// The loop function is called in an endless loop
void loop()
{
	EXECUTELOOP(){
		UPDATELOOP();

		if(Aregs[MB_SELPRACT] < PRACTICAS_MODBUS){
			domoboard.leerAllSensor();
		}else mbDomoboard.leerAllSensor();

		RTU_ModBus();

		LOOP_x100ms{
			AccionesTemporizadas_Timer();
		}
	}
}
