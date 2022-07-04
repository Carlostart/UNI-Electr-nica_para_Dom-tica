package Base_COM_Serie;

public class MB_Registers {
	
	//Selección configuración Práctica
	public static final int SELPRACT[] = {0x11, 0x12, 0x13, 0x32, 0x41, 0x42, 0x43, 0x54, 0x62, 0x70};
/*	
	public enum PRACTICA{
		PRACTICA1(0x11, 0x12, 0x13),
		PRACTICA6(0x61, 0x62, 0x63);
		
		private int ap1;
		private int ap2;
		private int ap3;
		
		PRACTICA(int ap1, int ap2, int ap3){
			this.ap1 = ap1;
			this.ap2 = ap2;
			this.ap3 = ap3;
		}
		
		public int getAp(int ap) {
			switch(ap) {
			case 1:
				return ap1;
				
			case 2:
				return ap2;
				
			case 3:
				return ap3;
				
			default:
				return 0x00;
			}
		}
	}
*/	
	//Analog Output Holding Registers
	public enum MB_Analog_Output_Holding {
			
		//Discrete Output Coils
		MB_PRACT(0),			// Registro para indicar la práctica con la que trabajamos
		MB_TMP_PIR(1),			// Registro para controlar el tiempo activo del sensor PIR (Segundos)
		MB_AREGS(2);
			
		int reg;
			
		MB_Analog_Output_Holding(int rg){
			reg = rg;
		}
			
		public int getReg(){
			return reg;
		}
			
		public void setReg(int rg) {
			reg = rg;
		}
	}
		
	public enum MB_Discrete_Output_Coils {
		
		//Discrete Output Coils
		MB_RELE(0),
		MB_TRIAC(1),
		MB_ACTPIR(2),					//Salida virtual para Activar/Desactivar PIR
		MB_O_COILS(3);
		
		int reg;
		
		MB_Discrete_Output_Coils(int rg){
			reg = rg;
		}
		
		public int getReg(){
			return reg;
		}
		
		public void setReg(int rg) {
			reg = rg;
		}
	}
	
	public enum MB_Discrete_Input_Contacts {
		
		//Discrete Output Coils
		MB_BTN1(0),
		MB_BTN2(1),
		MB_OPT(2),
		MB_PIRMOV(3),			//Sensor de movimiento
		MB_I_REGS(4);
		
		int reg;
		
		MB_Discrete_Input_Contacts(int rg){
			reg = rg;
		}
		
		public int getReg(){
			return reg;
		}
		
		public void setReg(int rg) {
			reg = rg;
		}
		
		public int getDefaultValue() {
			switch(this) {
			case MB_BTN1:
				return 0;
			case MB_BTN2:
				return 0;
			case MB_OPT:
				return 1;
			default:
				return -1;
			}
		}
	}
	
public enum MB_Analog_Input_Register {
		
		//Discrete Output Coils
		MB_POT1(0),
		MB_POT2(1),
		MB_I_AREGS(2);
		
		int reg;
		
		MB_Analog_Input_Register(int rg){
			reg = rg;
		}
		
		public int getReg(){
			return reg;
		}
		
		public void setReg(int rg) {
			reg = rg;
		}
		
		public int getDefaultValue() {
			switch(this) {
			case MB_POT1:
				return 0;
			case MB_POT2:
				return 1;
			default:
				return -1;
			}
		}
	}
	
	public enum TSwitchState { 
		OFF, 
		ON,  
		TOGGLE;
		
		public static String ToNumberString(TSwitchState x) {
	        switch(x) {
	        case OFF:
	            return "0";
	        case ON:
	        	return "1";
	        case TOGGLE:
	        	return "2";
	        default:
	        	return "0";
	        }
	    }
		
	}
	
	public enum TComandos{
		RESEST_STACIONES;
	}
}
