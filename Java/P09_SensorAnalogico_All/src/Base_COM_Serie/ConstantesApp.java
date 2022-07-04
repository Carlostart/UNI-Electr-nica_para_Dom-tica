package Base_COM_Serie;

import CommTransport.Comm.tCommConnector;

public class ConstantesApp 
{
	//IniFiles Params
		public static final String 			CONFIG_FILE = "Base_COMSerie.conf";
		
		public static final String 			WINDOW_TITLE = "PRÁCTICAS ELECTRÓNICA PARA DOMÓTICA - COMUNICACIONES MODBUS ";
		public static final String 			STATUSBAR_MSGLEFT = "Puerto seleccionado para comunicación ModBus : ";
			
		public static final String			SERIALCONSOLE = "serialConsole";
		
		public static final int				TIME_REGULARCALL = 250;	//tiempo llamada regular en miisegundos
		
		//Setup
		//TODO: Incluir la selección del conector serie en una forma de configuración 
		public static final tCommConnector	SERIALCONNECTION = tCommConnector.CN_JSSC;  //tCommConnector.CN_RXTX; //
}