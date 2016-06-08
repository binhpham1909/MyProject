#!/usr/bin/env python  
# coding=utf-8  
import MySQLdb
import smtplib
from email.mime.text import MIMEText
import os  
import time
import datetime
from time import strftime
import serial
import RPi.GPIO as GPIO
dateString = '%Y/%m/%d %H:%M:%S'
db = MySQLdb.connect(host="localhost", user="root", passwd="!()(!(*(", db="hbwarning")	

ring_pin=5
ring_dismiss_pin=0
global_alert_pin=1
RING_ON=0
RING_OFF=1

GPIO.setmode(GPIO.BCM)

GPIO.setup(ring_dismiss_pin, GPIO.IN)
GPIO.setup(global_alert_pin, GPIO.IN)
GPIO.setup(ring_pin, GPIO.OUT)
GPIO.output(ring_pin, GPIO.HIGH)
print GPIO.input(ring_pin)
# Function for set and get global alert
def get_global_alert():
	re=0;
	cur = db.cursor()
	sql = "SELECT * FROM setting WHERE name = 'alert' AND value = %d" % (1)
	cur.execute(sql)
	if(cur.rowcount>0):
		re=1;
	else:
		re=0;
	cur.close()
	return re
def set_global_alert(value):
	cur = db.cursor()
	sql="UPDATE setting SET value = '%d' WHERE name = 'alert'" % (value)
	try:
		cur.execute(sql)
		db.commit()
	except:
		db.rollback()
	cur.close()
	
# Function for set and get dismiss alert
def get_dismiss():
	re=0;
	cur = db.cursor()
	sql = "SELECT * FROM setting WHERE name = 'dismiss' AND value = %d" % (1)
	cur.execute(sql)
	if(cur.rowcount>0):
		re=1;
	else:
		re=0;
	cur.close()
	return re
def set_dismiss_alert(value):
	cur = db.cursor()
	sql="UPDATE setting SET value = '%d' WHERE name = 'dismiss'" % (value)
	try:
		cur.execute(sql)
		db.commit()
	except:
		db.rollback()
	cur.close()

# Function for set and get ring alert
def set_ring(value):
	cur = db.cursor()
	sql="UPDATE setting SET value = '%d' WHERE name = 'ring'" % (value)
	try:
		cur.execute(sql)
		db.commit()
	except:
		db.rollback()
	cur.close()

def get_ring():
	re=0;
	cur = db.cursor()
	sql = "SELECT * FROM setting WHERE name = 'ring' AND value = %d" % (1)
	cur.execute(sql)
	if(cur.rowcount>0):
		re=0;
	else:
		re=1;
	cur.close()
	return re
def ringing(value):
	GPIO.output(ring_pin, value)
	print GPIO.input(ring_pin)

# Function for dismiss button
def setup_dismiss():
	set_ring(0)
	ringing(RING_OFF)
	set_dismiss_alert(0)
	print "dismiss ring alert"
	
def setup_alert():
	if GPIO.input(active_pin):
		set_global_alert(1)
		print "Active global alert"
	else:
		set_global_alert(0)
		print "Deactive global alert"

def millis():
	return int(round(time.time() * 1000))
# check alert and ring


def check_alert():
	cur = db.cursor()
	sql = "SELECT * FROM alert WHERE phone_dismiss = '%d' OR email_dismiss = '%d'" % (0,0)
	cur.execute(sql)
	if(cur.rowcount>0):
		GPIO.output(ring_pin, GPIO.LOW)	# Low is on relay
	cur.close()
# get sms wait for send in table alert
def get_pending_sms():	
	cur = db.cursor()
	sql = "SELECT * FROM alert WHERE phone_dismiss = '%d'" % (0)
	cur.execute(sql)
	for row in cur.fetchall() :
		send_sms(row[0])
		sms_dismiss(row[0])
	cur.close()
	
# send sms with id in table alert
def send_sms(id):
	cur = db.cursor()
	sql="SELECT * FROM user WHERE receive_alert = '%d'" %(1)
	cur.execute(sql)
	for row in cur.fetchall() :
		print "sending sms to: "	+row[2]+"\n"	
		sms = TextMessage(row[2],get_alert_content(id))	# 0-3 id, content, email_dismiss, phone_dismiss
		sms.connectPhone()
		sms.sendMessage()
		sms.disconnectPhone()
	cur.close()
	sms_dismiss(id)

# Get pending alert wait for send email
def get_pending_email():	
	cur = db.cursor()
	sql = "SELECT * FROM alert WHERE email_dismiss = '%d'" % (0)
	cur.execute(sql)
	for row in cur.fetchall() :
		send_email(row[0])
		email_dismiss(row[0])
	cur.close()
# Send email function
def send_email(id):
	cur = db.cursor()
	sql="SELECT * FROM user WHERE receive_alert = '%d'" %(1)
	email_content=""
	cur.execute(sql)
	for row in cur.fetchall() :
		print "sending email to: "	+row[3]+"\n"
		USERNAME = "nafi1.alert@gmail.com"
		PASSWORD = "!()(!(*("
		server = smtplib.SMTP('smtp.gmail.com:587')
		MAILTO  = row[3]
		msg = MIMEText(get_alert_content(id),'plain')
		msg['Subject'] = "Cảnh báo quá nhiệt"
		msg['From'] = USERNAME
		msg['To'] = MAILTO
		server.ehlo_or_helo_if_needed()
		server.starttls()
		server.ehlo_or_helo_if_needed()
		server.login(USERNAME,PASSWORD)
		server.sendmail(USERNAME, MAILTO, msg.as_string())
		server.quit()
	cur.close()
	email_dismiss(id)

# get alert content
def get_alert_content(id):
	cont="";
	cur = db.cursor()
	sql = "SELECT content FROM alert WHERE id = '%d'" % (id)
	cur.execute(sql)
	for row in cur.fetchall() :
		print row[0]
		cont=row[0]
	cur.close()
	return cont
	
# Dismiss email for this id alert after send_email
def email_dismiss(id):
	cur = db.cursor()
	sql="UPDATE alert SET email_dismiss = '%d' WHERE id = '%d'" % (1,id)
	try:
		cur.execute(sql)
		db.commit()
	except:
		db.rollback()
	cur.close()	
# Dismiss sms for this id alert after send_email
def sms_dismiss(id):
	cur = db.cursor()
	sql="UPDATE alert SET phone_dismiss = '%d' WHERE id = '%d'" % (1,id)
	try:
		cur.execute(sql)
		db.commit()
	except:
		db.rollback()
	cur.close()
	
# send SMS message
class TextMessage:
    def __init__(self, recipient="", message=""):
        self.recipient = recipient
        self.content = message
 
    def setRecipient(self, number):
        self.recipient = number
 
    def setContent(self, message):
        self.content = message
 
    def connectPhone(self):
        self.ser = serial.Serial('/dev/ttyAMA0', 115200, timeout=5)
        time.sleep(1)
 
    def sendMessage(self):
		self.ser.write('ATZ')
		self.ser.write("\r")
		time.sleep(1)
		self.ser.write('AT+CMGF=1')
		self.ser.write("\r")
		time.sleep(1)
		self.ser.write('AT+CMGS="' + self.recipient + '"')
		self.ser.write("\r")
		time.sleep(1)
		self.ser.write(self.content)
		self.ser.write("\r")
		time.sleep(1)
		self.ser.write(chr(26))
		time.sleep(1)
 
    def disconnectPhone(self):
        self.ser.close()
GPIO.add_event_detect(ring_dismiss_pin, GPIO.FALLING, callback=setup_dismiss)
GPIO.add_event_detect(global_alert_pin, GPIO.BOTH, callback=setup_alert)
set_global_alert(1)
set_dismiss_alert(0)
while (1):
	ringing(get_ring())
	get_pending_sms()
	get_pending_email()
	time.sleep(5)

 # close the connection
db.close ()
