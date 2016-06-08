#!/usr/bin/env python  
# coding=utf-8 
import os 
import time
import database
import gmail


mail=gmail.GMAIL('nafi1.alert@gmail.com','!()(!(*(')
db=database.DB("localhost","root","!()(!(*(","hbiot")
nowtime= int(round(time.time() * 1000*1000))	#seconds
lasttime=nowtime
while (1):
	nowtime= int(round(time.time() * 1000*1000))	#seconds
	if((nowtime-lasttime)>600):
		if(db.GetSetting('email_pending')):
			rcs=db.GetRecipientEmail()
			ctn=db.GetAlertContent()
			for k, v in rcs.iteritems():
				mail.SendEmail(v,"Cảnh báo quá nhiệt tới "+k,ctn)
			db.WasSendEmail()
			lasttime=nowtime
	time.sleep(2)