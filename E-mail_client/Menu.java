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
	//下-接到发送邮件界面、接收邮件界面
	SendEmailView sendView=new SendEmailView(this);
	ReceiveEmailView receiveView=new ReceiveEmailView(this,sendView);
	//下-接到发送邮件界面
//	SendEmail sendEmail=new SendEmail(sendView);
	
	
	PrintWriter output;//验证-socket输出
	BufferedReader input;//输入
	Socket socket = null;
	
	//row1目录条界面
	JPanel row1=new JPanel();
	JButton send=new JButton("Send E-mail");
	JButton receive=new JButton("Receive E-mail");
	//row2邮箱服务器设置
	JPanel row2=new JPanel();
	JLabel mailServer=new JLabel("MailServer:");
	JComboBox server=new JComboBox();
	JLabel smtpPort=new JLabel("Smtp Port:");
	JTextField smtp=new JTextField("25",5);
	JLabel pop3Port=new JLabel("Pop3 Port:");
	JTextField pop3=new JTextField("110",5);
	JButton OK1=new JButton("ServerOK");
	//row3输入用户名和密码
	JPanel row3=new JPanel();
	JLabel userName=new JLabel("Username:");
	JTextField user=new JTextField("appiaaai@163.com",35);	
	JLabel password=new JLabel("Password:");
	JPasswordField psw=new JPasswordField("请输入密码",35);
	JButton OK2=new JButton("UserOK");
	JButton clear=new JButton("Clear");
	
		
	public Menu(){
		super("E-mail");
		setLookAndFeel();
		setSize(500,600);
		setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		
		//整体布局
		GridLayout mainLayout=new GridLayout(5,1,10,0);
		setLayout(mainLayout);
		
		//对send、 receive键添加监听，转到发送邮件页面
		send.addActionListener(sendView);
		receive.addActionListener(receiveView);
		//对OK1、OK2、clear添加监听
		OK1.addActionListener(this);
		OK2.addActionListener(this);
		clear.addActionListener(this);
		
		//row1收发邮件按钮
		FlowLayout layout1=new FlowLayout(FlowLayout.CENTER,10,10);
		row1.setLayout(layout1);
		row1.add(send);
		row1.add(receive);
		add(row1);
		
		//row2选择邮件服务器
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
		
		//row3输入邮箱账号、密码
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
	
	//确认线程
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
	
	void serverOK(){//验证服务器配置
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
	
	void userOK(){//验证账户密码
		try {
			Integer smtpPort = Integer.valueOf(smtp.getText());
			socket = new Socket("smtp."+server.getSelectedItem().toString(), smtpPort);
			output = new PrintWriter(new OutputStreamWriter(socket
					.getOutputStream()));
			input = new BufferedReader(new InputStreamReader(socket
					.getInputStream()));
			System.out.println(input.readLine());
			output.println("HELO "+server.getSelectedItem().toString());//验证握手
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
	
	void clear(){//清空用户名密码
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