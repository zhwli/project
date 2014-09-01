package com.zhwli.shutdown;

import java.awt.Dimension;
import java.awt.FlowLayout;
import java.awt.GridLayout;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.io.IOException;
import java.util.Date;

import javax.swing.JButton;
import javax.swing.JComboBox;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JOptionPane;
import javax.swing.JPanel;
import javax.swing.UIManager;

import com.sun.org.apache.bcel.internal.generic.NEW;



public class ShutDownCountdown extends JFrame implements ActionListener,Runnable{
	
	static int hour = 0;
	static int minute = 0;
	static int second = 0;
	static String action = "shut down";
	static String ac = "s";
	
	Thread shutDownCountdownThread;
	
	//计时条
	JPanel row1 = new JPanel();
	JLabel show = new JLabel("0 hour ");
	
	//时间设置
	JPanel row2 = new JPanel();
	JLabel timeSetLabel = new JLabel("TimeSet:");
	JComboBox<Integer> timeSetHour = new JComboBox<Integer>();
	JLabel labelHour = new JLabel("hour");
	JComboBox<Integer> timeSetMin = new JComboBox<Integer>();
	JLabel labelMin = new JLabel("minute");
	JComboBox<Integer> timeSetSec = new JComboBox<Integer>();
	JLabel labelSec = new JLabel("second");
	
	//功能键
	JPanel row3 = new JPanel();
	JComboBox<String> style = new JComboBox<String>();
	JButton startButton = new JButton("START");
	JButton pauseButton = new JButton("pause");
	JButton stopButton = new JButton("STOP");
	
	//当前时间
	JPanel row4 = new JPanel();
	JLabel currenttime = new JLabel("TIME");
	
	public ShutDownCountdown() {//构造函数
		super("SHUT DOWN");
		setLookAndFeel();
		setSize(400,300);
		setLocationRelativeTo(null); 
		setDefaultCloseOperation(EXIT_ON_CLOSE);
		
		//监听
		startButton.addActionListener(this);
		pauseButton.addActionListener(this);
		stopButton.addActionListener(this);

		GridLayout layout = new GridLayout(4,1);
		setLayout(layout);
	
		//row2
		FlowLayout layout2 = new FlowLayout(FlowLayout.CENTER);
		row2.setLayout(layout2);
		row2.add(timeSetLabel);
		
		row2.add(timeSetHour);
		timeSetHour.setEditable(true);
		timeSetHour.setPreferredSize(new Dimension(40,20));
		timeSetHour.addItem(0);
		timeSetHour.addItem(1);
		timeSetHour.addItem(2);
		timeSetHour.addItem(3);
		timeSetHour.addItem(4);
		timeSetHour.addItem(5);
		row2.add(labelHour);
		
		
		row2.add(timeSetMin);
		timeSetMin.setEditable(true);
		timeSetMin.setPreferredSize(new Dimension(50,20));
		timeSetMin.addItem(0);
		timeSetMin.addItem(5);
		timeSetMin.addItem(10);
		timeSetMin.addItem(20);
		timeSetMin.addItem(30);
		row2.add(labelMin);
		
		row2.add(timeSetSec);
		timeSetSec.setEditable(true);
		timeSetSec.setPreferredSize(new Dimension(50,20));
		timeSetSec.addItem(30);
		timeSetSec.addItem(20);
		timeSetSec.addItem(10);
		timeSetSec.addItem(5);
		timeSetSec.addItem(0);
		row2.add(labelSec);
		
		add(row2);

		//row1
		FlowLayout layout1 = new FlowLayout(FlowLayout.CENTER);
		row1.setLayout(layout1);
		row1.add(show);
		add(row1);
		
		//row3
		FlowLayout layout3 = new FlowLayout(FlowLayout.CENTER);
		row3.setLayout(layout3);
		row3.add(style);
		style.addItem("shut down");
		style.addItem("restart");
		style.addItem("log off");
		row3.add(startButton);
		row3.add(pauseButton);
		row3.add(stopButton);
		add(row3);
		
		FlowLayout layout4 = new FlowLayout(FlowLayout.CENTER);
		row4.setLayout(layout4);
		row4.add(currenttime);
		add(row4);
		
		setVisible(true);
		row1.setVisible(false);//初始不显示倒计时
		pauseButton.setEnabled(false);//初始暂停键和停止键屏蔽
		stopButton.setEnabled(false);
	}
	
	//反馈获取
	public void actionPerformed(ActionEvent event){
		String command = event.getActionCommand();
		if(command.equalsIgnoreCase("start")){
			timeStart();
		}else if(command.equalsIgnoreCase("pause")){
			timePause();
		}else if(command.equalsIgnoreCase("continue")){
			timeContinue();
		}else if(command.equalsIgnoreCase("stop")){
			timeStop();
		}
	}

	@SuppressWarnings("deprecation")
	private void timeStart() {
		//获取初始值
		hour = Integer.parseInt(timeSetHour.getSelectedItem().toString());
		minute = Integer.parseInt(timeSetMin.getSelectedItem().toString());
		second = Integer.parseInt(timeSetSec.getSelectedItem().toString());
		action = style.getSelectedItem().toString();
		if(action.equals("shut down")){
			ac = "s";
		}else if(action.equals("restart")){
			ac = "r";
		}else if(action.equals("log off")){
			ac = "l";
		}
		//执行
		shutDownCountdownThread = new Thread(this);
		shutDownCountdownThread.start();
		row1.setVisible(true);
		pauseButton.setText("pause");
		
		//执行限制及解除
		startButton.setEnabled(false);
		pauseButton.setEnabled(true);
		stopButton.setEnabled(true);
		timeSetHour.setEditable(false);//输入限制
		timeSetMin.setEditable(false);
		timeSetSec.setEditable(false);
		timeSetHour.enable(false);
		timeSetMin.enable(false);
		timeSetSec.enable(false);
		style.setEnabled(false);

	}
	
	private void timePause() {
		shutDownCountdownThread = null;
		pauseButton.setText("continue");
		
	}
	
	private void timeContinue() {
		shutDownCountdownThread = new Thread(this);
		shutDownCountdownThread.start();
		pauseButton.setText("pause");
	}
	
	@SuppressWarnings("deprecation")
	private void timeStop() {
		shutDownCountdownThread = null;
		row1.setVisible(false);
		pauseButton.setText("pause");
		//执行限制及解除
		startButton.setEnabled(true);
		pauseButton.setEnabled(false);
		stopButton.setEnabled(false);
		timeSetHour.enable(true);//输入限制解除
		timeSetMin.enable(true);
		timeSetSec.enable(true);
		timeSetHour.setEditable(true);
		timeSetMin.setEditable(true);
		timeSetSec.setEditable(true);
		style.setEnabled(true);
	}
	
	public void run(){
		Thread thisThread = Thread.currentThread();
		
		if(!(hour>=0)||!(minute>=0&&minute<=59)||!(second>=0&&second<=59)){
			JOptionPane.showMessageDialog(this,"Input Error!");
			return;
		}
		

		
		while(shutDownCountdownThread==thisThread){
			show.setText("your computer will "+action+" in "+hour+" hour "+minute+" minute "+second+" second.");
			try {
				Thread.sleep(1000);
			} catch (InterruptedException e) {
				e.printStackTrace();
			}
			
			if(second!=0){
				second--;
			}else if(minute!=0){
				minute--;
				second = 59;
			}else if(hour!=0){
				hour--;
				minute = 59;
				second = 59;
			}else if(second==0&&minute==0&&hour==0){
				timeStop();
				Runtime rt=Runtime.getRuntime();
				 try {
					rt.exec("shutdown.exe -"+ac);
				} catch (IOException e) {
					e.printStackTrace();
				}
			}else{
				JOptionPane.showMessageDialog(this,"error!");
			}
		}
		
		
	}
	
	private void setLookAndFeel(){
		try{
			UIManager.setLookAndFeel("com.sun.java.swing.plaf.nimbus.NimbusLookAndFeel");
		}catch (Exception exc){
			
		}
	}
	
	public static void main(String[] args){
		ShutDownCountdown shutDown = new ShutDownCountdown();
		while(true){
			//time
			shutDown.currenttime.setText(new Date()+"");
			try {
				Thread.sleep(50);
			} catch (InterruptedException e) {
				e.printStackTrace();
			}
		}
	}

}
