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
	
	public static int[] readMultipleRegisters(int SlaveAddress, int HoldingRegister, int numReg, CommTransport sn_Transport){
		
		int		Aregs[] = new int [MB_Registers.MB_Analog_Output_Holding.MB_AREGS.getReg()];
		
		String[] args = {Integer.toString(SlaveAddress), String.valueOf(Const_Modbus.READ_MULTIPLE_REGISTERS), 
						String.valueOf(HoldingRegister), String.valueOf(numReg)};
		Modbus.InitComunication(args, sn_Transport, Aregs);
		
		return Aregs;
	}
	
public static void writeSingleRegister(String SlaveAddress, int ModBusRegister, String ValReg, CommTransport sn_Transport) {
		
		String[] args = {SlaveAddress, String.valueOf(Const_Modbus.WRITE_SINGLE_REGISTER), 
				Integer.toString(ModBusRegister), ValReg};
		
		//Iniciamos Comunicación
		Modbus.InitComunication(args, sn_Transport);	
	}
}
