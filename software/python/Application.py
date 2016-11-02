#!/bin/env python3

import serial
import re
import numpy as np
import matplotlib.pyplot as plt

plt.ion()

ser = serial.Serial('/dev/ttyACM0')
i = 0;
while True:                        
	line = ser.readline()
	line = str(line)
	line  = str((re.sub ("'",'',(re.sub("b", '', line)))))
	line = line.replace(' ', '')[:-2].upper()
	#print (line)
	data = line.split(",")
	data = [ int(x) for x in data ]
	print (data[0:1])

	#plt.scatter(i, data[1])
	#plt.pause(0.05)
	#++i

while True:
	plt.pause(0.05)
