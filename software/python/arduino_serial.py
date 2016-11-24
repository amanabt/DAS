#!/bin/env python

import serial
import re
import sys
import csv

ser = serial.Serial (sys.argv[1], int (sys.argv[2]))
final = []
for idx in range (9):
	line = ser.readline()

outputfile = open ("output.csv", 'w', newline="\n") 
outwriter = csv.writer (outputfile, delimiter=',')
lc_16 = 5.0 / 65535
lc_10 = 1.1 / 1024
while True:
	line = ser.readline()
	line = str(line)
	line  = (re.sub ("'",'',(re.sub("b", '', line)))).strip()
	data = line.split(",")
	data [5] = data[5].replace("\\n","")	
	data[1] = str(int (data[1]) * lc_16 / 100)
	data[3] = str(int (data[3]) * lc_10)
	data[5] = str(int (data[5]) * lc_10)
	outwriter.writerow (data)
	print (data)

