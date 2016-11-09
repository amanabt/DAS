set datafile separator ","
plot "output.csv" using 3:4 with p
pause 1
reread
