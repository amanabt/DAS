set title "Realtime plot of the DAS data"
set xlabel "Time (us)"
set ylabel "Thermocouple Voltage (mV)"
set datafile separator ","
plot "output.csv" using 1:2 with p title "Thermocouple voltage vs Time"
pause 5
reread
