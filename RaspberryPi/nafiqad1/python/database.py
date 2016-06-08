import MySQLdb
class DB:
	def __init__(self,host,user,passwd,db):
		self.db = MySQLdb.connect(host, user, passwd, db)
# Function for set and get setting: alert,ring,bell,sms,email,sms_pending,email_pending
	def GetSetting(self,name):
		ret=0;
		cur = self.db.cursor()
		sql = "SELECT * FROM setting WHERE name = '%s' AND value = %d" % (name,1)
		cur.execute(sql)
		if(cur.rowcount>0):
			ret= 1
		else:
			ret= 0
		cur.close()
		return ret
	def SetSetting(self,name,value):
		cur = self.db.cursor()
		sql="UPDATE setting SET value = '%d' WHERE name = '%s'" % (value,name)
		try:
			cur.execute(sql)
			self.db.commit()
		except:
			self.db.rollback()
		cur.close()
	def WasSendSms(self):
		self.SetSetting('sms_pending',0)
	def WasSendEmail(self):
		self.SetSetting('email_pending',0)
	def GetAlertContent(self):
		trave=''
		cur = self.db.cursor()
		sql = "SELECT content FROM warning_log WHERE id=(SELECT MAX(id) FROM warning_log)"
		cur.execute(sql)
		for row in cur.fetchall():
			trave=row[0]
		cur.close()
		return trave
	def GetRecipientSMS(self):
		trave=[]
		cur = self.db.cursor()
		sql = "SELECT phone FROM receive_sms"
		cur.execute(sql)
		for row in cur.fetchall() :
			trave.append(row[0])
		cur.close()
		return trave
	def GetRecipientEmail(self):
		trave={}
		cur = self.db.cursor()
		sql = "SELECT name,email FROM receive_email"
		cur.execute(sql)
		for row in cur.fetchall():
			trave[row[0]]=row[1]
		cur.close()
		return trave

	def CheckExistDeviceSerial(self,serial1):
		ret=0
		cur=self.db.cursor()
		sql="SELECT id FROM device WHERE serial = '%s'"%(serial1)
		cur.execute(sql)
		if(cur.rowcount>0):
			ret=1
		else:
			ret=0
		cur.close()
		return ret
	def GetDeviceIdFromSerial(self,serial1):
		trave=''
		cur = self.db.cursor()
		sql = "SELECT id FROM device WHERE serial ='%s'"%serial1
		cur.execute(sql)
		for row in cur.fetchall() :
			trave=row[0]
		cur.close()
		return trave 
	def GetDeviceNameFromId(self,dvid):
		trave=''
		cur = self.db.cursor()
		sql = "SELECT name FROM device WHERE id = '%s'"%dvid
		cur.execute(sql)
		for row in cur.fetchall() :
			trave=row[0]
		cur.close()
		return trave    
	def UpdateTempValue(self,serial1,value):
		serialid = self.GetDeviceIdFromSerial(serial1)
		cur = self.db.cursor()
		sql="UPDATE sensor_temp SET temp = '%s' WHERE device = '%s'" % (value,serialid)
		try:
			cur.execute(sql)
			self.db.commit()
		except:
			self.db.rollback()
		cur.close()
	def UpdateHumiValue(self,serial1,value):
		serialid = self.GetDeviceIdFromSerial(serial1)
		cur = self.db.cursor()
		sql="UPDATE sensor_humi SET humi = '%s' WHERE device = '%s'" % (value,serialid)
		try:
			cur.execute(sql)
			self.db.commit()
		except:
			self.db.rollback()
		cur.close()
	def CheckHumiInRange(self,serial1,value):
		deviceid = self.GetDeviceIdFromSerial(serial1)
		ret = 0
		cur = self.db.cursor()
		sql = "SELECT * FROM sensor_humi WHERE device = '%s' AND (min_humi>'%s' OR max_humi<'%s') AND alert = 1"%(deviceid,value,value)
		cur.execute(sql)
		if(cur.rowcount>0):
			ret= 1
		else:
			ret= 0
		cur.close()
		return ret
	def CheckTempInRange(self,serial1,value):
		deviceid = self.GetDeviceIdFromSerial(serial1)
		ret = 0
		cur = self.db.cursor()
		sql = "SELECT * FROM sensor_temp WHERE device = '%s' AND (min_temp>'%s' OR max_temp<'%s') AND alert = 1"%(deviceid,value,value)
		cur.execute(sql)
		if(cur.rowcount>0):
			ret= 1
		else:
			ret= 0
		cur.close()
		return ret
	def UpdateWarningLog(self,deviceid,content):
		cur = self.db.cursor()
		sql="INSERT INTO warning_log(device,content) VALUE ('%s','%s')" % (deviceid,content)
		try:
			cur.execute(sql)
			self.db.commit()
		except:
			self.db.rollback()
		cur.close()
	def RangeTempToString(self,serial1):
		deviceid=self.GetDeviceIdFromSerial(serial1)
		cur = self.db.cursor()
		sql = "SELECT min_temp,max_temp FROM sensor_temp WHERE device ='%s'"%deviceid
		cur.execute(sql)
		for row in cur.fetchall() :
			tmin=row[0]
			tmax=row[1]
		cur.close()
		trave = "\nNhiet do MIN: %d\nNhiet do MAX: %d"%(tmin,tmax)
		return trave
	def RangeHumiToString(self,serial1):
		deviceid=self.GetDeviceIdFromSerial(serial1)
		cur = self.db.cursor()
		sql = "SELECT min_humi,max_humi FROM sensor_humi WHERE device ='%s'"%deviceid
		cur.execute(sql)
		for row in cur.fetchall() :
			hmin=row[0]
			hmax=row[1]
		cur.close()
		trave = "\nDo am MIN: %d\nDo am MAX: %d"%(hmin,hmax)
		return trave          
	def CloseDb(self):
		self.db.close()