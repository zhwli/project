package com.zhwli.emailclient;

import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.MouseEvent;
import java.awt.event.MouseListener;

import javax.swing.*;
import javax.swing.table.DefaultTableModel;

public class ReceiveEmailView extends JFrame implements ActionListener,Runnable{
	
	//上-接到主目录
	Menu guiMenu;
	SendEmailView guiS;
	Thread receiveView;
	
	//下-接到邮件接收程序
	ReceiveEmail receive=new ReceiveEmail(this);
	
	int i;
	String date;
	String from; 
	String subject; 
	String context;
	
	//接收邮件界面
	//row1
	JPanel row1=new JPanel();
	String[] columnName={"Date","From","Subject","Context"};
	Object[][] data ={};
	DefaultTableModel model=new DefaultTableModel(data,columnName);
	JTable table=new JTable(model);
	
	//row1-e
	JPanel row1e=new JPanel();
	JTextArea text=new JTextArea(10,40);
	
	//row2
	JPanel row2=new JPanel();
	JCheckBox loadDelete=new JCheckBox("Load Delete",false);
	JButton flush=new JButton("flush");
	JButton loadAll=new JButton("Load All");
	JButton delete=new JButton("delete");
	JButton forward=new JButton("reply");
	JButton report=new JButton("report");
	JButton back=new JButton("<<back");
	JProgressBar pb=new JProgressBar();
	

		
	public ReceiveEmailView(Menu in,SendEmailView in1){
		super("Receive E-mail");
		setLookAndFeel();
		setSize(500,600);
		setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		guiMenu=in;
		guiS=in1;
		
		//整体布局
		FlowLayout receiveLayout=new FlowLayout(FlowLayout.CENTER);
		setLayout(receiveLayout);
		
		//添加监听
		back.addActionListener(receive);
		flush.addActionListener(receive);
		delete.addActionListener(receive);
		report.addActionListener(receive);
		forward.addActionListener(receive);
		loadAll.addActionListener(receive);
		
		//row1 layout
		FlowLayout layout1=new FlowLayout(FlowLayout.RIGHT);
		row1.setLayout(layout1);
		row1.add(table);	
		JScrollPane scrollpane1 =new JScrollPane(table,JScrollPane.VERTICAL_SCROLLBAR_ALWAYS,JScrollPane.HORIZONTAL_SCROLLBAR_NEVER);//添加滚动条
		row1.add(scrollpane1);
		row1.setPreferredSize(new Dimension(500,200));
		add(row1);		

		//row1-e
		FlowLayout layout1e=new FlowLayout(FlowLayout.RIGHT);
		row1e.setLayout(layout1e);
		row1e.add(text);
		JScrollPane scrollpane2 =new JScrollPane(text,JScrollPane.VERTICAL_SCROLLBAR_ALWAYS,JScrollPane.HORIZONTAL_SCROLLBAR_NEVER);//添加滚动条
		row1e.add(scrollpane2);
		row1e.setPreferredSize(new Dimension(500,200));
		add(row1e);	
		
		//row2 layout
		GridLayout layout2=new GridLayout(3,3,5,5);
		row2.setLayout(layout2);
		row2.add(loadDelete);
		row2.add(flush);
		row2.add(loadAll);
		row2.add(delete);
		row2.add(forward);
		row2.add(report);
		row2.add(back);
		row2.add(pb);
		pb.setStringPainted(true);
		add(row2);
		



		table.addMouseListener(new MouseListener() {
			  public void mousePressed(MouseEvent e) {
			    // 鼠标按下时的处理
			  }

			  public void mouseReleased(MouseEvent e) {
			    // 鼠标松开时的处理
			  }

			  public void mouseEntered(MouseEvent e) {
			    // 鼠标进入表格时的处理
			  }

			  public void mouseExited(MouseEvent e) {
			    // 鼠标退出表格时的处理
			  }

			  public void mouseClicked(MouseEvent e) {
			    // 鼠标点击时的处理		  
				  i = table.getSelectedRow(); // 取得用户所选单行 
					pb.setValue(30);
				  date=(String)model.getValueAt(i, 0); 
				  from=(String)model.getValueAt(i, 1); 
				  subject=(String)model.getValueAt(i, 2); 
				  context=(String)model.getValueAt(i, 3);
					pb.setValue(60);
				  text.setText(null);
				  text.setText("From:"+from+"\nSubject:"+subject+"\n"+date+"\nContext:\n"+context);
					pb.setValue(100);
				  if(loadDelete.isSelected()){
				  receive.delete();
				  }
			  }
			});

	}
	
	private void setLookAndFeel(){
		try{
			UIManager.setLookAndFeel("com.sun.java.swing.plaf.nimbus.NimbusLookAndFeel");
		}catch (Exception exc){
			
		}
	}

	public void actionPerformed(ActionEvent event){
		String command=event.getActionCommand();//获取反馈，看是否是发送邮件选项
		if(command.equals("Receive E-mail")){
			startSendingView();
		}
	}
	
	void startSendingView(){//转到发送邮件页面
		receiveView=new Thread(this);//构建发送线程
//		guiMenu.setVisible(false);
		receiveView.start();
	}


	
	public void run(){
		Thread thisThread=Thread.currentThread();
		if(receiveView==thisThread){
		ReceiveEmailView receiveView1=new ReceiveEmailView(guiMenu,guiS);
		receiveView1.setVisible(true);
		}
	}

}
