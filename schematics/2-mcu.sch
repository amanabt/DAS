v 20130925 2
C 40000 40000 0 0 0 title-B.sym
T 50000 40700 9 10 1 0 0 0 1
2-MCU
C 46500 41600 1 0 0 U-ATmega328P-PU.sym
{
T 50305 49905 5 10 1 1 180 0 1
refdes=U?
T 48105 49905 5 10 1 1 180 0 1
value=ATmega328-PU
}
N 49000 50000 49000 50300 4
N 48500 50300 48500 50000 4
N 49000 41600 49000 41000 4
N 48200 41000 49000 41000 4
N 48500 41000 48500 41600 4
C 47700 41200 1 270 0 DGND-1.sym
C 48300 50300 1 0 0 A+5V.sym
C 48800 50300 1 0 0 D+5V.sym
C 54800 43100 1 0 0 A+5V.sym
C 56300 43100 1 0 0 D+5V.sym
C 54700 41500 1 0 0 AGND-1.sym
C 56300 41500 1 0 0 DGND-1.sym
C 55200 42000 1 90 0 capacitor-1.sym
{
T 54700 42200 5 10 1 1 90 0 1
refdes=100nF
T 54300 42200 5 10 0 0 90 0 1
symversion=0.1
}
C 56700 42000 1 90 0 capacitor-1.sym
{
T 56200 42200 5 10 1 1 90 0 1
refdes=100nF
T 55800 42200 5 10 0 0 90 0 1
symversion=0.1
}
N 55000 43100 55000 42900 4
N 56500 43100 56500 42900 4
C 55300 42800 1 0 0 L-INDUCTOR.sym
{
T 55500 43100 5 10 1 1 0 0 1
refdes=100uH
T 55500 43300 5 10 1 1 0 0 1
status=Inductor
}
N 55000 42900 55300 42900 4
N 56300 42900 56500 42900 4
C 46500 45400 1 180 0 interpage_from-1.sym
{
T 46100 45300 5 10 1 1 180 0 1
pages=DSCK
}
C 46500 44900 1 180 0 interpage_from-1.sym
{
T 46100 44800 5 10 1 1 180 0 1
pages=DMISO
}
C 46500 44400 1 180 0 interpage_from-1.sym
{
T 46100 44300 5 10 1 1 180 0 1
pages=DMOSI
}
C 45800 42400 1 180 0 interpage_from-1.sym
{
T 45400 42300 5 10 1 1 180 0 1
pages=DRESET
}
N 45800 42200 46500 42200 4
C 41200 45300 1 180 0 connector10-2.sym
{
T 40500 40800 5 10 1 1 180 6 1
refdes=IDC CON
}
C 41200 41100 1 0 0 interpage_from-1.sym
{
T 41700 41200 5 10 1 1 0 0 1
pages=DMOSI
}
C 41200 41900 1 270 0 D+5V.sym
C 41200 42700 1 0 0 interpage_from-1.sym
{
T 41600 42800 5 10 1 1 0 0 1
pages=DRESET
}
C 41200 43500 1 0 0 interpage_from-1.sym
{
T 41700 43600 5 10 1 1 0 0 1
pages=DSCK
}
C 41200 44300 1 0 0 interpage_from-1.sym
{
T 41700 44400 5 10 1 1 0 0 1
pages=DMISO
}
N 41200 44900 42600 44900 4
N 42600 44900 42600 42500 4
N 42600 42500 41200 42500 4
N 41200 43300 42600 43300 4
N 41200 44100 42600 44100 4
C 43100 43100 1 90 0 DGND-1.sym
N 46100 42200 46100 41900 4
C 46200 41000 1 90 0 resistor-1.sym
{
T 45900 41200 5 10 1 1 90 0 1
refdes=R?
}
C 46300 41000 1 180 0 D+5V.sym
C 46500 46900 1 180 0 interpage_from-1.sym
{
T 46100 46800 5 10 1 1 180 0 1
pages=T_AMB
}
T 53800 40100 9 10 1 0 0 0 1
Aman Abhishek Tiwari
T 53800 40400 9 10 1 0 0 0 1
1
