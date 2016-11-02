#!/bin/bash
import time

f = open('lm35_adc', 'r')
for line in f :
    dec = int (line, 2)
    print (int (dec))
    #time.sleep(0.01)
