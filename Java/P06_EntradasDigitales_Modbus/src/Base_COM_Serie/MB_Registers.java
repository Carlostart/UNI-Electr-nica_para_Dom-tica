package Base_COM_Serie;

public class MB_Registers {
/*	
	//Discrete Output Coils
	public static final int MB_RELE 		= 0;
	public static final int MB_TRIAC 		= 1;
	public static final int MB_O_COILS		= 2;
		
	//Discrete Input Contacts (Digital Registers)
	public static final int MB_BTN1			= 0;				// Pulsador 1
	public static final int MB_BTN2			= 1;				// Pulsador 2
	public static final int MB_OPT			= 2;				// Entrada Optocoplada	
	public static final int MB_I_REGS		= 3;	 			// total number of registers on slave	
	
	public static final int BTN1_DF			= 0;				// Pulsador 1
	public static final int BTN2_DF			= 0;				// Pulsador 2
	public static final int OPT_DF			= 1;				// Entrada Optocoplada
*/
	
	public enum MB_Discrete_Output_Coils {
		
		//Discrete Output Coils
		MB_RELE(0),
		MB_TRIAC(1),
		MB_O_COILS(2);
		
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
		MB_I_REGS(3);
		
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
}
