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
	
	PrintWriter output;//���
	BufferedReader input;//����
	Socket socket = null;
	int count;
	DefaultTableModel tableModel;//���ģ��
	String subject;//����
	String from;//������
	String date;//����
	String context="";//����
	String contextY="";

	
	public ReceiveEmail(ReceiveEmailView in){
		gui=in;
		
	}

	public void actionPerformed(ActionEvent event){
		String command=event.getActionCommand();//��ȡ���������Ƿ��Ƿ����ʼ�ѡ��
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
	
	void backMenu(){//����������
		gui.setVisible(false);
	}
	void flush(){//ˢ��
		gui.pb.setValue(10);
		receive=new Thread(this);//���������߳�
		tableModel=(DefaultTableModel)gui.table.getModel();
		gui.pb.setValue(30);
		tableModel.setRowCount(0);// ���ԭ����
		gui.pb.setValue(60);
		receive.start();
		gui.pb.setValue(100);
	}
	void loadAll(){//��ȫ����
		
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
	void delete(){//ɾ��
        output.println("dele "+(gui.i+1));
        output.println("quit");
        flush();
	}
	void reply(){//�ظ�
		gui.guiS.send.clearAll();
		gui.guiS.setVisible(true);
		gui.guiS.toText.setText(gui.from);
		gui.guiS.subText.setText("Reply:"+gui.subject);
		
	}
	void report(){//ת��
		gui.guiS.setVisible(true);
		gui.guiS.send.clearAll();
		gui.guiS.subText.setText("Repost:"+gui.subject);
		gui.guiS.mainText.setText("Repost:\n"+context);
	}
	
	public void run(){
		String mailServer=gui.guiMenu.server.getSelectedItem().toString();//���������
		String username=gui.guiMenu.user.getText();//�û���
		String password=new String(gui.guiMenu.psw.getPassword());//����
		Integer POP3Port = Integer.valueOf(gui.guiMenu.pop3.getText());
        try {
            // ��POP3���������һ���׽������ӡ�
				if(mailServer=="hust.edu.cn"){
				socket = new Socket("mail.hust.edu.cn", POP3Port);
			}else{
            socket = new Socket("pop."+mailServer, POP3Port);
			}
			output = new PrintWriter(new OutputStreamWriter(socket
					.getOutputStream()),true);
			input = new BufferedReader(new InputStreamReader(socket
					.getInputStream()));
            // ��ʾpop�����������ֹ��̡�
            System.out.println("S:" + input.readLine());
            output.println("user " + username);
            System.out.println("S:" + input.readLine());
            output.println("pass " + password);
            System.out.println("S:" + input.readLine());
            output.println("stat");
            String temp[] = input.readLine().split(" ");
            count = Integer.parseInt(temp[1]);//�õ������й��ж��ٷ��ʼ�
            System.out.println( count+ "���ʼ�");
            //count+1
            for (int i = 1; i < count+1  ; i++) {//���δ�ӡ���ʼ�������
                    
                    output.println("retr " + i);
	                System.out.println("����Ϊ��" + i + "���ʼ�������");
	                Boolean boolReady=false;
	                Boolean bool=true;//�����Ƿ��Ѿ���ȡ����

	                while (true) {
	                    String reply = input.readLine();
	                    
	                  //�����˻�ȡ
	                    if(reply.startsWith("From")){
	                    int fromS=reply.indexOf("<");
	                    int fromE=reply.indexOf(">",fromS);
	                    from=reply.substring(fromS+1,fromE);
//	                    System.out.println("From:"+from);
	                    }
	                    
	                  //�����ȡ
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

	                  //���ڻ�ȡ
	                    if(reply.startsWith("Date")){
	                    	date=reply;
	                    }
	                    
	                    //���ݻ�ȡ
	                    if(bool&&boolReady){//�ж��зǿ�
	                    if((reply.endsWith("="))){//��ȡ����
	                    	contextY=contextY+reply;
                    		bool=false;
                        	context=new String(new BASE64Decoder().decodeBuffer(contextY),"gb2312");
                        	contextY="";
	                    }
	                    else{
	                    	contextY=contextY+reply;
	                    }
	                }
                    	
	                    if(reply.startsWith("Content-Transfer-Encoding: base64")){//�ж���ʼ��
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

