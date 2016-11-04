#! /bin/env python3

from __future__ import division
import numpy as np
import csv
import sys
from itertools import islice

def extractor () :
	datafile = open ("test1_3_11_16", 'r', newline='\n')
	reader = csv.reader (datafile, delimiter = ',')
	datarows = list (reader)
	thermocouple_data = []
	coldJunctionData = []
	for row in datarows :
		thermocouple_data.append (int (row[0]))
		coldJunctionData.append (int (row[1]))
	
	return thermocouple_data, coldJunctionData

def average (datapoints, avg_intensity) :
	size = len (datapoints)
	averaged = []
	for idx in range (0, size, avg_intensity) :
		averaged.append (sum (datapoints [idx : idx + avg_intensity]) / avg_intensity)
	for element in averaged :
		print (int (round (element, 0)))

if __name__ == '__main__':
	thermocouple_data, coldJunctionData = extractor ()
	average (thermocouple_data, 100)
