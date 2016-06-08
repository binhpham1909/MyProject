#!/usr/bin/env python  
# coding=utf-8 
import os 
import time
import database
import hware
import sim900a
import gmail
from thread import *
import socket
import json
import sys

# pinconnect
alertbtn=4
alertled=18
smsbtn=24
smsled=23
ringbtn=17
ringsms=25
bellbtn=27
bellout=22

hw=hware.HARDWARE(alertbtn,alertled,smsbtn,smsled,ringbtn,ringsms,bellbtn,bellout)
gsm900 = sim900a.SIM900A('/dev/ttyAMA0',115200)


# socket
HOST = ''   # Symbolic name meaning all available interfaces
PORT = 8989 # Arbitrary non-privileged port

MYSQL_HOST = "localhost"
MYSQL_USER = "root"
MYSQL_PASS = "!()(!(*("
MYSQL_DB = "hbiot"

#Function for handling connections. This will be used to create threads
def HandlerSocket(conn):
	while True:
		data = conn.recv(1024)
		print data
		sys.stdout.flush()
		if data=="":
			conn.send("404")
			break
		else:
			ss = json.loads(data)
			print ss['serial']
			sys.stdout.flush()
			re = HandlerSensor(ss['serial'],ss['temp'],ss['humi'])
			conn.send(re)
	conn.close()

def HandlerAlert(hw):
	db=database.DB(MYSQL_HOST,MYSQL_USER,MYSQL_PASS,MYSQL_DB)
	hw.UpdateIndicator()
	while 1:
		if(db.GetSetting('bell')):
			hw.RingBell(1)
		else:
			hw.RingBell(0)
		time.sleep(1)
	db.CloseDb()
def HandlerSIM900A(gsm):
	db = database.DB(MYSQL_HOST,MYSQL_USER,MYSQL_PASS,MYSQL_DB)
	gsm.Connect()
	nowtime = int(round(time.time() * 1000*1000))	#seconds
	lasttime = nowtime - 600
	gsm.checkModule()
	gsm.checkSIM()
	gsm.resetDefaultConfig()
	gsm.setCommandEcho(0)
	gsm.enableCallerInfo(1)
	
	while 1:
		nowtime= int(round(time.time() * 1000*1000))	#seconds
		if((nowtime-lasttime)>=600):
			if(db.GetSetting('sms_pending')):
				print "Start send sms at: %s"%(time.asctime( time.localtime(time.time()) ))
				sys.stdout.flush()
				lasttime=nowtime		
				rcs=db.GetRecipientSMS()
				ctn=db.GetAlertContent()
				for rc in rcs:
					gsm.sendSMS(rc,ctn)
				db.SetSetting('sms_pending',0)

#		if(db.GetSetting('ussd_pending')):
		serrcv = gsm.readSerial(0)
		if(serrcv!=''):
			if(('+CRING' in serrcv) and ('VOICE' in serrcv)):
				gsm.debug('Call incomming\r\n')
				phonenum = gsm.extractPhoneCall(serrcv)
				phonenum = gsm.convertPhoneGlobal('+84',phonenum)
				gsm.debug(phonenum + '\r\n')
				gsm.acceptCall()
				time.sleep(3)
				gsm.hangupCall()
		time.sleep(1)
	gsm.Disconnect()
	db.CloseDb()
def HanlerEmail():
	db=database.DB(MYSQL_HOST,MYSQL_USER,MYSQL_PASS,MYSQL_DB)
	mail=gmail.GMAIL('nafi1.alert@gmail.com','!()(!(*(')
	nowtime= int(round(time.time() * 1000*1000))	#seconds
	lasttime=nowtime
	while 1:
		nowtime= int(round(time.time() * 1000*1000))	#seconds
		if((nowtime-lasttime)>600):
			if(db.GetSetting('email_pending')):
				rcs=db.GetRecipientEmail()
				ctn=db.GetAlertContent()
				for k, v in rcs.iteritems():
					mail.SendEmail(v,"Cảnh báo quá nhiệt tới "+k,ctn)
				db.WasSendEmail()
		time.sleep(1)
	db.CloseDb()
def HandlerSensor(serial1,temp,humi):
	db=database.DB(MYSQL_HOST,MYSQL_USER,MYSQL_PASS,MYSQL_DB)
	trave = ""
	stype = serial1[0:3]
	dvid = db.GetDeviceIdFromSerial(serial1)
	if(stype=="THS"):
		db.UpdateTempValue(serial1,temp)
		db.UpdateHumiValue(serial1,humi)
		if(db.CheckHumiInRange(serial1,humi) or db.CheckTempInRange(serial1,temp)):
			if(db.GetSetting('alert')):
				warning_content=db.GetDeviceNameFromId(dvid)+'\nNhiet do: ' + temp + db.RangeTempToString(serial1) + '\nDo am: ' + humi + db.RangeHumiToString(serial1)
				db.UpdateWarningLog(dvid,warning_content)
			if(db.GetSetting('sms') and not(db.GetSetting('bell'))):
				db.SetSetting('sms_pending',1)
			if(db.GetSetting('email') and not(db.GetSetting('bell'))):
				db.SetSetting('email_pending',1)
			if(db.GetSetting('ring')):
				db.SetSetting('bell',1)
			trave = "alert>ON&&"
		else:
			trave = "alert>OFF&&"
	db.CloseDb()
	return trave

start_new_thread(HandlerAlert ,(hw,))
start_new_thread(HandlerSIM900A ,(gsm900,))
#start_new_thread(HanlerEmail ,())
try:
	#create an AF_INET, STREAM socket (TCP)
	s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
except socket.error, msg:
	print 'Failed to create socket. Error code: ' + str(msg[0]) + ' , Error message : ' + msg[1]
	sys.exit(); 
#s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
print 'Socket created'
#Bind socket to local host and port
try:
	s.bind((HOST, PORT))
except socket.error as msg:
	print 'Bind failed. Error Code : ' + str(msg[0]) + ' Message ' + msg[1]
	sys.exit()   
print 'Socket bind complete'
#Start listening on socket
s.listen(10)
print 'Socket now listening'
#now keep talking with the client

while 1:
	#wait to accept a connection - blocking call
	conn, addr = s.accept()
	print 'Connected with ' + addr[0] + ':' + str(addr[1])
	#start new thread takes 1st argument as a function name to be run, second is the tuple of arguments to the function.
	start_new_thread(HandlerSocket ,(conn,))
 
s.close()
db.CloseDb()