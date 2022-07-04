package gui.Visualizers;

import java.awt.Component;
import java.awt.Color;

import java.awt.event.MouseAdapter;
import java.awt.event.MouseEvent;

import javax.swing.JPanel;

import Base_COM_Serie.MB_Registers;
import Base_COM_Serie.MB_Registers.*;
import Base_COM_Serie.ModBus_Communications;
//import Base_COM_Serie.MB_Registers.MB_Discrete_Input_Contacts;
//import Base_COM_Serie.MB_Registers.MB_Discrete_Output_Coils;
//import Base_COM_Serie.MB_Registers.TSwitchState;
import CommTransport.CommTransport;
import eu.hansolo.steelseries.extras.LightBulb;
import modbus.Const_Modbus;
import modbus.ModBusEvent;
import modbus.Modbus;

import javax.swing.JLabel;

import java.awt.Font;
import javax.swing.border.BevelBorder;
import javax.swing.SwingConstants;
import eu.hansolo.steelseries.extras.Led;
import javax.swing.border.LineBorder;
import javax.swing.AbstractButton;
import javax.swing.JCheckBox;
import javax.swing.JTextField;
import java.awt.event.ActionListener;
import java.awt.event.ActionEvent;

public class DomoBoardGui extends JPanel implements Visualizer {
	/**
	 * 
	 */
	private static final long serialVersionUID = 8619767299083215147L;
	private 		MouseAdapter 		ma_lightBulb;
	private final 	String 				category;
	private final 	boolean				isCategory = true;
	private final	CommTransport 		sn_Transport;
	private final 	String				address;
	private			LightBulb 			lightBulb1;
	private			LightBulb 			lightBulb2;
	private			Led 				ledBtn1;
	private			Led 				ledBtn2;
	private			Led 				ledBtnOpt;
	
	//Banco de registros para mantener sincronizada la comunicación Modbus 
	private  		int 				Cregs[];
	private  		int 				Dregs[];
	private         int 				Aregs[];
	
	private			boolean     		stActualize = true;
	private 		JTextField 			tiempoPIR;
	private			JCheckBox 			cbActPIR;
	private 		Led 				ledPIR;

	public DomoBoardGui(String category, String address, CommTransport sn_Transport) {
		
		super();
		
		this.category 		= category;
		this.address		= address;
		this.sn_Transport 	= sn_Transport;
		
		this.setLayout(null);
		
		//Crea Banco de registros para mantener sincronizada la comunicación Modbus 
		Cregs = new int [MB_Registers.MB_Discrete_Output_Coils.MB_O_COILS.getReg()];
		Dregs = new int [MB_Registers.MB_Discrete_Input_Contacts.MB_I_REGS.getReg()];
		Aregs = new int [MB_Registers.MB_Analog_Output_Holding.MB_AREGS.getReg()];
		
		ma_lightBulb = new MouseAdapter() {
			@Override
			public void mouseClicked(MouseEvent e) {
				ONOFF_Bulb(((LightBulb)e.getComponent()));
			}
		};
		
		JPanel panel = new JPanel();
		panel.setLayout(null);
		panel.setBorder(new BevelBorder(BevelBorder.LOWERED, new Color(0, 0, 255), new Color(0, 255, 0), Color.BLUE, Color.MAGENTA));
		panel.setBounds(10, 22, 248, 100);
		add(panel);
		
		JLabel label = new JLabel("Estado Pulsadores");
		label.setHorizontalAlignment(SwingConstants.CENTER);
		label.setForeground(Color.RED);
		label.setFont(new Font("Times New Roman", Font.PLAIN, 20));
		label.setBounds(0, 75, 248, 14);
		panel.add(label);
		
		ledBtn1 = new Led();
		ledBtn1.setBounds(35, 11, 36, 36);
		panel.add(ledBtn1);
		
		ledBtn2 = new Led();
		ledBtn2.setBounds(111, 11, 36, 36);
		panel.add(ledBtn2);
		
		ledBtnOpt = new Led();
		ledBtnOpt.setBounds(178, 11, 36, 36);
		panel.add(ledBtnOpt);
		
		JLabel label_1 = new JLabel("BTN 1");
		label_1.setForeground(Color.BLUE);
		label_1.setFont(new Font("Tahoma", Font.PLAIN, 12));
		label_1.setBounds(35, 39, 36, 14);
		panel.add(label_1);
		
		JLabel label_2 = new JLabel("BTN 2");
		label_2.setForeground(Color.BLUE);
		label_2.setFont(new Font("Tahoma", Font.PLAIN, 12));
		label_2.setBounds(111, 39, 36, 14);
		panel.add(label_2);
		
		JLabel label_3 = new JLabel("BTN_OPT");
		label_3.setForeground(Color.BLUE);
		label_3.setFont(new Font("Tahoma", Font.PLAIN, 12));
		label_3.setBounds(168, 39, 56, 14);
		panel.add(label_3);
		
		JPanel panel_1 = new JPanel();
		panel_1.setBorder(new LineBorder(new Color(0, 0, 255), 2));
		panel_1.setBounds(302, 11, 202, 123);
		add(panel_1);
		panel_1.setLayout(null);
		
		lightBulb1 = new LightBulb();
		lightBulb1.setOn(true);
		lightBulb1.setGlowColor(Color.RED);
		lightBulb1.setBounds(10, 11, 78, 78);
		panel_1.add(lightBulb1);
		lightBulb1.addMouseListener(ma_lightBulb);
		
		JLabel lblNewLabel = new JLabel("REL\u00C9");
		lblNewLabel.setHorizontalAlignment(SwingConstants.CENTER);
		lblNewLabel.setFont(new Font("Times New Roman", Font.PLAIN, 20));
		lblNewLabel.setBounds(15, 92, 68, 24);
		panel_1.add(lblNewLabel);
		
		lightBulb2 = new LightBulb();
		lightBulb2.setOn(true);
		lightBulb2.setGlowColor(Color.YELLOW);
		lightBulb2.setBounds(114, 11, 78, 78);
		panel_1.add(lightBulb2);
		lightBulb2.addMouseListener(ma_lightBulb);
		
		JLabel lblRel = new JLabel("TRIAC");
		lblRel.setHorizontalAlignment(SwingConstants.CENTER);
		lblRel.setFont(new Font("Times New Roman", Font.PLAIN, 20));
		lblRel.setBounds(122, 92, 63, 24);
		panel_1.add(lblRel);
		
		JPanel panel_1_1 = new JPanel();
		panel_1_1.setLayout(null);
		panel_1_1.setBorder(new BevelBorder(BevelBorder.LOWERED, new Color(0, 0, 255), new Color(0, 255, 0), Color.BLUE, Color.MAGENTA));
		panel_1_1.setBounds(10, 145, 248, 100);
		add(panel_1_1);
		
		ledPIR = new Led();
		ledPIR.setBounds(0, 2, 95, 95);
		panel_1_1.add(ledPIR);
		
		JLabel label_4 = new JLabel("PIR");
		label_4.setForeground(Color.RED);
		label_4.setHorizontalAlignment(SwingConstants.CENTER);
		label_4.setFont(new Font("Times New Roman", Font.PLAIN, 20));
		label_4.setBounds(0, 73, 248, 24);
		panel_1_1.add(label_4);
		
		cbActPIR = new JCheckBox("Activar PIR");
		cbActPIR.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent arg0) {
				
				TSwitchState vSel;
				
				//AbstractButton aButton = ((AbstractButton) arg0.getSource()).getModel().isSelected();
		        
				
				if(((AbstractButton) arg0.getSource()).getModel().isSelected()) vSel = TSwitchState.ON;
				else vSel = TSwitchState.OFF;
				
				ModBus_Communications.writeCoil(Integer.parseInt(address), MB_Registers.MB_Discrete_Output_Coils.MB_ACTPIR.getReg(),
						vSel, sn_Transport);
			}
		});
		cbActPIR.setSelected(true);
		cbActPIR.setBounds(101, 12, 87, 23);
		panel_1_1.add(cbActPIR);
		
		tiempoPIR = new JTextField();
		tiempoPIR.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				
				ModBus_Communications.writeSingleRegister(address, MB_Registers.MB_Analog_Output_Holding.MB_TMP_PIR.getReg(),
						tiempoPIR.getText(), sn_Transport);		
			}
		});
		tiempoPIR.setColumns(10);
		tiempoPIR.setBounds(105, 42, 37, 20);
		panel_1_1.add(tiempoPIR);
		
		JLabel label_5 = new JLabel("Tiempo (Segs.)");
		label_5.setBounds(152, 45, 85, 14);
		panel_1_1.add(label_5);
		
		if(sn_Transport.isConnected())
			leerConfiguracionInicial();
	}
	
	private void leerConfiguracionInicial(){
		//******************************************
		//Leer elementos de configuración Analógico
		//******************************************
		
		String 	vSel 	= "1"; //Leemos un registro, tiempo PIR  
		String 	vReg 	= String.valueOf(MB_Registers.MB_Analog_Output_Holding.MB_TMP_PIR.getReg());
						
		//int		Aregs[] = new int [MB_Registers.MB_Analog_Output_Holding.MB_AREGS.getReg()];
						
		String[] args = {address, String.valueOf(Const_Modbus.READ_MULTIPLE_REGISTERS), vReg, vSel};
		Modbus.InitComunication(args, sn_Transport, Aregs);
						
		ModBusEvent e = new ModBusEvent(Aregs);
		e.set_Args(args);
		UpdateElements(e);

		
		//******************************************
		//Leer elementos de configuración Digitales
		//******************************************
		
		//vReg = String.valueOf(MB_Registers.MB_Discrete_Output_Coils.MB_ACTPIR.getReg());
		//String nCoils = "1"; // Leemos solo un rgistro
					
		//String[] iargs = {address, String.valueOf(Const_Modbus.READ_COILS), vReg, nCoils};
		args[1] = String.valueOf(Const_Modbus.READ_COILS);
		args[2] = String.valueOf(MB_Registers.MB_Discrete_Output_Coils.MB_ACTPIR.getReg());
		args[3] = "1"; // Leemos solo un rgistro
		Modbus.InitComunication(args, sn_Transport, Cregs);
					
		e.setRegs(Cregs);
		e.setDigital(true);
		e.set_Args(args);
					
		UpdateElements(e);
		
	}
	
	private void ONOFF_Bulb(LightBulb lightBulb){
		
		String vBulb;
		String vReg;
		
		lightBulb.setOn(!lightBulb.isOn());
		
		if(lightBulb.isOn()) vBulb = TSwitchState.ToNumberString(TSwitchState.ON);
		else vBulb = TSwitchState.ToNumberString(TSwitchState.OFF);
		
		if(lightBulb == lightBulb1) vReg = String.valueOf(MB_Registers.MB_Discrete_Output_Coils.MB_RELE.getReg());
		else vReg = String.valueOf(MB_Registers.MB_Discrete_Output_Coils.MB_TRIAC.getReg());
		
		String[] args = {address, String.valueOf(Const_Modbus.WRITE_COIL), vReg, vBulb};
		Modbus.InitComunication(args, sn_Transport, Cregs);
	}

	@Override
	public String getCategory() {
		return category;
	}

	@Override
	public String getTitle() {
		return "Address : "+address;
	}

	@Override
	public Component getPanel() {
		return this;
	}

	@Override
	public boolean isCategory() {
		return isCategory;
	}

	@Override
	public void Actualize() {
		
		// Actualizar dispositivos modbus
		//String nCoils;
		//String vReg;
		
		ModBusEvent e = new ModBusEvent(Cregs);
		
		if(stActualize) {	
			
			//Read OutputCoils
			//vReg = String.valueOf(MB_Registers.MB_Discrete_Output_Coils.MB_RELE);
			//nCoils = String.valueOf(MB_Registers.MB_Discrete_Output_Coils.MB_O_COILS);
		
			String[] args = {address, String.valueOf(Const_Modbus.READ_COILS), 
							String.valueOf(MB_Registers.MB_Discrete_Output_Coils.MB_RELE.getReg()), 
							String.valueOf(MB_Registers.MB_Discrete_Output_Coils.MB_O_COILS.getReg())};
			
			//send ModBus request and wait response
			Modbus.InitComunication(args, sn_Transport, Cregs);
			
			e.setDigital(true);
			e.set_Args(args);
	
			UpdateElements(e);
		
			//Read Digital Input Register
			//vReg = String.valueOf(MB_Registers.MB_Discrete_Input_Contacts.MB_BTN1.getReg());
			//nCoils = String.valueOf(MB_Registers.MB_Discrete_Input_Contacts.MB_I_REGS.getReg());
	
			//String[] iargs = {address, String.valueOf(Const_Modbus.READ_INPUT_DISCRETES), vReg, nCoils};
			args[1] = String.valueOf(Const_Modbus.READ_INPUT_DISCRETES);
			args[2] = String.valueOf(MB_Registers.MB_Discrete_Input_Contacts.MB_BTN1.getReg());
			args[3] = String.valueOf(MB_Registers.MB_Discrete_Input_Contacts.MB_I_REGS.getReg());
			Modbus.InitComunication(args, sn_Transport, Dregs);
	
			e.set_Args(args);
			e.setRegs(Dregs);
			UpdateElements(e);

		}
	}
	
	public void UpdateElements(final ModBusEvent e){

		int addr = Integer.parseInt(e.get_Args()[2]);
		int nReg = Integer.parseInt(e.get_Args()[3]);
					
		switch(Integer.parseInt(e.get_Args()[1])){
		
		case Const_Modbus.READ_MULTIPLE_REGISTERS:
			//Cada registro esta compruesto de dos bytes "little endian"
			for(int i = addr;i<(addr+nReg); i++){
				switch(MB_Analog_Output_Holding.values()[i]){
				case MB_TMP_PIR:
					//tiempoPIR.setText(Integer.toString(((e.getRegs()[i+1]&0xFF)<<8)|((e.getRegs()[i]&0xFF))));
					tiempoPIR.setText(Integer.toString(e.getRegs()[i-addr]));
					break;
				default:
					break;
				}
			}
			break;
		
		case Const_Modbus.READ_COILS:
			
			//MB_Discrete_Output_Coils mbDOC =  MB_Discrete_Output_Coils.values()[addr];//new MB_Discrete_Output_Coils(addr);
			
			for(int i = addr;i<(addr+nReg); i++){
				switch(MB_Discrete_Output_Coils.values()[i]){
				case MB_RELE:							
					lightBulb1.setOn((e.getRegs()[i] == 1));
					break;
							
				case MB_TRIAC:							
					lightBulb2.setOn((e.getRegs()[i] == 1));
					break;
					
				case MB_ACTPIR:
					cbActPIR.setSelected((e.getRegs()[i] == 1));
					break;
					
				default:
					break;
				
				}
				
				//mbDOC.setReg(addr++);
			}
			break;
						
		case Const_Modbus.READ_INPUT_DISCRETES:	
			
			MB_Discrete_Input_Contacts mbDIC; //= MB_Discrete_Input_Contacts.values()[addr];
			
			for(int i = addr;i<(addr+nReg); i++){
				mbDIC = MB_Discrete_Input_Contacts.values()[i];
				switch(mbDIC){
				case MB_BTN1:
					ledBtn1.setLedOn((e.getRegs()[i] != mbDIC.getDefaultValue()));
					break;
								
				case MB_BTN2:
					ledBtn2.setLedOn((e.getRegs()[i] != mbDIC.getDefaultValue()));
					break;
							
				case MB_OPT:
					ledBtnOpt.setLedOn((e.getRegs()[i] != mbDIC.getDefaultValue()));
					break;
					
				case MB_PIRMOV:
					ledPIR.setLedOn((e.getRegs()[i] == 1));
					break;
					
				default:
					break;
				}
			}
			break;
		}				
	}
	
	@Override
	public void setActualize(boolean st) {
		stActualize = st;		
	}

	@Override
	public boolean getActualize() {
		
		return stActualize;
	}

	@Override
	public void vlog(String message) {
		// TODO Auto-generated method stub
		
	}
}
