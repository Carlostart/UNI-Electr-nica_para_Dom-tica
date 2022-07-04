package gui.Visualizers;

import java.awt.Component;

import javax.swing.JPanel;

import Base_COM_Serie.MB_Registers;
import Base_COM_Serie.ModBus_Communications;
import CommTransport.CommTransport;
import javax.swing.JButton;

import java.awt.event.MouseAdapter;
import java.awt.event.MouseEvent;
import java.awt.event.ActionListener;
import java.awt.event.ActionEvent;

@SuppressWarnings("serial")
public class AppTest extends JPanel implements Visualizer {
	private final 	String 					category;
	private final 	boolean					isCategory = false;
	@SuppressWarnings("unused")
	private final	CommTransport 			sn_Transport;
	
	public AppTest(String category, CommTransport sn_Transport) {
		
		super();
		
		this.category = category;
		this.sn_Transport = sn_Transport;
		
		setLayout(null);
		
		JButton btnNewButton = new JButton("ON RELÉ");
		btnNewButton.addMouseListener(new MouseAdapter() {
			@Override
			public void mouseClicked(MouseEvent e) {
				ModBus_Communications.writeCoil(1, MB_Registers.MB_Discrete_Output_Coils.MB_RELE.getReg(), 
						MB_Registers.TSwitchState.ON , sn_Transport);   
			}
		});
		btnNewButton.setBounds(83, 112, 144, 23);
		add(btnNewButton);
		
		JButton btnOnTriac = new JButton("ON TRIAC");
		btnOnTriac.addMouseListener(new MouseAdapter() {
			@Override
			public void mouseClicked(MouseEvent e) {
				ModBus_Communications.writeCoil(1, MB_Registers.MB_Discrete_Output_Coils.MB_TRIAC.getReg(), 
						MB_Registers.TSwitchState.ON , sn_Transport);   
			}
		});
		btnOnTriac.setBounds(260, 112, 144, 23);
		add(btnOnTriac);
		
		JButton btnOffTriac = new JButton("OFF TRIAC");
		btnOffTriac.addMouseListener(new MouseAdapter() {
			@Override
			public void mouseClicked(MouseEvent e) {
				ModBus_Communications.writeCoil(1, MB_Registers.MB_Discrete_Output_Coils.MB_TRIAC.getReg(), 
						MB_Registers.TSwitchState.OFF , sn_Transport);   
			}
		});
		btnOffTriac.setBounds(260, 148, 144, 23);
		add(btnOffTriac);
		
		JButton btnOffRel = new JButton("OFF RELÉ");
		btnOffRel.addMouseListener(new MouseAdapter() {
			@Override
			public void mouseClicked(MouseEvent e) {
				ModBus_Communications.writeCoil(1, MB_Registers.MB_Discrete_Output_Coils.MB_RELE.getReg(), 
						MB_Registers.TSwitchState.OFF , sn_Transport);   
			}
		});
		btnOffRel.setBounds(83, 148, 144, 23);
		add(btnOffRel);
		
		JButton btnReadAohr = new JButton("Read A_O_H_R");
		btnReadAohr.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				
				ModBus_Communications.readMultipleRegisters(1, MB_Registers.MB_Analog_Output_Holding.MB_PRACT.getReg(), 
						MB_Registers.MB_Analog_Output_Holding.MB_AREGS.getReg(), sn_Transport);
			}
		});
		btnReadAohr.setBounds(83, 182, 144, 23);
		add(btnReadAohr);
	}

	@Override
	public String getCategory() {
		return category;
	}

	@Override
	public String getTitle() {
		return "** Test **";
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
		// TODO Auto-generated method stub
		
	}

	@Override
	public void setActualize(boolean st) {
		// TODO Auto-generated method stub
		
	}

	@Override
	public boolean getActualize() {
		// TODO Auto-generated method stub
		return false;
	}

	@Override
	public void vlog(String message) {
		// TODO Auto-generated method stub
		
	}
}
