package com.zhwli.emailclient;

import java.io.*;
import java.net.Socket;
import com.sun.xml.internal.messaging.saaj.packaging.mime.internet.MimeUtility;
import java.awt.event.*;

import javax.swing.JOptionPane;
import javax.swing.table.DefaultTableModel;

import sun.misc.BASE64Decoder;

public class ReceiveEmail implements ActionListener,Runnable{
	
	ReceiveEmailView gui;
	Thread receive;
	
	PrintWriter output;//输出
	BufferedReader input;//输入
	Socket socket = null;
	int count;
	DefaultTableModel tableModel;//表格模板
	String subject;//主题
	String from;//发件人
	String date;//日期
	String context="";//内容
	String contextY="";

	
	public ReceiveEmail(ReceiveEmailView in){
		gui=in;
		
	}

	public void actionPerformed(ActionEvent event){
		String command=event.getActionCommand();//获取反馈，看是否是发送邮件选项
		if(command.equals("flush")){
			flush();
			JOptionPane.showMessageDialog(gui,"Flush succeed!");
			gui.pb.setValue(0);
		}
		if(command.equals("Load All")){
			loadAll();
		}
		if(command.equals("delete")){
			delete();
		}
		if(command.equals("reply")){
			reply();
		}
		if(command.equals("report")){
			report();
		}
		if(command.equals("<<back")){
			backMenu();
		}
	}
	
	void backMenu(){//返回主界面
		gui.setVisible(false);
	}
	void flush(){//刷新
		gui.pb.setValue(10);
		receive=new Thread(this);//构建发送线程
		tableModel=(DefaultTableModel)gui.table.getModel();
		gui.pb.setValue(30);
		tableModel.setRowCount(0);// 清除原有行
		gui.pb.setValue(60);
		receive.start();
		gui.pb.setValue(100);
	}
	void loadAll(){//完全下载
		
		String load="";
		gui.pb.setValue(10);
		for(int i=0;i<count;i++){
		  String dateA=(String)gui.model.getValueAt(i, 0); 
		  String fromA=(String)gui.model.getValueAt(i, 1); 
		  String subjectA=(String)gui.model.getValueAt(i, 2); 
		  String contextA=(String)gui.model.getValueAt(i, 3);
			int j=i+1;
			String e="E-mail:"+j+"\nFrom:"+fromA+"\nSubject:"+subjectA+"\n"+dateA+"\nContext:\n"+contextA+"\n\n";
			load=load+e;
			if(gui.loadDelete.isSelected()){
		        output.println("dele "+(i+1));
			}
		}
		gui.pb.setValue(50);
		if(gui.loadDelete.isSelected()){
	        output.println("quit");
	        flush();
		}
		gui.text.setText(load);
		gui.pb.setValue(100);
		JOptionPane.showMessageDialog(gui,"Load succeed!");
		gui.pb.setValue(0);
	}
	void delete(){//删除
        output.println("dele "+(gui.i+1));
        output.println("quit");
        flush();
	}
	void reply(){//回复
		gui.guiS.send.clearAll();
		gui.guiS.setVisible(true);
		gui.guiS.toText.setText(gui.from);
		gui.guiS.subText.setText("Reply:"+gui.subject);
		
	}
	void report(){//转发
		gui.guiS.setVisible(true);
		gui.guiS.send.clearAll();
		gui.guiS.subText.setText("Repost:"+gui.subject);
		gui.guiS.mainText.setText("Repost:\n"+context);
	}
	
	public void run(){
		String mailServer=gui.guiMenu.server.getSelectedItem().toString();//邮箱服务器
		String username=gui.guiMenu.user.getText();//用户名
		String password=new String(gui.guiMenu.psw.getPassword());//密码
		Integer POP3Port = Integer.valueOf(gui.guiMenu.pop3.getText());
        try {
            // 向POP3服务程序建立一个套接字连接。
				if(mailServer=="hust.edu.cn"){
				socket = new Socket("mail.hust.edu.cn", POP3Port);
			}else{
            socket = new Socket("pop."+mailServer, POP3Port);
			}
			output = new PrintWriter(new OutputStreamWriter(socket
					.getOutputStream()),true);
			input = new BufferedReader(new InputStreamReader(socket
					.getInputStream()));
            // 显示pop服务程序的握手过程。
            System.out.println("S:" + input.readLine());
            output.println("user " + username);
            System.out.println("S:" + input.readLine());
            output.println("pass " + password);
            System.out.println("S:" + input.readLine());
            output.println("stat");
            String temp[] = input.readLine().split(" ");
            count = Integer.parseInt(temp[1]);//得到信箱中共有多少封邮件
            System.out.println( count+ "封邮件");
            //count+1
            for (int i = 1; i < count+1  ; i++) {//依次打印出邮件的内容
                    
                    output.println("retr " + i);
	                System.out.println("以下为第" + i + "封邮件的内容");
	                Boolean boolReady=false;
	                Boolean bool=true;//鉴别是否已经读取内容

	                while (true) {
	                    String reply = input.readLine();
	                    
	                  //发件人获取
	                    if(reply.startsWith("From")){
	                    int fromS=reply.indexOf("<");
	                    int fromE=reply.indexOf(">",fromS);
	                    from=reply.substring(fromS+1,fromE);
//	                    System.out.println("From:"+from);
	                    }
	                    
	                  //主题获取
	                    if(reply.startsWith("Subject")){
		                    subject=new String(reply.getBytes("iso-8859-1"));
		                    if (subject.contains("=")) {
			                    int subjectS=subject.indexOf("=?");
			                    int subjectE=subject.indexOf("?=",subjectS);
			                    String subjectY=subject.substring(subjectS,subjectE+2);
			                    subject=MimeUtility.decodeText(subjectY);
		                 }else{
		                	 String subjectN=subject.substring(9);
		                	 subject=subjectN;
		                 }
		                    }

	                  //日期获取
	                    if(reply.startsWith("Date")){
	                    	date=reply;
	                    }
	                    
	                    //内容获取
	                    if(bool&&boolReady){//判断行非空
	                    if((reply.endsWith("="))){//获取正文
	                    	contextY=contextY+reply;
                    		bool=false;
                        	context=new String(new BASE64Decoder().decodeBuffer(contextY),"gb2312");
                        	contextY="";
	                    }
	                    else{
	                    	contextY=contextY+reply;
	                    }
	                }
                    	
	                    if(reply.startsWith("Content-Transfer-Encoding: base64")){//判断起始点
	                    	boolReady=true;
	                    }
                    
                    
                    if (reply.toLowerCase().equals(".")) {
                        break;
                    }
                }
	        String[] arr={date,from,subject,context};
    		tableModel.addRow(arr);
    		gui.table.invalidate();
            }
			System.out.println("Done");
        }catch (Exception e) {
			System.out.println("Error " + e);
			return;
		}
		
		}
}

