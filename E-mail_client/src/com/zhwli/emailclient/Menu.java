package com.zhwli.emailclient;

import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.OutputStreamWriter;
import java.io.PrintWriter;
import java.net.Socket;
import java.net.UnknownHostException;

import javax.swing.*;

import sun.misc.BASE64Encoder;

public class Menu extends JFrame implements ActionListener,Runnable{
	Thread OK;
	//��-�ӵ������ʼ����桢�����ʼ�����
	SendEmailView sendView=new SendEmailView(this);
	ReceiveEmailView receiveView=new ReceiveEmailView(this,sendView);
	//��-�ӵ������ʼ�����
//	SendEmail sendEmail=new SendEmail(sendView);
	
	
	PrintWriter output;//��֤-socket���
	BufferedReader input;//����
	Socket socket = null;
	
	//row1Ŀ¼������
	JPanel row1=new JPanel();
	JButton send=new JButton("Send E-mail");
	JButton receive=new JButton("Receive E-mail");
	//row2�������������
	JPanel row2=new JPanel();
	JLabel mailServer=new JLabel("MailServer:");
	JComboBox server=new JComboBox();
	JLabel smtpPort=new JLabel("Smtp Port:");
	JTextField smtp=new JTextField("25",5);
	JLabel pop3Port=new JLabel("Pop3 Port:");
	JTextField pop3=new JTextField("110",5);
	JButton OK1=new JButton("ServerOK");
	//row3�����û���������
	JPanel row3=new JPanel();
	JLabel userName=new JLabel("Username:");
	JTextField user=new JTextField("appiaaai@163.com",35);	
	JLabel password=new JLabel("Password:");
	JPasswordField psw=new JPasswordField("����������",35);
	JButton OK2=new JButton("UserOK");
	JButton clear=new JButton("Clear");
	
		
	public Menu(){
		super("E-mail");
		setLookAndFeel();
		setSize(500,600);
		setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		
		//���岼��
		GridLayout mainLayout=new GridLayout(5,1,10,0);
		setLayout(mainLayout);
		
		//��send�� receive����Ӽ�����ת�������ʼ�ҳ��
		send.addActionListener(sendView);
		receive.addActionListener(receiveView);
		//��OK1��OK2��clear��Ӽ���
		OK1.addActionListener(this);
		OK2.addActionListener(this);
		clear.addActionListener(this);
		
		//row1�շ��ʼ���ť
		FlowLayout layout1=new FlowLayout(FlowLayout.CENTER,10,10);
		row1.setLayout(layout1);
		row1.add(send);
		row1.add(receive);
		add(row1);
		
		//row2ѡ���ʼ�������
		FlowLayout layout2=new FlowLayout();
		row2.setLayout(layout2);
		row2.add(mailServer);
		server.setEditable(true);
		server.addItem("163.com");
		server.addItem("hust.edu.cn");
		server.addItem("gmail.com");
		server.addItem("sina.com.cn");
		row2.add(server);
		row2.add(smtpPort);
		row2.add(smtp);
		row2.add(pop3Port);
		row2.add(pop3);
		row2.add(OK1);
		add(row2);
		
		//row3���������˺š�����
		FlowLayout laypout3=new FlowLayout();
		row3.setLayout(laypout3);
		row3.add(userName);
		row3.add(user);
		row3.add(password);

		row3.add(psw);
		row3.add(OK2);
		row3.add(clear);
		add(row3);
		
//		System.out.println(user.getText());
		
		setVisible(true);
	}
	
	//ȷ���߳�
	public void actionPerformed(ActionEvent event){
		String command=event.getActionCommand();
		if(command.equals("ServerOK")){
			serverOK();
		}
		if(command.equals("UserOK")){
			userOK();
		}
		if(command.equals("Clear")){
			clear();
		}
	}
	
	void serverOK(){//��֤����������
		try {
			Integer smtpPort = Integer.valueOf(smtp.getText());
			socket = new Socket("smtp."+server.getSelectedItem().toString(), smtpPort);
			output = new PrintWriter(new OutputStreamWriter(socket
					.getOutputStream()));
			input = new BufferedReader(new InputStreamReader(socket
					.getInputStream()));
			if(input.readLine().startsWith("220")){ 
				JOptionPane.showMessageDialog(this,"Authentication succeed!");
			}else{
			} 
			socket.close();
			input.close();
			output.close();
		} catch (Exception e) {
		//	System.out.println("Error " + e);
			JOptionPane.showMessageDialog(this,"Wrong mailserver!");
		}
	}
	
	void userOK(){//��֤�˻�����
		try {
			Integer smtpPort = Integer.valueOf(smtp.getText());
			socket = new Socket("smtp."+server.getSelectedItem().toString(), smtpPort);
			output = new PrintWriter(new OutputStreamWriter(socket
					.getOutputStream()));
			input = new BufferedReader(new InputStreamReader(socket
					.getInputStream()));
			System.out.println(input.readLine());
			output.println("HELO "+server.getSelectedItem().toString());//��֤����
			output.flush();
			System.out.println(input.readLine());
			output.println("AUTH LOGIN");
			output.flush();
			System.out.println(input.readLine());
			output.println(new BASE64Encoder().encode(user.getText().getBytes()));
			output.flush();
			System.out.println(input.readLine());
			output.println(new BASE64Encoder().encode((new String(psw.getPassword())).getBytes()));
			output.flush();
			if(input.readLine().startsWith("235")){ 
				JOptionPane.showMessageDialog(this,"Login succeed!");
			}else{
				JOptionPane.showMessageDialog(this,"Login failed");
			} 
			
			socket.close();
			input.close();
			output.close();
		} catch (Exception e) {
			JOptionPane.showMessageDialog(this,"Login failed");
		}
	}
	
	void clear(){//����û�������
		user.setText(null);
		psw.setText(null);
	}
	
	public void run(){
		OK=new Thread(this);
	}
	

	
	private void setLookAndFeel(){
		try{
			UIManager.setLookAndFeel("com.sun.java.swing.plaf.nimbus.NimbusLookAndFeel");
		}catch (Exception exc){
			
		}
	}
	

	
	public static void main(String[] arguments){
		Menu menu1=new Menu();
	}
}