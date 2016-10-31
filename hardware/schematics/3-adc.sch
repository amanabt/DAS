v 20130925 2
C 40000 40000 0 0 0 title-B.sym
T 50000 40700 9 10 1 0 0 0 1
SENSORS
T 53900 40100 9 10 1 0 0 0 1
Aman Abhishek Tiwari
C 49000 45700 1 0 0 U-AD7685.sym
{
T 50500 46895 5 10 1 1 0 0 1
value=AD7685
T 51500 48095 5 10 1 1 0 0 1
refdes=U2
T 50485 47095 5 10 1 1 0 0 1
device=IC SMD
T 49000 45700 5 10 0 0 0 0 1
footprint=MSOP-10_DIP_Breakout
}
N 51000 49000 51000 48300 4
N 49000 45600 49000 46500 4
N 49000 45600 50500 45600 4
C 50200 45000 1 0 0 AGND.sym
N 51000 45700 51000 44100 4
N 51000 44100 51400 44100 4
C 50300 49000 1 0 0 A+5V.sym
N 50500 48300 50500 49000 4
C 49400 49500 1 270 0 capacitor-1.sym
{
T 49100 48900 5 10 1 1 90 0 1
refdes=C9
T 50300 49300 5 10 0 0 270 0 1
symversion=0.1
T 49300 48800 5 10 1 1 90 0 1
value=100nF
T 49400 49500 5 10 0 1 0 0 1
footprint=CAPR-254P-254W-381L-533H__Kemet_C315
}
C 52300 48600 1 90 0 capacitor-1.sym
{
T 52700 48900 5 10 1 1 90 0 1
refdes=C10
T 51400 48800 5 10 0 0 90 0 1
symversion=0.1
T 52500 48800 5 10 1 1 90 0 1
value=100nF
T 52300 48600 5 10 0 1 0 0 1
footprint=CAPR-254P-254W-381L-533H__Kemet_C315
}
C 49900 50100 1 180 0 AGND.sym
C 44400 49000 1 180 1 U-AD620ARZ.sym
{
T 45295 47515 5 10 1 1 180 6 1
value=AD620
T 44995 46115 5 10 1 1 180 6 1
refdes=U1
T 45295 47715 5 10 1 1 180 6 1
device=IC SMD
T 44400 49000 5 10 0 0 0 0 1
footprint=SOIC8-Breakout
}
C 46400 46000 1 180 0 A+5V.sym
N 46200 46500 46200 46000 4
C 46600 46100 1 270 0 capacitor-1.sym
{
T 47100 45600 5 10 1 1 0 0 1
refdes=C8
T 47500 45900 5 10 0 0 270 0 1
symversion=0.1
T 46600 46100 5 10 0 1 0 0 1
footprint=CAPR-254P-254W-381L-533H__Kemet_C315
T 47000 45300 5 10 1 1 0 0 1
value=100nF
}
C 46500 44800 1 0 0 AGND.sym
N 46200 46200 46800 46200 4
N 44400 48000 44200 48000 4
C 44300 47000 1 90 0 resistor-1.sym
{
T 44000 47100 5 10 1 1 90 0 1
refdes=R5
T 44000 47500 5 10 1 1 90 0 1
value=499R
T 44300 47000 5 10 0 1 0 0 1
footprint=RES-635P-340L-190D__Yageo-MFR-12
}
N 44200 47900 44200 48000 4
N 44200 47000 44400 47000 4
N 42900 48500 44400 48500 4
N 43200 46500 44400 46500 4
N 43300 46500 43300 47100 4
C 45900 49400 1 180 0 AGND.sym
N 45600 48800 45600 48900 4
N 46200 48500 46200 48900 4
N 47600 47500 49100 47500 4
N 43300 48000 43300 48500 4
C 42000 48400 1 0 0 resistor-1.sym
{
T 42200 48700 5 10 1 1 0 0 1
refdes=R3
T 42500 48700 5 10 1 1 0 0 1
value=10K
T 42000 48400 5 10 0 1 0 0 1
footprint=RES-635P-340L-190D__Yageo-MFR-12
}
C 42300 46400 1 0 0 resistor-1.sym
{
T 42500 46700 5 10 1 1 0 0 1
refdes=R4
T 42800 46700 5 10 1 1 0 0 1
value=10K
T 42300 46400 5 10 0 1 0 0 1
footprint=RES-635P-340L-190D__Yageo-MFR-12
}
L 41600 49000 41600 46000 3 0 0 2 100 100
C 40700 46700 1 0 0 Thermocouple_K.sym
{
T 40605 46885 5 10 1 1 90 0 1
device=Thermocouple
T 40700 46700 5 10 0 1 0 0 1
refdes=T1
}
N 41000 48300 41000 48500 4
N 41000 48500 42000 48500 4
N 41000 46700 41000 46500 4
N 41000 46500 42300 46500 4
C 42300 45500 1 90 0 resistor-1.sym
{
T 42000 45600 5 10 1 1 90 0 1
refdes=R6
T 42000 46000 5 10 1 1 90 0 1
value=1M
T 42300 45500 5 10 0 1 0 0 1
footprint=RES-635P-340L-190D__Yageo-MFR-12
}
C 41900 44900 1 0 0 AGND.sym
N 49600 48500 50500 48500 4
N 51000 48500 52400 48500 4
N 52300 47600 52400 47600 4
N 52400 47600 52400 48500 4
N 52300 46800 54500 46800 4
N 52300 46400 54800 46400 4
C 52200 45100 1 180 0 AGND.sym
N 51900 44600 51900 44500 4
N 52400 44300 52600 44300 4
N 52600 44300 52600 45500 4
N 52600 45500 51300 45500 4
N 51300 45500 51300 44100 4
N 51900 43700 51900 43200 4
N 52400 43900 53000 43900 4
N 46800 45200 46800 45300 4
N 46800 46100 46800 46200 4
N 49600 49500 49600 49600 4
N 49600 48600 49600 48500 4
N 52100 48500 52100 48600 4
N 49000 46500 49100 46500 4
N 50500 45500 50500 45700 4
C 52400 44500 1 180 0 lm324-1.sym
{
T 52200 43600 5 10 1 1 180 0 1
refdes=U3
T 52400 44500 5 10 0 1 0 0 1
footprint=DIP-14-300
}
N 55000 43700 55500 43700 4
N 55500 43600 55500 43800 4
C 55300 44700 1 270 0 capacitor-1.sym
{
T 55800 44200 5 10 1 1 0 0 1
refdes=C11
T 56200 44500 5 10 0 0 270 0 1
symversion=0.1
T 55300 44700 5 10 0 1 0 0 1
footprint=CAPR-254P-254W-381L-533H__Kemet_C315
T 55700 43900 5 10 1 1 0 0 1
value=100nF
}
N 54000 44800 54000 44900 4
N 54000 44900 55500 44900 4
N 55500 44900 55500 44700 4
C 55000 45500 1 180 0 AGND.sym
N 54700 44900 54700 45000 4
C 55000 44800 1 180 0 U-REF02-DIP8.sym
{
T 54505 43405 5 10 1 1 180 0 1
value=REF02AP
T 54505 43205 5 10 1 1 180 0 1
refdes=U4
T 55000 44800 5 10 0 1 0 0 1
footprint=DIP-8-300
}
C 50800 49000 1 0 0 A+5V.sym
C 52400 50100 1 180 0 AGND.sym
N 52100 49600 52100 49500 4
C 53700 49500 1 270 0 connector4-1.sym
{
T 55100 49500 5 10 1 1 270 0 1
refdes=CONN1
T 53700 49500 5 10 0 1 0 0 1
footprint=CON_HDR-200P-4C-1R-4N__JST_B4B-PH-K
}
N 52300 47200 54200 47200 4
N 54200 47200 54200 47800 4
N 54500 46800 54500 47800 4
N 54800 46400 54800 47800 4
C 53400 47800 1 270 0 AGND.sym
N 53900 47800 53900 47500 4
C 46000 48900 1 0 0 A-5V.sym
C 55700 43600 1 180 0 9V-plus-1.sym
C 52200 43300 1 270 0 capacitor-1.sym
{
T 52700 42800 5 10 1 1 0 0 1
refdes=C12
T 53100 43100 5 10 0 0 270 0 1
symversion=0.1
T 52200 43300 5 10 0 1 0 0 1
footprint=CAPR-254P-254W-381L-533H__Kemet_C315
T 52600 42500 5 10 1 1 0 0 1
value=100nF
}
N 51900 43400 52400 43400 4
N 52400 43400 52400 43300 4
C 52100 41800 1 0 0 AGND.sym
N 52400 42300 52400 42400 4
C 52100 43200 1 180 0 9V-plus-1.sym
C 47100 48700 1 90 0 capacitor-1.sym
{
T 47500 49200 5 10 1 1 180 0 1
refdes=C13
T 46200 48900 5 10 0 0 90 0 1
symversion=0.1
T 47100 48700 5 10 0 1 180 0 1
footprint=CAPR-254P-254W-381L-533H__Kemet_C315
T 47600 49500 5 10 1 1 180 0 1
value=100nF
}
C 47200 49900 1 180 0 AGND.sym
N 46900 49600 46900 49400 4
N 46900 48600 46200 48600 4
N 46900 48600 46900 48700 4
N 42200 45400 42200 45500 4
N 42200 46400 42200 46500 4
C 43000 44900 1 0 0 AGND.sym
N 43300 45400 43300 45500 4
C 43600 50100 1 180 0 AGND.sym
N 43300 49600 43300 49500 4
N 43300 48500 43300 48600 4
N 43300 46400 43300 46500 4
C 43100 48000 1 270 0 capacitor-1.sym
{
T 42800 47600 5 10 1 1 0 0 1
refdes=C7
T 44000 47800 5 10 0 0 270 0 1
symversion=0.1
T 43100 48000 5 10 0 1 0 0 1
footprint=CAPR-254P-254W-381L-533H__Kemet_C315
T 42700 47300 5 10 1 1 0 0 1
value=330nF
}
C 43100 46400 1 270 0 capacitor-1.sym
{
T 43600 45900 5 10 1 1 0 0 1
refdes=C15
T 44000 46200 5 10 0 0 270 0 1
symversion=0.1
T 43100 46400 5 10 0 1 0 0 1
footprint=CAPR-254P-254W-381L-533H__Kemet_C315
T 43500 45600 5 10 1 1 0 0 1
value=330nF
}
C 43100 49500 1 270 0 capacitor-1.sym
{
T 43600 49000 5 10 1 1 0 0 1
refdes=C14
T 44000 49300 5 10 0 0 270 0 1
symversion=0.1
T 43100 49500 5 10 0 1 0 0 1
footprint=CAPR-254P-254W-381L-533H__Kemet_C315
T 43500 48700 5 10 1 1 0 0 1
value=330nF
}
T 53800 40400 9 10 1 0 0 0 1
1
T 50000 40400 9 10 1 0 0 0 1
3-adc
