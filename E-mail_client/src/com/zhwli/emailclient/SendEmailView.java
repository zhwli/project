package com.zhwli.emailclient;

import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import javax.swing.*;

public class SendEmailView extends JFrame implements ActionListener,Runnable{
	
	//上-接到主目录
	Menu guiMenu;
	Thread sendingView;
	
	//下-接到发送邮件的程序
	SendEmail send=new SendEmail(this);
	ReceiveEmailView receive=new ReceiveEmailView(guiMenu,this);
	
//	String max1="";

	//发送邮件界面	
	//set row1
		JPanel row1=new JPanel();
		JLabel toLabel=new JLabel("To:");
		JTextField toText=new JTextField("2500499195@qq.com",35);
		
		//set row2
		JPanel row2=new JPanel();
		JLabel subLabel=new JLabel("Subject:");
		JTextField subText=new JTextField("",35);
		
		//set row3
		JPanel row3=new JPanel();
		JLabel mesLabel=new JLabel("Message:");
		JTextArea mainText=new JTextArea(20,33);
		
		//set row4
		JPanel row4=new JPanel();
		JProgressBar pb=new JProgressBar();
		JButton sendButton=new JButton("Send");
		JButton clear=new JButton("Clear");
		JButton back=new JButton("<<back");
		
	public SendEmailView(Menu in){
		super("Send E-mail");
		setLookAndFeel();
		setSize(500,600);
		setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		guiMenu=in;
		
		//整体布局
		FlowLayout sendLayout=new FlowLayout(FlowLayout.RIGHT);
		setLayout(sendLayout);
		

		//发送邮件界面布局
		//添加监听
		sendButton.addActionListener(send);
		clear.addActionListener(send);
		back.addActionListener(send);
		
		//row1 layout
		FlowLayout layout1=new FlowLayout(FlowLayout.RIGHT);
		row1.setLayout(layout1);
		row1.add(toLabel);
		row1.add(toText);
		add(row1);
		
		//row2 layout
		FlowLayout layout2=new FlowLayout(FlowLayout.RIGHT);
		row2.setLayout(layout2);
		row2.add(subLabel);
		row2.add(subText);
		add(row2);
		
		//row3 layout
		FlowLayout layout3=new FlowLayout(FlowLayout.RIGHT);
		row3.setLayout(layout3);
		row3.add(mesLabel);
		row3.add(mainText);	
		
		mainText.setLineWrap(true);
		mainText.setWrapStyleWord(true);
		JScrollPane scroll=new JScrollPane(mainText,JScrollPane.VERTICAL_SCROLLBAR_ALWAYS,JScrollPane.HORIZONTAL_SCROLLBAR_NEVER);
		row3.add(scroll);
		add(row3);
		
		//row4 layout
		FlowLayout layout4=new FlowLayout(FlowLayout.RIGHT);
		row4.add(pb);
		pb.setStringPainted(true);
		row4.setLayout(layout4);
		row4.add(sendButton);
		row4.add(clear);
		row4.add(back);
		add(row4);
//		setVisible(true);
	}
	

	

	public void actionPerformed(ActionEvent event){
		String command=event.getActionCommand();//获取反馈，看是否是发送邮件选项
		if(command.equals("Send E-mail")){
			startSendingView();
		}
	}
	
	void startSendingView(){//转到发送邮件
		sendingView=new Thread(this);//构建发送线程
		sendingView.start();
	}
	

	
	public void run(){
		Thread thisThread=Thread.currentThread();
		if(sendingView==thisThread){
		SendEmailView sendView1=new SendEmailView(guiMenu);
		sendView1.setVisible(true);
		}
	}
	
	
	private void setLookAndFeel(){
		try{
			UIManager.setLookAndFeel("com.sun.java.swing.plaf.nimbus.NimbusLookAndFeel");
		}catch (Exception exc){
			
		}
	}
	
}
	



