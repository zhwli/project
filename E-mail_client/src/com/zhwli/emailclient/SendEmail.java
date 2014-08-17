package com.zhwli.emailclient;

import java.io.*;
import java.net.*;
import sun.misc.*;
import javax.swing.*;
import java.awt.event.*;

public class SendEmail implements ActionListener,Runnable{
	
//	Menu guiMenu;//������
	SendEmailView gui;//���ʼ�����
	Thread sending;
	
	PrintWriter output;//���
	BufferedReader input;//����
	Socket socket = null;

	
	public SendEmail(SendEmailView in){
		gui=in;
	}
	
	public void actionPerformed(ActionEvent event){
		String command=event.getActionCommand();//��ȡ���������Ƿ��Ƿ����ʼ�ѡ��
		if(command.equals("Send")){
			startSending();
		}
		if(command.equals("Clear")){
			clearAll();
		}
		if(command.equals("<<back")){
			backMenu();
		}
	}
	
	void startSending(){//��ʼ����
		sending=new Thread(this);//���������߳�
		sending.start();
		gui.sendButton.setText("Sending...");
		gui.toText.setEditable(false);
		gui.subText.setEditable(false);
		gui.mainText.setEditable(false);
	}
	
	void clearAll(){//�������
		gui.toText.setText(null);
		gui.subText.setText(null);
		gui.mainText.setText(null);
	}
	
	void backMenu(){//����������
		gui.setVisible(false);
	}
	
	void stopSending(){
		gui.sendButton.setText("Send");
		gui.toText.setEditable(true);
		gui.subText.setEditable(true);
		gui.mainText.setEditable(true);
		sending=null;
	}
	
	public void run(){
		String accept=gui.toText.getText();
		String subject=gui.subText.getText();
		String message=gui.mainText.getText();
		String mailServer=gui.guiMenu.server.getSelectedItem().toString();//���������
		String username=gui.guiMenu.user.getText();//�û���
		String password=new String(gui.guiMenu.psw.getPassword());//����
		Integer smtpPort = Integer.valueOf(gui.guiMenu.smtp.getText());
			try {
				if(mailServer=="hust.edu.cn"){
				socket = new Socket("mail.hust.edu.cn", smtpPort);
			}else{
				socket = new Socket("smtp."+mailServer, smtpPort);
			}
				output = new PrintWriter(new OutputStreamWriter(socket
						.getOutputStream()));
				input = new BufferedReader(new InputStreamReader(socket
						.getInputStream()));
				output.println("HELO "+mailServer);
				output.flush();
				System.out.println(input.readLine());
				//��֤��½
				output.println("AUTH LOGIN");
				output.flush();
				System.out.println(input.readLine());
				output.println(new BASE64Encoder().encode(username.getBytes()));
				output.flush();
				System.out.println(input.readLine());
				output.println(new BASE64Encoder().encode(password.getBytes()));
				output.flush();
				System.out.println(input.readLine());
				gui.pb.setValue(20);
				output.println("mail from: <"+username+">");
				output.flush();
				System.out.println(input.readLine());
				gui.pb.setValue(40);
				output.println("rcpt to: <" + accept + ">");
				output.flush();
				System.out.println(input.readLine());
				gui.pb.setValue(60);

				//����
				output.println("data");
				output.flush();
				System.out.println(input.readLine());
				String con = "From: "+username+"\r\n";
				con += "To: <" + accept + ">\r\n";
				con = con +"Subject:"+ subject+"\r\n\n";
				con = con + message+"\r\n";
				con = con + ".\r\n";
				output.println(con);
				output.flush();
				System.out.println(input.readLine());
				gui.pb.setValue(80);
				if(input.readLine().startsWith("250")){
					gui.pb.setValue(100);
					JOptionPane.showMessageDialog(gui,"Send succeed!");
					gui.pb.setValue(0);
				}else{
				} 
				socket.close();
				input.close();
				output.close();
				System.out.println("Done");
				stopSending();

			} catch (Exception e) {
				JOptionPane.showMessageDialog(gui,"Send failed");
				stopSending();
				System.out.println("Error " + e);
				return;
			}
		}
}