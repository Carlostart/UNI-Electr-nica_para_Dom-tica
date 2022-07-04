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
import eu.hansolo.steelseries.gauges.Radial2Top;
import eu.hansolo.steelseries.tools.GaugeType;
import eu.hansolo.steelseries.tools.LcdColor;
import eu.hansolo.steelseries.tools.LedColor;
import eu.hansolo.steelseries.tools.PointerType;
import eu.hansolo.steelseries.gauges.DigitalRadial;
import eu.hansolo.steelseries.gauges.DisplaySingle;
import javax.swing.plaf.basic.BasicArrowButton;
import javax.swing.JButton;
import javax.swing.ImageIcon;

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
	private         int					Iregs[];
	
	private			boolean     		stActualize = true;
	private 		JTextField 			tiempoPIR;
	private 		JTextField 			tf_HL_SRC;
	private 		JTextField 			tf_LL_SRC;
	private			JCheckBox 			cbActPIR;
	private 		JCheckBox 			cbActSRC;
	private			JCheckBox 			cbActTTOR;
	private 		Led 				ledPIR;
	private 		Radial2Top 			r2T_Pot1;
	private 		Radial2Top 			r2T_Pot2;
	private			DigitalRadial 		dRSRC;
	private			DisplaySingle 		dSTemp;
	private			DigitalRadial 		dRTtor;
	private 		JTextField 			tf_LL_ttor;
	private			JLabel 				lbStadoPersiana;
	private 		JLabel 				label_PosPer;

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
		Iregs = new int [MB_Registers.MB_Analog_Input_Register.MB_I_AREGS.getReg()];
		
		ma_lightBulb = new MouseAdapter() {
			@Override
			public void mouseClicked(MouseEvent e) {
				ONOFF_Bulb(((LightBulb)e.getComponent()));
			}
		};
		
		JPanel panel = new JPanel();
		panel.setLayout(null);
		panel.setBorder(new BevelBorder(BevelBorder.LOWERED, new Color(0, 0, 255), new Color(0, 255, 0), Color.BLUE, Color.MAGENTA));
		panel.setBounds(10, 10, 248, 100);
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
		panel_1.setBounds(578, 10, 202, 123);
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
		
		r2T_Pot1 = new Radial2Top();
		r2T_Pot1.setUserLedVisible(true);
		r2T_Pot1.setUnitString("%");
		r2T_Pot1.setTrackVisible(true);
		r2T_Pot1.setTitle("Pot. 1");
		r2T_Pot1.setPointerType(PointerType.TYPE5);
		r2T_Pot1.setLedColor(LedColor.GREEN);
		r2T_Pot1.setLcdUnitStringVisible(true);
		r2T_Pot1.setLcdScientificFormat(true);
		r2T_Pot1.setLcdInfoString("Test");
		r2T_Pot1.setLcdColor(LcdColor.BLUE_LCD);
		r2T_Pot1.setLcdBackgroundVisible(false);
		r2T_Pot1.setGaugeType(GaugeType.TYPE5);
		r2T_Pot1.setBounds(10, 280, 193, 193);
		add(r2T_Pot1);
		
		r2T_Pot2 = new Radial2Top();
		r2T_Pot2.setUserLedVisible(true);
		r2T_Pot2.setUnitString("%");
		r2T_Pot2.setTrackVisible(true);
		r2T_Pot2.setTitle("Pot. 2");
		r2T_Pot2.setPointerType(PointerType.TYPE5);
		r2T_Pot2.setLedColor(LedColor.GREEN);
		r2T_Pot2.setLcdUnitStringVisible(true);
		r2T_Pot2.setLcdScientificFormat(true);
		r2T_Pot2.setLcdInfoString("Test");
		r2T_Pot2.setLcdColor(LcdColor.BLUE_LCD);
		r2T_Pot2.setLcdBackgroundVisible(false);
		r2T_Pot2.setGaugeType(GaugeType.TYPE5);
		r2T_Pot2.setBounds(223, 280, 193, 193);
		add(r2T_Pot2);
		
		JPanel panel_2 = new JPanel();
		panel_2.setLayout(null);
		panel_2.setBorder(new BevelBorder(BevelBorder.LOWERED, new Color(0, 0, 255), new Color(0, 255, 0), Color.BLUE, Color.MAGENTA));
		panel_2.setBounds(293, 10, 254, 124);
		add(panel_2);
		
		JLabel label_6 = new JLabel("Photo Resistencia");
		label_6.setHorizontalAlignment(SwingConstants.CENTER);
		label_6.setFont(new Font("Times New Roman", Font.PLAIN, 20));
		label_6.setBounds(0, 94, 218, 24);
		panel_2.add(label_6);
		
		cbActSRC = new JCheckBox("Activar SRC");
		cbActSRC.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent arg0) {
				
				TSwitchState vSel;
				
				//AbstractButton aButton = ((AbstractButton) arg0.getSource()).getModel().isSelected();
		        
				
				if(((AbstractButton) arg0.getSource()).getModel().isSelected()) vSel = TSwitchState.ON;
				else vSel = TSwitchState.OFF;
				
				ModBus_Communications.writeCoil(Integer.parseInt(address), MB_Registers.MB_Discrete_Output_Coils.MB_ACTSRC.getReg(),
						vSel, sn_Transport);
			}
		});
		cbActSRC.setSelected(true);
		cbActSRC.setBounds(111, 7, 101, 23);
		panel_2.add(cbActSRC);
		
		tf_HL_SRC = new JTextField();
		tf_HL_SRC.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				String vSel = tf_HL_SRC.getText();
				String vReg = String.valueOf(MB_Registers.MB_Analog_Output_Holding.MB_SRC_HL.getReg());
									
				String[] args = {address, String.valueOf(Const_Modbus.WRITE_SINGLE_REGISTER), vReg, vSel};
				Modbus.InitComunication(args, sn_Transport, Aregs);
					
				//TODO: Después de escribir, deberíamos leer para comprobar que se ha escrito de forma correcta
			}
		});
		tf_HL_SRC.setBounds(112, 37, 37, 20);
		panel_2.add(tf_HL_SRC);
		
		JLabel label_7 = new JLabel("High Level");
		label_7.setBounds(159, 40, 72, 14);
		panel_2.add(label_7);
		
		dRSRC = new DigitalRadial();
		dRSRC.setBounds(10, 7, 86, 86);
		dRSRC.setValue(500);
		dRSRC.setMaxValue(1023.0);
		panel_2.add(dRSRC);
		
		tf_LL_SRC = new JTextField();
		tf_LL_SRC.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				String vSel = tf_LL_SRC.getText();
				String vReg = String.valueOf(MB_Registers.MB_Analog_Output_Holding.MB_SRC_LL.getReg());
									
				String[] args = {address, String.valueOf(Const_Modbus.WRITE_SINGLE_REGISTER), vReg, vSel};
				Modbus.InitComunication(args, sn_Transport, Aregs);
					
				//TODO: Después de escribir, deberíamos leer para comprobar que se ha escrito de forma correcta
			}
		});
		tf_LL_SRC.setColumns(10);
		tf_LL_SRC.setBounds(111, 68, 37, 20);
		panel_2.add(tf_LL_SRC);
		
		JLabel label_8 = new JLabel("Low Level");
		label_8.setBounds(158, 71, 72, 14);
		panel_2.add(label_8);
		
		JPanel panel_3 = new JPanel();
		panel_3.setLayout(null);
		panel_3.setBorder(new BevelBorder(BevelBorder.LOWERED, new Color(0, 0, 255), new Color(0, 255, 0), Color.BLUE, Color.MAGENTA));
		panel_3.setBounds(293, 145, 254, 124);
		add(panel_3);
		
		JLabel lblPhotoTransistor = new JLabel("Photo Transistor");
		lblPhotoTransistor.setHorizontalAlignment(SwingConstants.CENTER);
		lblPhotoTransistor.setFont(new Font("Times New Roman", Font.PLAIN, 20));
		lblPhotoTransistor.setBounds(0, 98, 237, 24);
		panel_3.add(lblPhotoTransistor);
		
		dRTtor = new DigitalRadial();
		dRTtor.setValue(500.0);
		dRTtor.setMaxValue(1000.0);
		dRTtor.setBounds(10, 6, 86, 86);
		panel_3.add(dRTtor);
		
		cbActTTOR = new JCheckBox("Activar Detecci\u00F3n");
		cbActTTOR.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent arg0) {
				
				TSwitchState vSel;
				
				//AbstractButton aButton = ((AbstractButton) arg0.getSource()).getModel().isSelected();
		        
				
				if(((AbstractButton) arg0.getSource()).getModel().isSelected()) vSel = TSwitchState.ON;
				else vSel = TSwitchState.OFF;
				
				ModBus_Communications.writeCoil(Integer.parseInt(address), MB_Registers.MB_Discrete_Output_Coils.MB_ACTTOR.getReg(),
						vSel, sn_Transport);
			}
		});
		cbActTTOR.setSelected(true);
		cbActTTOR.setBounds(109, 6, 109, 23);
		panel_3.add(cbActTTOR);
		
		tf_LL_ttor = new JTextField();
		tf_LL_ttor.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				String vSel = tf_LL_ttor.getText();
				String vReg = String.valueOf(MB_Registers.MB_Analog_Output_Holding.MB_TTOR_LL.getReg());
									
				String[] args = {address, String.valueOf(Const_Modbus.WRITE_SINGLE_REGISTER), vReg, vSel};
				Modbus.InitComunication(args, sn_Transport, Aregs);
					
				//TODO: Después de escribir, deberíamos leer para comprobar que se ha escrito de forma correcta
			}
		});
		tf_LL_ttor.setColumns(10);
		tf_LL_ttor.setBounds(106, 46, 37, 20);
		panel_3.add(tf_LL_ttor);
		
		JLabel label_8_1 = new JLabel("Nivel Detecci\u00F3n");
		label_8_1.setBounds(151, 49, 93, 14);
		panel_3.add(label_8_1);
		
		dSTemp = new DisplaySingle();
		dSTemp.setLcdValueFont(new Font("Verdana", Font.PLAIN, 9));
		dSTemp.setLcdValue(24);
		dSTemp.setLcdUnitString("\u00BAC");
		dSTemp.setLcdUnitFont(new Font("Verdana", Font.PLAIN, 8));
		dSTemp.setLcdInfoString("Temperatura");
		dSTemp.setLcdInfoFont(new Font("Verdana", Font.PLAIN, 12));
		dSTemp.setFont(new Font("Courier New", Font.PLAIN, 12));
		dSTemp.setCustomLcdUnitFontEnabled(true);
		dSTemp.setCustomLcdUnitFont(new Font("Verdana", Font.PLAIN, 16));
		dSTemp.setBounds(578, 144, 150, 88);
		dSTemp.setLcdDecimals(0);
		//dSTemp.formatLcdValue(serialVersionUID)
		add(dSTemp);
		
		JPanel panel_4 = new JPanel();
		panel_4.setLayout(null);
		panel_4.setBorder(new BevelBorder(BevelBorder.LOWERED, new Color(0, 0, 255), new Color(0, 255, 0), Color.BLUE, Color.MAGENTA));
		panel_4.setBounds(460, 280, 338, 271);
		add(panel_4);
		
		BasicArrowButton perUP = new BasicArrowButton(1);
		perUP.addMouseListener(new MouseAdapter() {
			@Override
			public void mousePressed(MouseEvent e) {
				String vSel;
				String vReg = String.valueOf(MB_Registers.MB_Discrete_Output_Coils.MB_PERUP.getReg());
			
				vSel = "1";

				String[] args = {address, String.valueOf(Const_Modbus.WRITE_COIL), vReg, vSel};
				Modbus.InitComunication(args, sn_Transport, Cregs);
			}
			@Override
			public void mouseReleased(MouseEvent e) {
				String vSel;
				String vReg = String.valueOf(MB_Registers.MB_Discrete_Output_Coils.MB_PERUP.getReg());
			
				vSel = "0";

				String[] args = {address, String.valueOf(Const_Modbus.WRITE_COIL), vReg, vSel};
				Modbus.InitComunication(args, sn_Transport, Cregs);
			}
		});
		perUP.setBounds(44, 50, 57, 66);
		panel_4.add(perUP);
		
		BasicArrowButton perDOWN = new BasicArrowButton(5);
		perDOWN.addMouseListener(new MouseAdapter() {
			@Override
			public void mousePressed(MouseEvent e) {
				String vSel;
				String vReg = String.valueOf(MB_Registers.MB_Discrete_Output_Coils.MB_PERDOWN.getReg());
			
				vSel = "1";

				String[] args = {address, String.valueOf(Const_Modbus.WRITE_COIL), vReg, vSel};
				Modbus.InitComunication(args, sn_Transport, Cregs);
			}
			@Override
			public void mouseReleased(MouseEvent e) {
				String vSel;
				String vReg = String.valueOf(MB_Registers.MB_Discrete_Output_Coils.MB_PERDOWN.getReg());
			
				vSel = "0";

				String[] args = {address, String.valueOf(Const_Modbus.WRITE_COIL), vReg, vSel};
				Modbus.InitComunication(args, sn_Transport, Cregs);
			}
		});
		perDOWN.setBounds(44, 117, 57, 66);
		panel_4.add(perDOWN);
		
		label_PosPer = new JLabel("100 %");
		label_PosPer.setHorizontalAlignment(SwingConstants.CENTER);
		label_PosPer.setForeground(Color.BLUE);
		label_PosPer.setFont(new Font("Verdana", Font.BOLD, 30));
		label_PosPer.setBounds(0, 197, 338, 39);
		panel_4.add(label_PosPer);
		
		JButton btnNewButton = new JButton("");
		btnNewButton.addMouseListener(new MouseAdapter() {
			@Override
			public void mousePressed(MouseEvent e) {
				String vSel;
				String vReg = String.valueOf(MB_Registers.MB_Discrete_Output_Coils.MB_KEYGAR.getReg());
			
				vSel = "1";

				String[] args = {address, String.valueOf(Const_Modbus.WRITE_COIL), vReg, vSel};
				Modbus.InitComunication(args, sn_Transport, Cregs);
			}
		});
		btnNewButton.setIcon(new ImageIcon("D:\\Trabajo\\Eclipse\\Workspaces\\Java\\Practicas_Electronica_para_Domotica\\P10_ControlPersianas\\key.png"));
		btnNewButton.setBounds(167, 89, 130, 50);
		panel_4.add(btnNewButton);
		
		JLabel lblPuertaGaraje = new JLabel("Puerta Garaje");
		lblPuertaGaraje.setHorizontalAlignment(SwingConstants.CENTER);
		lblPuertaGaraje.setFont(new Font("SansSerif", Font.BOLD, 20));
		lblPuertaGaraje.setBounds(183, 247, 145, 24);
		panel_4.add(lblPuertaGaraje);
		
		JLabel label_11_2_2 = new JLabel("CTRL. Persiana");
		label_11_2_2.setHorizontalAlignment(SwingConstants.CENTER);
		label_11_2_2.setFont(new Font("SansSerif", Font.BOLD, 20));
		label_11_2_2.setBounds(0, 247, 173, 24);
		panel_4.add(label_11_2_2);
		
		lbStadoPersiana = new JLabel("Parada");
		lbStadoPersiana.setHorizontalAlignment(SwingConstants.CENTER);
		lbStadoPersiana.setForeground(Color.RED);
		lbStadoPersiana.setFont(new Font("Verdana", Font.BOLD | Font.ITALIC, 25));
		lbStadoPersiana.setBounds(0, 0, 334, 39);
		panel_4.add(lbStadoPersiana);
		
		if(sn_Transport.isConnected())
			leerConfiguracionInicial();
	}
	
	private void leerConfiguracionInicial(){
		//******************************************
		//Leer elementos de configuración Analógico
		//******************************************
		
		String 	vSel 	= "4"; //Leemos 4 registro2, tiempo PIR, y niveles  
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
		args[3] = "3"; // Leemos solo tres registros
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
					
			String[] args = {address, String.valueOf(Const_Modbus.READ_COILS), 
							String.valueOf(MB_Registers.MB_Discrete_Output_Coils.MB_RELE.getReg()), 
							String.valueOf(MB_Registers.MB_Discrete_Output_Coils.MB_O_COILS.getReg())};
			
			//send ModBus request and wait response
			Modbus.InitComunication(args, sn_Transport, Cregs);
			
			e.setDigital(true);
			e.set_Args(args);
	
			UpdateElements(e);
		
			//Read Digital Input Register
			
			//String[] iargs = {address, String.valueOf(Const_Modbus.READ_INPUT_DISCRETES), vReg, nCoils};
			args[1] = String.valueOf(Const_Modbus.READ_INPUT_DISCRETES);
			args[2] = String.valueOf(MB_Registers.MB_Discrete_Input_Contacts.MB_BTN1.getReg());
			args[3] = String.valueOf(MB_Registers.MB_Discrete_Input_Contacts.MB_I_REGS.getReg());
			Modbus.InitComunication(args, sn_Transport, Dregs);
	
			e.set_Args(args);
			e.setRegs(Dregs);
			UpdateElements(e);
			
			//Lectura de registros analógicas de entrada
			args[1] = String.valueOf(Const_Modbus.READ_INPUT_REGISTERS);
			args[2] = String.valueOf(MB_Registers.MB_Analog_Input_Register.MB_POT1.getReg());
			args[3] = String.valueOf(MB_Registers.MB_Analog_Input_Register.MB_I_AREGS.getReg());
			Modbus.InitComunication(args, sn_Transport, Iregs);
			
			if(Iregs != null) {
				e.set_Args(args);
				e.setRegs(Iregs);
				UpdateElements(e);
			}
			
			//Lectura de registros analógicas de salida (Holding Registers)
			args[1] = String.valueOf(Const_Modbus.READ_MULTIPLE_REGISTERS);
			args[2] = String.valueOf(MB_Registers.MB_Analog_Output_Holding.MB_STATEPER.getReg());
			args[3] = "1";
			
			//aArgs = new String[] {address, String.valueOf(Const_Modbus.READ_MULTIPLE_REGISTERS), vReg, nRegs};
			Modbus.InitComunication(args, sn_Transport, Aregs);
			
			e.set_Args(args);
			e.setRegs(Aregs);
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
				case MB_SRC_HL:					
					tf_HL_SRC.setText(Integer.toString(e.getRegs()[i-addr]));
					break;
					
				case MB_SRC_LL:
					//int test = ((e.getRegs()[i+1]&0xFF)<<8)|((e.getRegs()[i]&0xFF));
					tf_LL_SRC.setText(Integer.toString(e.getRegs()[i-addr]));
					break;
					
				case MB_TTOR_LL:
					tf_LL_ttor.setText(Integer.toString(e.getRegs()[i-addr]));
					break;
					
				case MB_STATEPER:
					try{
						MB_Registers.Ctrl_Persianas ctrlPer= MB_Registers.Ctrl_Persianas.fromInteger(e.getRegs()[i-addr]);
						//Ctrl_Persianas ctrlPersianas = Ctrl_Persianas.fromInteger(e.getRegs()[i]);
					
						switch((ctrlPer)){
						case PER_DOWN:
							lbStadoPersiana.setText("Bajando");
							break;
					
						case PER_STOP:
							lbStadoPersiana.setText("Parada");
							break;
					
						case PER_UP:
							lbStadoPersiana.setText("Subiendo");
							break;
					
						default:
							break;
					
						}
					} catch (Exception ex) {
						System.err.println("Error Estado persiana");
			        }
					
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
					
				case MB_ACTSRC:
					cbActSRC.setSelected((e.getRegs()[i] == 1));
					break;
					
				case MB_ACTTOR:
					cbActTTOR.setSelected((e.getRegs()[i] == 1));
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
			
		case Const_Modbus.READ_INPUT_REGISTERS:
			
			MB_Analog_Input_Register mbAIR; //= MB_Discrete_Input_Contacts.values()[addr];
			
			for(int i = addr;i<(addr+nReg); i++){
				mbAIR = MB_Analog_Input_Register.values()[i];
				switch(mbAIR){
				case MB_POT1:
					r2T_Pot1.setValue(100 - ((e.getRegs()[i]*100)/1024));
					break;
					
				case MB_POT2:
					r2T_Pot2.setValue((e.getRegs()[i]*100)/1024);
					break;
					
				case MB_PHOTORES:							
					dRSRC.setValue((e.getRegs()[i]));
					break;	
					
				case MB_TEMPSEN:							
					//dRSRC.setValue((e.getRegs()[i]));
					String Temp = ((e.getRegs()[i] >> 8)&0xff)+"."+(e.getRegs()[i]&0xff);
					//System.out.println("Temperatura : "+Temp);
					dSTemp.setLcdValue(Double.parseDouble(Temp));
					break;	
					
				case MB_PHOTOTTOR:							
					dRTtor.setValue((e.getRegs()[i]));
					break;	
					
				case MB_POSPER:
					label_PosPer.setText((e.getRegs()[i]) + " %");
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
