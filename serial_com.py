import serial
import time
import datetime
import os
import MySQLdb, sys

serdev = '/dev/ttyACM0'
s = serial.Serial(serdev,9600,timeout = 3.0)
db= MySQLdb.connect(user="root",passwd="test",db="sensordata")
cur = db.cursor()


def wait(s):
    time.sleep(s)


def parse(char):
    d = 0
    if char == '3':
        d = float(s.read(7))
        
    if char == '4':
        d = float(s.read(4))
            
    if char == '5':
        d = float(s.read(4))
            
    if char == '6':
        d = float(s.read(4))
    return d


Temperature = 0.0
Humidity = 0.0
Luminosity = 0.0
Height = 0.0


while 1:
	for i in range(4):
		s.write(str(i+3))
		wait(1)
		p = s.read(6) + '0'
		if len(p) ==  7:
			try:
				if i == 0:
					Luminosity = float(p)
				elif i == 1 and p[3] != '.':
					Temperature = float(p[2:5])
				elif i == 3:
					Height = float(p)
				else:
					Humidity = float(p[1:5])
			except ValueError,e:
				print "error",e,"on char ", i+3
		#print p
	#print Temperature,Humidity,Luminosity, Height
	TS = datetime.datetime.fromtimestamp(time.time()).strftime('%Y-%m-%d %H:%M:%S')
    	query = "INSERT INTO Data (TS, Temperature,Humidity,Luminosity, Height) VALUES(%s,%s,%s,%s,%s)"
    	cur.execute(query,(TS,Temperature,Humidity,Luminosity, Height))
    	db.commit()

	wait(5)
	s.flushInput()
	s.flushOutput()

    
