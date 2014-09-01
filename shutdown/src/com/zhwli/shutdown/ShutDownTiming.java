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



	@SuppressWarnings("serial")
	public class ShutDownTiming extends JFrame implements ActionListener,Runnable{
		
		static int hour = 0;
		static int minute = 0;
		Runtime rt=Runtime.getRuntime();
		
		Thread shutDownTimingThread;
		
		//计时条
		JPanel row1 = new JPanel();
		JLabel show = new JLabel("your computer will shut down at");
		JLabel time = new JLabel("00:00");
		
		//时间设置
		JPanel row2 = new JPanel();
		JLabel timeSetLabel = new JLabel("TimeSet:");
		JComboBox<String> timeSetHour = new JComboBox<String>();
		JLabel labelHour = new JLabel(":");
		JComboBox<String> timeSetMin = new JComboBox<String>();
		
		//功能键
		JPanel row3 = new JPanel();
		JButton startButton = new JButton("START");
		JButton stopButton = new JButton("STOP");

		//当前时间
		JPanel row4 = new JPanel();
		JLabel currenttime = new JLabel("TIME");
		
		public ShutDownTiming() {//构造函数
			super("SHUT DOWN");
			setLookAndFeel();
			setSize(400,300);
			setDefaultCloseOperation(EXIT_ON_CLOSE);
			
			//监听
			startButton.addActionListener(this);
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
			for(int i=0;i<24;i++){
				String t;
				if(i<10){
					t = "0"+i;
				}else{
					t = ""+i;
				}
			timeSetHour.addItem(t);
			}
			row2.add(labelHour);
			
			
			row2.add(timeSetMin);
			timeSetMin.setEditable(true);
			timeSetMin.setPreferredSize(new Dimension(50,20));
			for(int i=0;i<60;i++){
				String t;
				if(i<10){
					t = "0"+i;
				}else{
					t = ""+i;
				}
			timeSetMin.addItem(t);
			}
			add(row2);

			//row1
			FlowLayout layout1 = new FlowLayout(FlowLayout.CENTER);
			row1.setLayout(layout1);
			row1.add(show);
			row1.add(time);
			add(row1);
			
			
			//row3
			FlowLayout layout3 = new FlowLayout(FlowLayout.CENTER);
			row3.setLayout(layout3);
			row3.add(startButton);
			row3.add(stopButton);
			add(row3);
			
			//row4
			FlowLayout layout4 = new FlowLayout(FlowLayout.CENTER);
			row4.setLayout(layout4);
			row4.add(currenttime);
			add(row4);
			
			setVisible(true);
			row1.setVisible(false);//初始不显示倒计时
			stopButton.setEnabled(false);//初始停止键屏蔽
		}
		
		//反馈获取
		public void actionPerformed(ActionEvent event){
			String command = event.getActionCommand();
			if(command.equalsIgnoreCase("start")){
				timeStart();
			}else if(command.equalsIgnoreCase("stop")){
				timeStop();
			}
		}

		@SuppressWarnings("deprecation")
		private void timeStart() {
			//获取初始值
			hour = Integer.parseInt(timeSetHour.getSelectedItem().toString());
			minute = Integer.parseInt(timeSetMin.getSelectedItem().toString());
			//执行
			shutDownTimingThread = new Thread(this);
			shutDownTimingThread.start();
			row1.setVisible(true);
			
			//执行限制及解除
			startButton.setEnabled(false);
			stopButton.setEnabled(true);
			timeSetHour.setEditable(false);//输入限制
			timeSetMin.setEditable(false);
			timeSetHour.enable(false);
			timeSetMin.enable(false);

		}
		
		@SuppressWarnings("deprecation")
		private void timeStop() {
			//执行
			try {
				rt.exec("shutdown -a");
			} catch (IOException e) {
				e.printStackTrace();
			}
			shutDownTimingThread = null;
			row1.setVisible(false);
			//执行限制及解除
			startButton.setEnabled(true);
			stopButton.setEnabled(false);
			timeSetHour.enable(true);//输入限制解除
			timeSetMin.enable(true);
			timeSetHour.setEditable(true);
			timeSetMin.setEditable(true);
			JOptionPane.showMessageDialog(this,"shut down stoped!");
		}
		
		public void run(){
			String H = "00";
			String M = "00";
			if(!(hour>=0&&hour<=23)||!(minute>=0&&minute<=59)){
				JOptionPane.showMessageDialog(this,"Input Error!");
				return;
			}
			if(hour<10){
				H = "0"+hour;
			}else{
				H = hour+"";
			}
			if(minute<10){
				M = "0"+minute;
			}else{
				M = minute + "";
			}

			time.setText(H+":"+M);
			 try {
				 System.out.print("at "+H+":"+M+" shutdown -s");
				rt.exec("schtasks "+H+":"+M+" shutdown -s");//指令错误，at已被弃用
			} catch (IOException e) {
				e.printStackTrace();
			}
			
			
		}
		
		
		private void setLookAndFeel(){
			try{
				UIManager.setLookAndFeel("com.sun.java.swing.plaf.nimbus.NimbusLookAndFeel");
			}catch (Exception exc){
				
			}
		}
		
//		public static void main(String[] args){
//			ShutDownTiming shutDown = new ShutDownTiming();
//		while(true){
//			//time
//			shutDown.currenttime.setText(new Date()+"");
//			try {
//				Thread.sleep(50);
//			} catch (InterruptedException e) {
//				e.printStackTrace();
//			}
//		}
//	}
	}

