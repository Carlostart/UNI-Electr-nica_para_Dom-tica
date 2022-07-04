package Base_COM_Serie;

import Base_COM_Serie.MB_Registers.TSwitchState;
import CommTransport.CommTransport;
import modbus.Const_Modbus;
import modbus.Modbus;

public class ModBus_Communications {
	
	public static void writeCoil(int SlaveAddress, int ModBusRegister, TSwitchState coilState, CommTransport sn_Transport) {
		
		String[] args = {Integer.toString(SlaveAddress), String.valueOf(Const_Modbus.WRITE_COIL), 
				Integer.toString(ModBusRegister), 
				TSwitchState.ToNumberString(coilState)};
		
		//Iniciamos Comunicación
		Modbus.InitComunication(args, sn_Transport);	
	}
}
