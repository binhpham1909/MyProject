#!/usr/bin/env python  
# coding=utf-8  
import MySQLdb
import os  
import time
import datetime
from time import strftime
dateString = '%Y/%m/%d %H:%M:%S'
db = MySQLdb.connect(host="localhost", user="root", passwd="!()(!(*(", db="hbwarning")	

# At First we have to get the current CPU-Temperature with this defined function  
def getCPUtemperature():  
	res = os.popen('vcgencmd measure_temp').readline()  
	return(res.replace("temp=","").replace("'C\n","")) 

# Get Current Time
# now_string = datetime.datetime.now().strftime(dateString)
# Now we convert our value into a float number  

# Update hardware monitor
def hardware_monitor():
	cur = db.cursor()
	sql="INSERT INTO hardware(cpu_temp) VALUES ('%s')" % (getCPUtemperature())
	try:
		cur.execute(sql)
		db.commit()
	except:
		db.rollback()
	cur.close()

def print_hardware_monitor():	
	cur = db.cursor()
	cur.execute("SELECT * FROM hardware")
	print cur.rowcount
	for row in cur.fetchall() :
		cputemp = str(row[1])
		ti = str(row[3])
		print cputemp + " - " + ti
	cur.close()

hardware_monitor()
#print_hardware_monitor()
# close the connection
db.close ()
