#!/usr/bin/env python  
# coding=utf-8  
import time
import serial
import sys

class SIM900A:
# connection
	def __init__(self, port, baud):
		self.port = port
		self.baud = baud
		self.DEBUG = 1;
	def Connect(self):
		self.ser = serial.Serial(self.port, self.baud, timeout=5)
		time.sleep(1)
	def Disconnect(self):
		self.ser.close()
	def flushSerial(self):
		self.ser.flushInput()
		self.ser.flushOutput()
	def readSerial(self, timewait):
		respone = ''
		time.sleep(timewait)
		while self.ser.inWaiting() > 0:
			respone += self.ser.read(1)
		return respone
#debug 
	def debug(self,message):
		if(self.DEBUG):
			print message
			sys.stdout.flush()
# Module function
	def checkModule(self):
		self.flushSerial()
		self.ser.write('AT\r\n')
		self.debug('Check module SIM900A.\r\n')
		self.debug('>>: AT\r\n')
		time.sleep(1)
		respone = self.readSerial(1)
		self.debug('<<: '+respone)
		if('OK' in respone):
			return 1
		else:
			return 0
	def checkSIM(self):
		self.flushSerial()
		self.ser.write('AT+CPIN?\r\n');
		self.debug('Check SIM/Card.\r\n')
		self.debug('>>: AT+CPIN?\r\n')
		respone = self.readSerial(1)
		self.debug('<<: '+respone)
		if('READY' in respone):
			return 1
		else:
			return 0
	def resetDefaultConfig(self):
		self.flushSerial()
		self.ser.write('ATZ\r\n');
		self.debug('Set SIM900A to default config.\r\n')
		self.debug('>>: ATZ\r\n')
		respone = self.readSerial(1)
		self.debug('<<: '+respone)
		if('OK' in respone):
			return 1
		else:
			return 0
	def setCommandEcho(self, echoMode):
		self.flushSerial()
		self.debug('Set command echo.\r\n')
		if(not echoMode):
			self.ser.write('ATE0\r\n');
			self.debug('Echo command OFF\r\n')
			self.debug('>>: ATE0\r\n')
		else:
			self.ser.write('ATE1\r\n');
			self.debug('Echo command ON\r\n')
			self.debug('>>: ATE1\r\n')
		respone = self.readSerial(1)
		self.debug('<<: '+respone)
		if('OK' in respone):
			return 1
		else:
			return 0	
# function for call
	def enableCallerInfo(self, mode):
		self.flushSerial()
		self.debug('Enable/Disable caller info.\r\n')
		if(not mode):
			self.ser.write('AT+CLIP=0\r\n');
			self.debug('>>: AT+CLIP=0\r\n')
			self.debug('Disable caller info.\r\n')
			
		else:
			self.ser.write('AT+CLIP=1\r\n');
			self.debug('>>: AT+CLIP=1\r\n')
			self.debug('Enable caller info.\r\n')
		respone = self.readSerial(1)
		self.debug('<<: '+respone)
		if('OK' in respone):
			return 1
		else:
			return 0
	def callNumber(self, phonenumber):
		self.flushSerial()
		self.ser.write('ATD%s;\r\n'%(phonenumber))
		self.debug('Make call to %s.\r\n'%(phonenumber))
		self.debug('>>: ATD%s;\r\n'%(phonenumber))
		respone = self.readSerial(2)
		self.debug('<<: '+respone)
		if('OK' in respone):
			if('NO DIALTONE' in respone):
				return 'NO DIALTONE',''
			elif('BUSY' in respone):
				return 'BUSY',''
			elif('NO CARRIER' in respone):
				return 'NO CARRIER',''
			elif('NO ANSWER' in respone):
				return 'NO ANSWER',''
			elif('CUSD' in respone):
				return 'USSD',respone
	def payAccount(self, code):
		comm,res = self.callNumber('*100*' + code + '#')
		return res
	def checkBalance(self):
		comm,res = self.callNumber('*101#')
		return res
	def hangupCall(self):
		self.flushSerial()
		self.ser.write('ATH\r\n');
		self.debug('Hangup call.\r\n')
		self.debug('>>: ATH\r\n')
		respone = self.readSerial(1)
		self.debug('<<: '+respone)
		if('OK' in respone):
			return 1
		else:
			return 0
	def acceptCall(self):
		self.flushSerial()
		self.ser.write('ATA1\r\n');
		self.debug('Accept call.\r\n')
		self.debug('>>: ATA1\r\n')
		respone = self.readSerial(1)
		self.debug('<<: '+respone)
		if('OK' in respone):
			return 1
		else:
			return 0
	def extractPhoneCall(self, stphone):
		# extract number +CLIP: "0988472989",161,"",,"",0
		self.debug('extract phone number\r\n');
		StBrake = stphone.find('+CLIP',0)
		StBrake = stphone.find('"',StBrake)
		NdBrake = stphone.find('"',StBrake)
		phoneNumber = stphone[StBrake:NdBrake]
		return phoneNumber
	def convertPhoneGlobal(self, zipcode, phonenum):
		return zipcode + phonenum[1:]
#function for SMS
	def setSMSTextMode(self, mode):
		self.ser.write('AT+CMGF==%d\r\n'%(mode));
		self.debug('Set SMS to Text mode.\r\n')
		self.debug('>>: AT+CMGF==%d\r\n'%(mode))
		respone = self.readSerial(1)
		self.debug('<<: '+respone)
		if('OK' in respone):
			return 1
		else:
			return 0		
	def sendSMS(self, number, content):
#		self.resetDefaultConfig()
		self.setSMSTextMode(1)
		self.ser.write("AT+CMGS=%s\r\n"%(number))
		time.sleep(1)
		self.ser.write(content)
		self.ser.write("\r\n")
		time.sleep(1)
		self.ser.write(chr(26))
		time.sleep(1)
	def readSMS(self):
		a=1
	