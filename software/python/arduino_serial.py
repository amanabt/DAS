#!/bin/env python

import serial
import re
import sys
import csv

ser = serial.Serial (sys.argv[1], int (sys.argv[2]))
final = []
for idx in range (9):
	line = ser.readline()

outputfile = open ("displacement.csv", 'w', newline="\n") 
outwriter = csv.writer (outputfile, delimiter=',')
while True:
	line = ser.readline()
	line = str(line)
	line  = (re.sub ("'",'',(re.sub("b", '', line)))).strip()
	data = line.split(",")
	data [5] = data[5].replace("\\n","")	
	outwriter.writerow (data)
	print (data)

