import os
import RPi.GPIO as GPIO
import database

class HARDWARE:
	def __init__(self,alertbtn,alertled,smsbtn,smsled,ringbtn,ringsms,bellbtn,bellout):
		self.alert_btn=alertbtn
		self.alert_led=alertled
		self.sms_btn=smsbtn
		self.sms_led=smsled
		self.ring_btn=ringbtn
		self.ring_led=ringsms
		self.bell_btn=bellbtn
		self.bell_out=bellout
		self.RELAY_ON=GPIO.LOW
		self.RELAY_OFF=GPIO.HIGH
		GPIO.setmode(GPIO.BCM)
		GPIO.setwarnings(False)
		GPIO.setup(self.alert_btn, GPIO.IN)
		GPIO.setup(self.sms_btn, GPIO.IN)
		GPIO.setup(self.ring_btn, GPIO.IN)
		GPIO.setup(self.bell_btn, GPIO.IN)
		GPIO.setup(self.alert_led, GPIO.OUT)
		GPIO.setup(self.sms_led, GPIO.OUT)
		GPIO.setup(self.ring_led, GPIO.OUT)
		GPIO.setup(self.bell_out, GPIO.OUT)
		GPIO.output(self.bell_out, self.RELAY_OFF)
		GPIO.add_event_detect(self.alert_btn, GPIO.FALLING, callback=self.UpdateAlert)
		GPIO.add_event_detect(self.ring_btn, GPIO.FALLING, callback=self.UpdateRing)
		GPIO.add_event_detect(self.bell_btn, GPIO.FALLING, callback=self.DisableBell)
		GPIO.add_event_detect(self.sms_btn, GPIO.FALLING, callback=self.UpdateSms)
	def UpdateAlert(self,chanel):
		os.system('shutdown now -h')
#		db=database.DB("localhost","root","!()(!(*(","hbiot")
#		if(db.GetSetting('alert')):
#			db.SetSetting('alert',0)
#			db.SetSetting('ring',0)
#			db.SetSetting('sms',0)
##			db.SetSetting('email',0)
#			self.SetLed(self.alert_led,db.GetSetting('alert'))
#			self.SetLed(self.sms_led,db.GetSetting('sms'))
#			self.SetLed(self.ring_led,db.GetSetting('ring'))
#		else:
#			db.SetSetting('alert',db.GetSetting('alert'))
#			db.SetSetting('ring',1)
#			db.SetSetting('sms',1)
#			db.SetSetting('email',1)
#			self.SetLed(self.alert_led,db.GetSetting('alert'))
#			self.SetLed(self.sms_led,db.GetSetting('sms'))
#			self.SetLed(self.ring_led,db.GetSetting('ring'))
#		db.CloseDb()
	def UpdateRing(self,chanel):
		os.system('reboot')
#		db=database.DB("localhost","root","!()(!(*(","hbiot")
#		if(db.GetSetting('ring')):
#			db.SetSetting('ring',0)
#			self.SetLed(self.ring_led,db.GetSetting('ring'))
#		else:
#			db.SetSetting('ring',1)
#			self.SetLed(self.ring_led,db.GetSetting('ring'))
#		db.CloseDb()
	def UpdateSms(self,chanel):
		db=database.DB("localhost","root","!()(!(*(","hbiot")
		if(db.GetSetting('sms')):
			db.SetSetting('sms',0)
			self.SetLed(self.sms_led,db.GetSetting('sms'))
		else:
			db.SetSetting('sms',1)
			self.SetLed(self.sms_led,db.GetSetting('sms'))
		db.CloseDb()
	def DisableBell(self,chanel):
		db=database.DB("localhost","root","!()(!(*(","hbiot")
		db.SetSetting('bell',0)
		self.RingBell(0)
		db.CloseDb()
	def RingBell(self,val):
		if(val):
			GPIO.output(self.bell_out, self.RELAY_ON)
		else:
			GPIO.output(self.bell_out, self.RELAY_OFF)
	def SetLed(self,pin,val):
		GPIO.output(pin,val)
	def UpdateIndicator(self):
		db=database.DB("localhost","root","!()(!(*(","hbiot")
		al=db.GetSetting('alert')
		sm=db.GetSetting('sms')
		ri=db.GetSetting('ring')
		self.SetLed(self.alert_led,al)
		self.SetLed(self.sms_led,sm)
		self.SetLed(self.ring_led,ri)
		db.CloseDb()