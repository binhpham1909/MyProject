#!/usr/bin/env python  
# coding=utf-8  
import smtplib
from email.mime.text import MIMEText

# Send email function
class GMAIL:
	def __init__(self,username,password):
		self.username=username
		self.password=password
	def SendEmail(self,mailto,subject,content):
		USERNAME = self.username
		PASSWORD = self.password
		server = smtplib.SMTP('smtp.gmail.com:587')
		MAILTO  = mailto
		msg = MIMEText(content,'plain')
		msg['Subject'] = subject
		msg['From'] = USERNAME
		msg['To'] = MAILTO
		server.ehlo_or_helo_if_needed()
		server.starttls()
		server.ehlo_or_helo_if_needed()
		server.login(USERNAME,PASSWORD)
		server.sendmail(USERNAME, MAILTO, msg.as_string())
		server.quit()