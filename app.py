#!/usr/bin/env python
from importlib import import_module
import os
from flask import Flask, render_template, Response,jsonify, redirect
import time
import serial
import RPi.GPIO as GPIO
import random
import datetime
import glob
import MySQLdb, sys
import math

# Raspberry Pi camera module (requires picamera package)
# from camera_pi import Camera
app = Flask(__name__)
db= MySQLdb.connect(user="root",passwd="test",db="sensordata")
cur = db.cursor()


@app.route('/')
def index():
    temp_times = []
    temp_temps = []
    temp_hums = []
    temp_lums = []
    temp_heights = []
    times = []
    temps = []
    hums = []
    lums = []
    heights = []

    # exe = cur.execute("SELECT TS, Temperature, Humidity, Luminosity, Height From Data order by ID desc limit 15;")
    exe = cur.execute("SELECT Temperature, Humidity, Luminosity, Height From Data;")
    results = cur.fetchall()
    
    for item in results:
      temp_temps.append(item[0])
      temp_hums.append(item[1])
      temp_lums.append(item[2])
      temp_heights.append(9.5-item[3]) #9.5 is the distance measured by the sonar sensor with no

    exe = cur.execute("SELECT DATE(TS) from Data;")
    results = cur.fetchall()
    for item in results:
      temp_times.append(item)
    
    length = len(temp_times)
    step = int(math.floor(length/10))

    # get average of 10 sections of all historical data
    index = 0
    tempsum = 0.0
    humsum = 0.0
    lumsum = 0.0
    heightsum = 0.0
    for i in range(length):
      if index < step:
        tempsum += temp_temps[i]
        humsum += temp_hums[i]
        lumsum += temp_lums[i]
        heightsum += temp_heights[i]
        index += 1
      else:
        # times.append(temp_times[i])
        times.append(i)
        temps.append(float(tempsum/step))
        hums.append(float(humsum/step))
        lums.append(float(lumsum/step))
        heights.append(float(heightsum/step))
        index = 0
        tempsum = 0.0
        humsum = 0.0
        lumsum = 0.0
        heightsum = 0.0

    currtemp = temp_temps[length-1]
    currhum = temp_hums[length-1]
    currlum = temp_lums[length-1]
    currheight = temp_heights[length-1]


    return render_template('home.html', times = times, temps = temps, hums = hums, lums = lums, heights = heights, currtemp =currtemp, currhum = currhum, currlum = currlum, currheight = currheight)


@app.route("/lightOn")
def light():
    response = 'Light On Succesful'
    print "light on route"
    try:

      usbCom = serial.Serial('/dev/ttyACM0',9600)
      usbCom.write('1')
    except:
      response = 'Failure'
    #return render_template('home.html')
    return redirect('/')

@app.route("/lightOff")
def lightO():
    response = "Light Off Succesful"
    print "light off route"
    try:
      usbCom = serial.Serial('/dev/ttyACM0',9600)
      usbCom.write('0')
    except:
      response = 'Failure'
    #return render_template('home.html')
    return redirect('/')

@app.route("/water")
def water():
    response = "Water Succesful"
    print "water route"
    try:
      usbCom = serial.Serial('/dev/ttyACM0',9600)
      usbCom.write('2')
    except:
      response = 'Failure'
    #return render_template('home.html')
    return redirect('/')



if __name__ == '__main__':
    try:
        app.run(host='0.0.0.0', threaded=False)
    finally:
        connection.close()


