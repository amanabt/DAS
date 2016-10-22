v 20130925 2
C 40000 40000 0 0 0 title-B.sym
T 50000 40700 9 10 1 0 0 0 1
3-adc
T 53900 40100 9 10 1 0 0 0 1
Aman Abhishek Tiwari
C 49000 45700 1 0 0 U-AD7685.sym
{
T 50500 46895 5 10 1 1 0 0 1
value=AD7685
T 51500 48095 5 10 1 1 0 0 1
refdes=U?
T 50485 47095 5 10 1 1 0 0 1
device=IC SMD
}
C 50800 49000 1 0 0 D+5V.sym
N 51000 49000 51000 48300 4
N 49100 46500 49100 45700 4
N 49100 45700 50500 45700 4
C 50200 45200 1 0 0 AGND.sym
N 51000 45700 51000 44600 4
N 51000 44600 52900 44600 4
C 51500 43600 1 90 0 capacitor-1.sym
{
T 51000 43700 5 10 1 1 90 0 1
refdes=C?
T 50600 43800 5 10 0 0 90 0 1
symversion=0.1
T 51000 44000 5 10 1 1 90 0 1
value=10uF
}
C 51000 43200 1 0 0 AGND.sym
C 50300 49000 1 0 0 A+5V.sym
N 50500 48300 50500 49000 4
C 49400 49400 1 270 0 capacitor-1.sym
{
T 49900 49200 5 10 1 1 270 0 1
refdes=100nF
T 50300 49200 5 10 0 0 270 0 1
symversion=0.1
}
C 52300 48500 1 90 0 capacitor-1.sym
{
T 51800 48700 5 10 1 1 90 0 1
refdes=100nF
T 51400 48700 5 10 0 0 90 0 1
symversion=0.1
}
C 49900 49900 1 180 0 AGND.sym
C 52300 49900 1 180 0 DGND-1.sym
C 44400 49000 1 180 1 U-AD620ARZ.sym
{
T 45295 47515 5 10 1 1 180 6 1
value=AD620
T 44995 46115 5 10 1 1 180 6 1
refdes=U?
T 45295 47715 5 10 1 1 180 6 1
device=IC SMD
}
C 46400 46000 1 180 0 A+5V.sym
N 46200 46500 46200 46000 4
C 46600 46200 1 270 0 capacitor-1.sym
{
T 47100 45700 5 10 1 1 0 0 1
refdes=0.1uF
T 47500 46000 5 10 0 0 270 0 1
symversion=0.1
}
C 46500 44800 1 0 0 AGND.sym
N 46200 46200 46800 46200 4
N 44400 48000 44200 48000 4
C 44300 47000 1 90 0 resistor-1.sym
{
T 44000 47100 5 10 1 1 90 0 1
refdes=R?
T 44000 47500 5 10 1 1 90 0 1
value=499R
}
N 44200 47900 44200 48000 4
N 44200 47000 44400 47000 4
N 42900 48500 44400 48500 4
N 42900 46500 44400 46500 4
N 43300 46500 43300 47100 4
C 46200 50000 1 180 0 AGND.sym
N 45600 48800 45600 49500 4
N 43300 49500 46200 49500 4
N 46200 48500 46200 49500 4
N 47600 47500 49100 47500 4
C 43500 47100 1 90 0 capacitor-1.sym
{
T 43000 47200 5 10 1 1 90 0 1
refdes=C?
T 42600 47300 5 10 0 0 90 0 1
symversion=0.1
T 43000 47600 5 10 1 1 90 0 1
value=10uF
}
N 43300 48500 43300 48000 4
C 42000 48400 1 0 0 resistor-1.sym
{
T 42200 48700 5 10 1 1 0 0 1
refdes=R?
T 42500 48700 5 10 1 1 0 0 1
value=1K
}
C 42000 46400 1 0 0 resistor-1.sym
{
T 42200 46700 5 10 1 1 0 0 1
refdes=R?
T 42500 46700 5 10 1 1 0 0 1
value=1K
}
L 41600 49000 41600 46000 3 0 0 2 100 100
C 40700 46700 1 0 0 Thermocouple_K.sym
{
T 40605 46885 5 10 1 1 90 0 1
device=Thermocouple
}
N 41000 48300 41000 48500 4
N 41000 48500 42000 48500 4
N 41000 46700 41000 46500 4
N 41000 46500 42000 46500 4
N 43300 49500 43300 48500 4
C 43400 45500 1 90 0 resistor-1.sym
{
T 43100 45700 5 10 1 1 90 0 1
refdes=R?
T 43100 46000 5 10 1 1 90 0 1
value=1K
}
N 43300 46400 43300 46500 4
C 43000 45000 1 0 0 AGND.sym
N 49600 48500 50500 48500 4
N 51000 48500 52400 48500 4
N 52300 47600 52400 47600 4
N 52400 47600 52400 48500 4
C 52400 47200 1 180 0 input-orcad-1.sym
{
T 53200 47100 5 10 1 1 0 0 1
value=SCK
}
C 52400 46800 1 180 0 input-orcad-1.sym
{
T 53200 46700 5 10 1 1 0 0 1
value=DMISO
}
C 52400 46400 1 180 0 input-orcad-1.sym
N 52300 47200 52400 47200 4
N 52300 46800 52400 46800 4
N 52300 46400 52400 46400 4
C 54000 45100 1 180 0 U-AD8031-1.sym
{
T 53200 44100 5 10 1 1 180 0 1
refdes=U?
}
C 53800 45800 1 180 0 AGND.sym
N 53500 45300 53500 45100 4
N 54000 44900 54100 44900 4
N 54100 44900 54100 46000 4
N 54100 46000 52800 46000 4
N 52800 46000 52800 44600 4
C 53700 44000 1 180 0 A+5V.sym
N 53500 44100 53500 44000 4
N 51300 43600 51300 43700 4
N 51300 44500 51300 44600 4
C 55000 43400 1 90 0 capacitor-1.sym
{
T 54500 43500 5 10 1 1 90 0 1
refdes=C?
T 54100 43600 5 10 0 0 90 0 1
symversion=0.1
T 54500 43800 5 10 1 1 90 0 1
value=1uF
}
N 54000 44300 55200 44300 4
C 55200 44200 1 0 0 resistor-1.sym
{
T 55400 44500 5 10 1 1 0 0 1
refdes=R?
T 55700 44500 5 10 1 1 0 0 1
value=10K
}
N 56100 44300 56500 44300 4
N 56500 44300 56500 44200 4
C 56700 44200 1 180 0 A+5V.sym
C 54500 42800 1 0 0 AGND.sym
N 54800 43400 54800 43300 4
