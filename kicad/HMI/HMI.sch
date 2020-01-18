EESchema Schematic File Version 4
LIBS:HMI-cache
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 1
Title "HMI Board"
Date "2020-01-18"
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 "TR-9 Open Source Digital Radio"
$EndDescr
$Comp
L Connector_Generic:Conn_01x03 J?
U 1 1 5E22D06F
P 1050 1850
F 0 "J?" H 968 1525 50  0000 C CNN
F 1 "ENCODER" H 968 1616 50  0000 C CNN
F 2 "" H 1050 1850 50  0001 C CNN
F 3 "~" H 1050 1850 50  0001 C CNN
	1    1050 1850
	-1   0    0    1   
$EndComp
$Comp
L Device:R R?
U 1 1 5E22E5EF
P 1450 1500
F 0 "R?" H 1380 1454 50  0000 R CNN
F 1 "10k" H 1380 1545 50  0000 R CNN
F 2 "" V 1380 1500 50  0001 C CNN
F 3 "~" H 1450 1500 50  0001 C CNN
	1    1450 1500
	-1   0    0    1   
$EndComp
$Comp
L Device:R R?
U 1 1 5E22FFC6
P 1700 1500
F 0 "R?" H 1770 1546 50  0000 L CNN
F 1 "R" H 1770 1455 50  0000 L CNN
F 2 "" V 1630 1500 50  0001 C CNN
F 3 "~" H 1700 1500 50  0001 C CNN
	1    1700 1500
	1    0    0    -1  
$EndComp
$Comp
L Device:R R?
U 1 1 5E2302CC
P 1950 1750
F 0 "R?" V 1743 1750 50  0000 C CNN
F 1 "10k" V 1834 1750 50  0000 C CNN
F 2 "" V 1880 1750 50  0001 C CNN
F 3 "~" H 1950 1750 50  0001 C CNN
	1    1950 1750
	0    1    1    0   
$EndComp
$Comp
L Device:R R?
U 1 1 5E230501
P 1950 1950
F 0 "R?" V 2157 1950 50  0000 C CNN
F 1 "10k" V 2066 1950 50  0000 C CNN
F 2 "" V 1880 1950 50  0001 C CNN
F 3 "~" H 1950 1950 50  0001 C CNN
	1    1950 1950
	0    -1   -1   0   
$EndComp
$Comp
L Device:C C?
U 1 1 5E23085E
P 2150 2150
F 0 "C?" H 2265 2196 50  0000 L CNN
F 1 "10n" H 2265 2105 50  0000 L CNN
F 2 "" H 2188 2000 50  0001 C CNN
F 3 "~" H 2150 2150 50  0001 C CNN
	1    2150 2150
	1    0    0    -1  
$EndComp
$Comp
L Device:C C?
U 1 1 5E230D68
P 2500 2150
F 0 "C?" H 2615 2196 50  0000 L CNN
F 1 "10n" H 2615 2105 50  0000 L CNN
F 2 "" H 2538 2000 50  0001 C CNN
F 3 "~" H 2500 2150 50  0001 C CNN
	1    2500 2150
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 5E2311A2
P 1450 2400
F 0 "#PWR?" H 1450 2150 50  0001 C CNN
F 1 "GND" H 1455 2227 50  0000 C CNN
F 2 "" H 1450 2400 50  0001 C CNN
F 3 "" H 1450 2400 50  0001 C CNN
	1    1450 2400
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 5E23164C
P 2150 2400
F 0 "#PWR?" H 2150 2150 50  0001 C CNN
F 1 "GND" H 2155 2227 50  0000 C CNN
F 2 "" H 2150 2400 50  0001 C CNN
F 3 "" H 2150 2400 50  0001 C CNN
	1    2150 2400
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 5E231AC2
P 2500 2400
F 0 "#PWR?" H 2500 2150 50  0001 C CNN
F 1 "GND" H 2505 2227 50  0000 C CNN
F 2 "" H 2500 2400 50  0001 C CNN
F 3 "" H 2500 2400 50  0001 C CNN
	1    2500 2400
	1    0    0    -1  
$EndComp
$Comp
L power:+3.3V #PWR?
U 1 1 5E2324C9
P 1450 1250
F 0 "#PWR?" H 1450 1100 50  0001 C CNN
F 1 "+3.3V" H 1465 1423 50  0000 C CNN
F 2 "" H 1450 1250 50  0001 C CNN
F 3 "" H 1450 1250 50  0001 C CNN
	1    1450 1250
	1    0    0    -1  
$EndComp
Wire Wire Line
	1250 1850 1450 1850
Wire Wire Line
	1450 1850 1450 2400
Wire Wire Line
	1250 1750 1450 1750
Wire Wire Line
	1450 1750 1450 1650
Wire Wire Line
	1450 1750 1800 1750
Connection ~ 1450 1750
Wire Wire Line
	1700 1650 1700 1950
Wire Wire Line
	1700 1950 1800 1950
Wire Wire Line
	1250 1950 1700 1950
Connection ~ 1700 1950
Wire Wire Line
	1450 1350 1450 1300
Wire Wire Line
	1700 1350 1700 1300
Wire Wire Line
	1700 1300 1450 1300
Connection ~ 1450 1300
Wire Wire Line
	1450 1300 1450 1250
Wire Wire Line
	2100 1950 2150 1950
Wire Wire Line
	2150 1950 2150 2000
Wire Wire Line
	2100 1750 2500 1750
Wire Wire Line
	2500 1750 2500 2000
Wire Wire Line
	2150 2300 2150 2400
Wire Wire Line
	2500 2300 2500 2400
Text GLabel 2750 1750 2    50   Output ~ 10
ENC_A
Text GLabel 2750 1950 2    50   Output ~ 10
ENC_B
Wire Wire Line
	2750 1750 2500 1750
Connection ~ 2500 1750
Wire Wire Line
	2750 1950 2150 1950
Connection ~ 2150 1950
$Comp
L MCU_ST_STM32L0:STM32L011K4Tx U?
U 1 1 5E236893
P 2700 4500
F 0 "U?" H 2700 3411 50  0000 C CNN
F 1 "STM32L011K4Tx" H 2700 3320 50  0000 C CNN
F 2 "Package_QFP:LQFP-32_7x7mm_P0.8mm" H 2300 3600 50  0001 R CNN
F 3 "http://www.st.com/st-web-ui/static/active/en/resource/technical/document/datasheet/DM00206508.pdf" H 2700 4500 50  0001 C CNN
	1    2700 4500
	1    0    0    -1  
$EndComp
Wire Notes Line
	700  750  3300 750 
Wire Notes Line
	3300 750  3300 2900
Wire Notes Line
	3300 2900 700  2900
Wire Notes Line
	700  2900 700  750 
Text Notes 700  700  0    50   ~ 0
We are using a pot instead - so no longer needed
$Comp
L power:GND #PWR?
U 1 1 5E239F41
P 2700 5750
F 0 "#PWR?" H 2700 5500 50  0001 C CNN
F 1 "GND" H 2705 5577 50  0000 C CNN
F 2 "" H 2700 5750 50  0001 C CNN
F 3 "" H 2700 5750 50  0001 C CNN
	1    2700 5750
	1    0    0    -1  
$EndComp
$Comp
L power:+3.3V #PWR?
U 1 1 5E23A29A
P 2700 3450
F 0 "#PWR?" H 2700 3300 50  0001 C CNN
F 1 "+3.3V" H 2715 3623 50  0000 C CNN
F 2 "" H 2700 3450 50  0001 C CNN
F 3 "" H 2700 3450 50  0001 C CNN
	1    2700 3450
	1    0    0    -1  
$EndComp
$Comp
L Device:C C?
U 1 1 5E23A471
P 4550 6450
F 0 "C?" H 4665 6496 50  0000 L CNN
F 1 "100n" H 4665 6405 50  0000 L CNN
F 2 "" H 4588 6300 50  0001 C CNN
F 3 "~" H 4550 6450 50  0001 C CNN
	1    4550 6450
	1    0    0    -1  
$EndComp
$Comp
L Connector_Generic:Conn_01x04 J?
U 1 1 5E23E36E
P 4650 1850
F 0 "J?" H 4730 1842 50  0000 L CNN
F 1 "SWD" H 4730 1751 50  0000 L CNN
F 2 "" H 4650 1850 50  0001 C CNN
F 3 "~" H 4650 1850 50  0001 C CNN
	1    4650 1850
	1    0    0    -1  
$EndComp
Wire Wire Line
	4450 1750 4250 1750
Wire Wire Line
	4450 1850 4250 1850
Wire Wire Line
	4450 1950 4350 1950
Wire Wire Line
	4450 2050 4350 2050
Wire Wire Line
	4350 2050 4350 2150
Text GLabel 4250 1750 0    50   BiDi ~ 10
SWDIO
Text GLabel 4250 1850 0    50   BiDi ~ 10
SWCLK
Text GLabel 4250 1950 0    50   Input ~ 10
NRST
$Comp
L Device:R R?
U 1 1 5E24415C
P 4350 1500
F 0 "R?" H 4420 1546 50  0000 L CNN
F 1 "10k" H 4420 1455 50  0000 L CNN
F 2 "" V 4280 1500 50  0001 C CNN
F 3 "~" H 4350 1500 50  0001 C CNN
	1    4350 1500
	1    0    0    -1  
$EndComp
$Comp
L power:+3.3V #PWR?
U 1 1 5E2446B5
P 4350 1250
F 0 "#PWR?" H 4350 1100 50  0001 C CNN
F 1 "+3.3V" H 4365 1423 50  0000 C CNN
F 2 "" H 4350 1250 50  0001 C CNN
F 3 "" H 4350 1250 50  0001 C CNN
	1    4350 1250
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 5E245B8F
P 4350 2150
F 0 "#PWR?" H 4350 1900 50  0001 C CNN
F 1 "GND" H 4355 1977 50  0000 C CNN
F 2 "" H 4350 2150 50  0001 C CNN
F 3 "" H 4350 2150 50  0001 C CNN
	1    4350 2150
	1    0    0    -1  
$EndComp
Wire Wire Line
	4350 1250 4350 1350
Wire Wire Line
	4350 1650 4350 1950
Connection ~ 4350 1950
Wire Wire Line
	4350 1950 4250 1950
Wire Wire Line
	2700 5500 2700 5600
Wire Wire Line
	2600 5500 2600 5600
Wire Wire Line
	2600 5600 2700 5600
Connection ~ 2700 5600
Wire Wire Line
	2700 5600 2700 5750
Wire Wire Line
	2700 3450 2700 3500
Wire Wire Line
	2600 3600 2600 3500
Wire Wire Line
	2600 3500 2700 3500
Connection ~ 2700 3500
Wire Wire Line
	2700 3500 2700 3600
Wire Wire Line
	2800 3600 2800 3500
Wire Wire Line
	2800 3500 2700 3500
Wire Wire Line
	2200 3800 2050 3800
Text GLabel 2050 3800 0    50   Input ~ 10
NRST
Wire Wire Line
	3200 3800 3350 3800
Text GLabel 3350 3800 2    50   Input ~ 10
OSC
$Comp
L Oscillator:ASV-xxxMHz X?
U 1 1 5E24C77B
P 1600 6700
F 0 "X?" H 1944 6746 50  0000 L CNN
F 1 "ASV-4.000MHZ-EJ-T" H 1944 6655 50  0000 L CNN
F 2 "Oscillator:Oscillator_SMD_Abracon_ASV-4Pin_7.0x5.1mm" H 2300 6350 50  0001 C CNN
F 3 "http://www.abracon.com/Oscillators/ASV.pdf" H 1500 6700 50  0001 C CNN
	1    1600 6700
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 5E24DA8D
P 1600 7100
F 0 "#PWR?" H 1600 6850 50  0001 C CNN
F 1 "GND" H 1605 6927 50  0000 C CNN
F 2 "" H 1600 7100 50  0001 C CNN
F 3 "" H 1600 7100 50  0001 C CNN
	1    1600 7100
	1    0    0    -1  
$EndComp
$Comp
L power:+3.3V #PWR?
U 1 1 5E24DE58
P 1600 6250
F 0 "#PWR?" H 1600 6100 50  0001 C CNN
F 1 "+3.3V" H 1615 6423 50  0000 C CNN
F 2 "" H 1600 6250 50  0001 C CNN
F 3 "" H 1600 6250 50  0001 C CNN
	1    1600 6250
	1    0    0    -1  
$EndComp
$Comp
L Device:C C?
U 1 1 5E24E352
P 1150 6900
F 0 "C?" H 1265 6946 50  0000 L CNN
F 1 "10n" H 1265 6855 50  0000 L CNN
F 2 "" H 1188 6750 50  0001 C CNN
F 3 "~" H 1150 6900 50  0001 C CNN
	1    1150 6900
	1    0    0    -1  
$EndComp
Wire Wire Line
	1600 6250 1600 6300
Wire Wire Line
	1600 6300 1150 6300
Wire Wire Line
	1150 6300 1150 6700
Connection ~ 1600 6300
Wire Wire Line
	1600 6300 1600 6400
Wire Wire Line
	1300 6700 1150 6700
Connection ~ 1150 6700
Wire Wire Line
	1150 6700 1150 6750
Wire Wire Line
	1600 7000 1600 7100
$Comp
L power:GND #PWR?
U 1 1 5E251E2E
P 1150 7100
F 0 "#PWR?" H 1150 6850 50  0001 C CNN
F 1 "GND" H 1155 6927 50  0000 C CNN
F 2 "" H 1150 7100 50  0001 C CNN
F 3 "" H 1150 7100 50  0001 C CNN
	1    1150 7100
	1    0    0    -1  
$EndComp
Wire Wire Line
	1150 7050 1150 7100
Wire Wire Line
	1900 6700 2100 6700
Wire Wire Line
	2100 6700 2100 6600
Text GLabel 2100 6600 1    50   Input ~ 10
OSC
Wire Wire Line
	2200 5100 2050 5100
Text GLabel 2050 5100 0    50   Output ~ 10
TX
NoConn ~ 2200 4300
NoConn ~ 2200 4400
$Comp
L Switch:SW_Push SW?
U 1 1 5E259735
P 8600 5300
F 0 "SW?" V 8554 5448 50  0000 L CNN
F 1 "SW_Push" V 8645 5448 50  0000 L CNN
F 2 "" H 8600 5500 50  0001 C CNN
F 3 "~" H 8600 5500 50  0001 C CNN
	1    8600 5300
	0    1    1    0   
$EndComp
$Comp
L Switch:SW_Push SW?
U 1 1 5E25A791
P 9250 5300
F 0 "SW?" V 9204 5448 50  0000 L CNN
F 1 "SW_Push" V 9295 5448 50  0000 L CNN
F 2 "" H 9250 5500 50  0001 C CNN
F 3 "~" H 9250 5500 50  0001 C CNN
	1    9250 5300
	0    1    1    0   
$EndComp
$Comp
L Switch:SW_Push SW?
U 1 1 5E25AC26
P 9850 5300
F 0 "SW?" V 9804 5448 50  0000 L CNN
F 1 "SW_Push" V 9895 5448 50  0000 L CNN
F 2 "" H 9850 5500 50  0001 C CNN
F 3 "~" H 9850 5500 50  0001 C CNN
	1    9850 5300
	0    1    1    0   
$EndComp
$Comp
L Switch:SW_Push SW?
U 1 1 5E25D30B
P 8600 4600
F 0 "SW?" V 8554 4748 50  0000 L CNN
F 1 "SW_Push" V 8645 4748 50  0000 L CNN
F 2 "" H 8600 4800 50  0001 C CNN
F 3 "~" H 8600 4800 50  0001 C CNN
	1    8600 4600
	0    1    1    0   
$EndComp
$Comp
L Switch:SW_Push SW?
U 1 1 5E25D311
P 9250 4600
F 0 "SW?" V 9204 4748 50  0000 L CNN
F 1 "SW_Push" V 9295 4748 50  0000 L CNN
F 2 "" H 9250 4800 50  0001 C CNN
F 3 "~" H 9250 4800 50  0001 C CNN
	1    9250 4600
	0    1    1    0   
$EndComp
$Comp
L Switch:SW_Push SW?
U 1 1 5E25D317
P 9850 4600
F 0 "SW?" V 9804 4748 50  0000 L CNN
F 1 "SW_Push" V 9895 4748 50  0000 L CNN
F 2 "" H 9850 4800 50  0001 C CNN
F 3 "~" H 9850 4800 50  0001 C CNN
	1    9850 4600
	0    1    1    0   
$EndComp
$Comp
L Switch:SW_Push SW?
U 1 1 5E25EB45
P 8600 3900
F 0 "SW?" V 8554 4048 50  0000 L CNN
F 1 "SW_Push" V 8645 4048 50  0000 L CNN
F 2 "" H 8600 4100 50  0001 C CNN
F 3 "~" H 8600 4100 50  0001 C CNN
	1    8600 3900
	0    1    1    0   
$EndComp
$Comp
L Switch:SW_Push SW?
U 1 1 5E25EB4B
P 9250 3900
F 0 "SW?" V 9204 4048 50  0000 L CNN
F 1 "SW_Push" V 9295 4048 50  0000 L CNN
F 2 "" H 9250 4100 50  0001 C CNN
F 3 "~" H 9250 4100 50  0001 C CNN
	1    9250 3900
	0    1    1    0   
$EndComp
$Comp
L Switch:SW_Push SW?
U 1 1 5E25EB51
P 9850 3900
F 0 "SW?" V 9804 4048 50  0000 L CNN
F 1 "SW_Push" V 9895 4048 50  0000 L CNN
F 2 "" H 9850 4100 50  0001 C CNN
F 3 "~" H 9850 4100 50  0001 C CNN
	1    9850 3900
	0    1    1    0   
$EndComp
$Comp
L Switch:SW_Push SW?
U 1 1 5E260244
P 8600 3200
F 0 "SW?" V 8554 3348 50  0000 L CNN
F 1 "SW_Push" V 8645 3348 50  0000 L CNN
F 2 "" H 8600 3400 50  0001 C CNN
F 3 "~" H 8600 3400 50  0001 C CNN
	1    8600 3200
	0    1    1    0   
$EndComp
$Comp
L Switch:SW_Push SW?
U 1 1 5E26024A
P 9250 3200
F 0 "SW?" V 9204 3348 50  0000 L CNN
F 1 "SW_Push" V 9295 3348 50  0000 L CNN
F 2 "" H 9250 3400 50  0001 C CNN
F 3 "~" H 9250 3400 50  0001 C CNN
	1    9250 3200
	0    1    1    0   
$EndComp
$Comp
L Switch:SW_Push SW?
U 1 1 5E260250
P 9850 3200
F 0 "SW?" V 9804 3348 50  0000 L CNN
F 1 "SW_Push" V 9895 3348 50  0000 L CNN
F 2 "" H 9850 3400 50  0001 C CNN
F 3 "~" H 9850 3400 50  0001 C CNN
	1    9850 3200
	0    1    1    0   
$EndComp
Wire Wire Line
	9850 5100 9850 5000
Wire Wire Line
	9850 5000 9250 5000
Wire Wire Line
	9250 5000 9250 5100
Wire Wire Line
	9250 5000 8600 5000
Wire Wire Line
	8600 5000 8600 5100
Connection ~ 9250 5000
Connection ~ 8600 5000
Wire Wire Line
	9850 4400 9850 4300
Wire Wire Line
	9850 4300 9250 4300
Wire Wire Line
	8600 4400 8600 4300
Connection ~ 8600 4300
Wire Wire Line
	8600 4300 8050 4300
Wire Wire Line
	9250 4400 9250 4300
Connection ~ 9250 4300
Wire Wire Line
	9250 4300 8600 4300
Wire Wire Line
	9850 3700 9850 3600
Wire Wire Line
	9850 3600 9250 3600
Wire Wire Line
	8600 3700 8600 3600
Connection ~ 8600 3600
Wire Wire Line
	8600 3600 8050 3600
Wire Wire Line
	9250 3700 9250 3600
Connection ~ 9250 3600
Wire Wire Line
	9250 3600 8600 3600
Wire Wire Line
	9850 3000 9850 2900
Wire Wire Line
	9850 2900 9250 2900
Wire Wire Line
	8600 3000 8600 2900
Connection ~ 8600 2900
Wire Wire Line
	8600 2900 8050 2900
Wire Wire Line
	9250 3000 9250 2900
Connection ~ 9250 2900
Wire Wire Line
	9250 2900 8600 2900
Wire Wire Line
	8050 5000 8600 5000
Wire Wire Line
	8600 3400 8600 3500
Wire Wire Line
	8400 3500 8400 4200
Wire Wire Line
	8400 4200 8600 4200
Wire Wire Line
	8600 4200 8600 4100
Wire Wire Line
	8400 3500 8600 3500
Wire Wire Line
	8400 4200 8400 4900
Wire Wire Line
	8400 4900 8600 4900
Wire Wire Line
	8600 4900 8600 4800
Connection ~ 8400 4200
Wire Wire Line
	8400 4900 8400 5600
Wire Wire Line
	8400 5600 8600 5600
Wire Wire Line
	8600 5600 8600 5500
Connection ~ 8400 4900
Wire Wire Line
	9250 5500 9250 5600
Wire Wire Line
	9250 5600 9050 5600
Wire Wire Line
	9050 5600 9050 4900
Wire Wire Line
	9050 3500 9250 3500
Wire Wire Line
	9250 3400 9250 3500
Wire Wire Line
	9250 4800 9250 4900
Wire Wire Line
	9250 4900 9050 4900
Connection ~ 9050 4900
Wire Wire Line
	9050 4900 9050 4200
Wire Wire Line
	9250 4100 9250 4200
Wire Wire Line
	9250 4200 9050 4200
Connection ~ 9050 4200
Wire Wire Line
	9050 4200 9050 3500
Wire Wire Line
	9850 5500 9850 5600
Wire Wire Line
	9850 5600 9650 5600
Wire Wire Line
	9650 5600 9650 4900
Wire Wire Line
	9650 3500 9850 3500
Wire Wire Line
	9850 3500 9850 3400
Wire Wire Line
	9650 4200 9850 4200
Wire Wire Line
	9850 4200 9850 4100
Connection ~ 9650 4200
Wire Wire Line
	9650 4200 9650 3500
Wire Wire Line
	9650 4900 9850 4900
Wire Wire Line
	9850 4900 9850 4800
Connection ~ 9650 4900
Wire Wire Line
	9650 4900 9650 4200
Wire Wire Line
	8400 5600 8400 5850
Connection ~ 8400 5600
Wire Wire Line
	9050 5600 9050 5850
Connection ~ 9050 5600
Wire Wire Line
	9650 5600 9650 5850
Connection ~ 9650 5600
$Comp
L Connector_Generic:Conn_01x22 J?
U 1 1 5E2A885D
P 5900 4200
F 0 "J?" H 5980 4192 50  0000 L CNN
F 1 "DEM 128128A TMH-PW-N" H 5980 4101 50  0000 L CNN
F 2 "" H 5900 4200 50  0001 C CNN
F 3 "~" H 5900 4200 50  0001 C CNN
	1    5900 4200
	1    0    0    -1  
$EndComp
Text GLabel 8400 5850 3    50   BiDi ~ 10
C1
Text GLabel 9050 5850 3    50   BiDi ~ 10
C2
Text GLabel 9650 5850 3    50   BiDi ~ 10
C3
Text GLabel 8050 5000 0    50   BiDi ~ 10
R1
Text GLabel 8050 4300 0    50   BiDi ~ 10
R2
Text GLabel 8050 3600 0    50   BiDi ~ 10
R3
Text GLabel 8050 2900 0    50   BiDi ~ 10
R4
$Comp
L Switch:SW_Push SW?
U 1 1 5E2C0B13
P 8600 2500
F 0 "SW?" V 8554 2648 50  0000 L CNN
F 1 "SW_Push" V 8645 2648 50  0000 L CNN
F 2 "" H 8600 2700 50  0001 C CNN
F 3 "~" H 8600 2700 50  0001 C CNN
	1    8600 2500
	0    1    1    0   
$EndComp
$Comp
L Switch:SW_Push SW?
U 1 1 5E2C0B19
P 9250 2500
F 0 "SW?" V 9204 2648 50  0000 L CNN
F 1 "SW_Push" V 9295 2648 50  0000 L CNN
F 2 "" H 9250 2700 50  0001 C CNN
F 3 "~" H 9250 2700 50  0001 C CNN
	1    9250 2500
	0    1    1    0   
$EndComp
$Comp
L Switch:SW_Push SW?
U 1 1 5E2C0B1F
P 9850 2500
F 0 "SW?" V 9804 2648 50  0000 L CNN
F 1 "SW_Push" V 9895 2648 50  0000 L CNN
F 2 "" H 9850 2700 50  0001 C CNN
F 3 "~" H 9850 2700 50  0001 C CNN
	1    9850 2500
	0    1    1    0   
$EndComp
$Comp
L Switch:SW_Push SW?
U 1 1 5E2C0B25
P 8600 1800
F 0 "SW?" V 8554 1948 50  0000 L CNN
F 1 "SW_Push" V 8645 1948 50  0000 L CNN
F 2 "" H 8600 2000 50  0001 C CNN
F 3 "~" H 8600 2000 50  0001 C CNN
	1    8600 1800
	0    1    1    0   
$EndComp
$Comp
L Switch:SW_Push SW?
U 1 1 5E2C0B2B
P 9250 1800
F 0 "SW?" V 9204 1948 50  0000 L CNN
F 1 "SW_Push" V 9295 1948 50  0000 L CNN
F 2 "" H 9250 2000 50  0001 C CNN
F 3 "~" H 9250 2000 50  0001 C CNN
	1    9250 1800
	0    1    1    0   
$EndComp
$Comp
L Switch:SW_Push SW?
U 1 1 5E2C0B31
P 9850 1800
F 0 "SW?" V 9804 1948 50  0000 L CNN
F 1 "SW_Push" V 9895 1948 50  0000 L CNN
F 2 "" H 9850 2000 50  0001 C CNN
F 3 "~" H 9850 2000 50  0001 C CNN
	1    9850 1800
	0    1    1    0   
$EndComp
Wire Wire Line
	9850 2300 9850 2200
Wire Wire Line
	9850 2200 9250 2200
Wire Wire Line
	8600 2300 8600 2200
Connection ~ 8600 2200
Wire Wire Line
	8600 2200 8050 2200
Wire Wire Line
	9250 2300 9250 2200
Connection ~ 9250 2200
Wire Wire Line
	9250 2200 8600 2200
Wire Wire Line
	9850 1600 9850 1500
Wire Wire Line
	9850 1500 9250 1500
Wire Wire Line
	8600 1600 8600 1500
Connection ~ 8600 1500
Wire Wire Line
	8600 1500 8050 1500
Wire Wire Line
	9250 1600 9250 1500
Connection ~ 9250 1500
Wire Wire Line
	9250 1500 8600 1500
Wire Wire Line
	8600 2000 8600 2100
Wire Wire Line
	8400 2100 8400 2800
Wire Wire Line
	8400 2800 8600 2800
Wire Wire Line
	8600 2800 8600 2700
Wire Wire Line
	8400 2100 8600 2100
Wire Wire Line
	8400 2800 8400 3500
Connection ~ 8400 2800
Wire Wire Line
	9050 2100 9250 2100
Wire Wire Line
	9250 2000 9250 2100
Wire Wire Line
	9050 3500 9050 2800
Wire Wire Line
	9250 2700 9250 2800
Wire Wire Line
	9250 2800 9050 2800
Connection ~ 9050 2800
Wire Wire Line
	9050 2800 9050 2100
Wire Wire Line
	9650 2100 9850 2100
Wire Wire Line
	9850 2100 9850 2000
Wire Wire Line
	9650 2800 9850 2800
Wire Wire Line
	9850 2800 9850 2700
Connection ~ 9650 2800
Wire Wire Line
	9650 2800 9650 2100
Wire Wire Line
	9650 3500 9650 2800
Connection ~ 9650 3500
Connection ~ 9050 3500
Connection ~ 8400 3500
Text GLabel 8050 2200 0    50   BiDi ~ 10
R5
Text GLabel 8050 1500 0    50   BiDi ~ 10
R6
Wire Wire Line
	3200 3900 3350 3900
Wire Wire Line
	3200 4000 3350 4000
Wire Wire Line
	3200 4100 3350 4100
Text GLabel 3350 3900 2    50   BiDi ~ 10
C1
Text GLabel 3350 4000 2    50   BiDi ~ 10
C2
Text GLabel 3350 4100 2    50   BiDi ~ 10
C3
Wire Wire Line
	3200 4200 3350 4200
Wire Wire Line
	3200 4300 3350 4300
Wire Wire Line
	3200 4400 3350 4400
Wire Wire Line
	3200 4500 3350 4500
Text GLabel 3350 4200 2    50   BiDi ~ 10
R1
Text GLabel 3350 4300 2    50   BiDi ~ 10
R2
Text GLabel 3350 4400 2    50   BiDi ~ 10
R3
Text GLabel 3350 4500 2    50   BiDi ~ 10
R4
Wire Wire Line
	3200 4600 3350 4600
Wire Wire Line
	3200 4900 3350 4900
Text GLabel 3350 4600 2    50   BiDi ~ 10
R5
Text GLabel 3350 4900 2    50   BiDi ~ 10
R6
Wire Wire Line
	3200 4700 3350 4700
Wire Wire Line
	3200 4800 3350 4800
Text GLabel 3350 4700 2    50   Input ~ 10
ENC_A
Text GLabel 3350 4800 2    50   Input ~ 10
ENC_B
NoConn ~ 3200 5000
NoConn ~ 3200 5100
NoConn ~ 3200 5200
NoConn ~ 3200 5300
NoConn ~ 2200 5200
NoConn ~ 2200 5300
NoConn ~ 2200 4600
NoConn ~ 2200 4700
NoConn ~ 2200 4800
NoConn ~ 2200 4900
NoConn ~ 2200 5000
$Comp
L Device:Ferrite_Bead FB?
U 1 1 5E340E35
P 4200 6250
F 0 "FB?" V 3926 6250 50  0000 C CNN
F 1 "Ferrite_Bead" V 4017 6250 50  0000 C CNN
F 2 "" V 4130 6250 50  0001 C CNN
F 3 "~" H 4200 6250 50  0001 C CNN
	1    4200 6250
	0    1    1    0   
$EndComp
$Comp
L Device:C C?
U 1 1 5E36058C
P 3850 6450
F 0 "C?" H 3965 6496 50  0000 L CNN
F 1 "1u" H 3965 6405 50  0000 L CNN
F 2 "" H 3888 6300 50  0001 C CNN
F 3 "~" H 3850 6450 50  0001 C CNN
	1    3850 6450
	1    0    0    -1  
$EndComp
$Comp
L Device:C C?
U 1 1 5E36076B
P 3450 6450
F 0 "C?" H 3565 6496 50  0000 L CNN
F 1 "100n" H 3565 6405 50  0000 L CNN
F 2 "" H 3488 6300 50  0001 C CNN
F 3 "~" H 3450 6450 50  0001 C CNN
	1    3450 6450
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 5E36089D
P 5600 7250
F 0 "#PWR?" H 5600 7000 50  0001 C CNN
F 1 "GND" H 5605 7077 50  0000 C CNN
F 2 "" H 5600 7250 50  0001 C CNN
F 3 "" H 5600 7250 50  0001 C CNN
	1    5600 7250
	1    0    0    -1  
$EndComp
Wire Wire Line
	5700 7150 5600 7150
Wire Wire Line
	5600 7150 5600 7250
$Comp
L power:GND #PWR?
U 1 1 5E37ED2A
P 3850 6600
F 0 "#PWR?" H 3850 6350 50  0001 C CNN
F 1 "GND" H 3855 6427 50  0000 C CNN
F 2 "" H 3850 6600 50  0001 C CNN
F 3 "" H 3850 6600 50  0001 C CNN
	1    3850 6600
	1    0    0    -1  
$EndComp
Wire Wire Line
	3850 6250 3850 6300
Wire Wire Line
	3850 6250 3850 6150
Connection ~ 3850 6250
$Comp
L power:+3.3V #PWR?
U 1 1 5E38B8EA
P 3850 6150
F 0 "#PWR?" H 3850 6000 50  0001 C CNN
F 1 "+3.3V" H 3865 6323 50  0000 C CNN
F 2 "" H 3850 6150 50  0001 C CNN
F 3 "" H 3850 6150 50  0001 C CNN
	1    3850 6150
	1    0    0    -1  
$EndComp
Wire Wire Line
	5700 6850 5600 6850
Wire Wire Line
	5700 6950 5600 6950
Text GLabel 5450 6350 0    50   Input ~ 10
TFT_LED
Text GLabel 5450 6450 0    50   Input ~ 10
TFT_RST
Text GLabel 5450 6550 0    50   Input ~ 10
SPI1_CS
Text GLabel 5450 6650 0    50   Input ~ 10
SPI1_MOSI
Text GLabel 5450 6750 0    50   Input ~ 10
SPI1_SCK
Text GLabel 5450 7050 0    50   Input ~ 10
TX
Wire Wire Line
	5450 7050 5700 7050
Wire Wire Line
	5600 6850 5600 6950
Wire Wire Line
	5600 7150 5600 6950
Connection ~ 5600 7150
Connection ~ 5600 6950
Wire Wire Line
	5450 6350 5700 6350
Wire Wire Line
	5450 6450 5700 6450
Wire Wire Line
	5450 6550 5700 6550
Wire Wire Line
	5450 6650 5700 6650
Wire Wire Line
	5450 6750 5700 6750
Wire Wire Line
	3850 6250 4050 6250
$Comp
L power:GND #PWR?
U 1 1 5E4457D0
P 4550 6600
F 0 "#PWR?" H 4550 6350 50  0001 C CNN
F 1 "GND" H 4555 6427 50  0000 C CNN
F 2 "" H 4550 6600 50  0001 C CNN
F 3 "" H 4550 6600 50  0001 C CNN
	1    4550 6600
	1    0    0    -1  
$EndComp
Wire Wire Line
	4350 6250 4550 6250
Wire Wire Line
	4550 6300 4550 6250
Connection ~ 4550 6250
$Comp
L power:GND #PWR?
U 1 1 5E489C30
P 3450 6600
F 0 "#PWR?" H 3450 6350 50  0001 C CNN
F 1 "GND" H 3455 6427 50  0000 C CNN
F 2 "" H 3450 6600 50  0001 C CNN
F 3 "" H 3450 6600 50  0001 C CNN
	1    3450 6600
	1    0    0    -1  
$EndComp
Wire Wire Line
	3450 6300 3450 6250
Wire Wire Line
	3450 6250 3850 6250
Wire Wire Line
	4550 6250 5700 6250
$Comp
L Connector_Generic:Conn_01x10 J?
U 1 1 5E23CEE8
P 5900 6650
F 0 "J?" H 5980 6642 50  0000 L CNN
F 1 "RIBBON" H 5980 6551 50  0000 L CNN
F 2 "" H 5900 6650 50  0001 C CNN
F 3 "~" H 5900 6650 50  0001 C CNN
	1    5900 6650
	1    0    0    -1  
$EndComp
Wire Wire Line
	5700 3200 5600 3200
$Comp
L power:GND #PWR?
U 1 1 5E4BBABD
P 5600 3050
F 0 "#PWR?" H 5600 2800 50  0001 C CNN
F 1 "GND" H 5605 2877 50  0000 C CNN
F 2 "" H 5600 3050 50  0001 C CNN
F 3 "" H 5600 3050 50  0001 C CNN
	1    5600 3050
	-1   0    0    1   
$EndComp
$Comp
L power:+3.3V #PWR?
U 1 1 5E4BBF2E
P 5250 3400
F 0 "#PWR?" H 5250 3250 50  0001 C CNN
F 1 "+3.3V" H 5265 3573 50  0000 C CNN
F 2 "" H 5250 3400 50  0001 C CNN
F 3 "" H 5250 3400 50  0001 C CNN
	1    5250 3400
	0    -1   -1   0   
$EndComp
$Comp
L Device:R R?
U 1 1 5E4BC2E0
P 5500 3300
F 0 "R?" V 5707 3300 50  0000 C CNN
F 1 "0R" V 5616 3300 50  0000 C CNN
F 2 "" V 5430 3300 50  0001 C CNN
F 3 "~" H 5500 3300 50  0001 C CNN
	1    5500 3300
	0    -1   -1   0   
$EndComp
Wire Wire Line
	5650 3300 5700 3300
Wire Wire Line
	5350 3300 5300 3300
Text GLabel 5300 3300 0    50   Input ~ 10
TFT_LED
Wire Wire Line
	5250 3400 5600 3400
Wire Wire Line
	5700 3500 5600 3500
Wire Wire Line
	5600 3500 5600 3400
Connection ~ 5600 3400
Wire Wire Line
	5600 3400 5700 3400
Wire Wire Line
	5700 3600 5600 3600
Wire Wire Line
	5600 3600 5600 3500
Connection ~ 5600 3500
Wire Wire Line
	5700 3700 5300 3700
Text GLabel 5300 3700 0    50   Input ~ 10
SPI1_SCK
Wire Wire Line
	5700 3800 5300 3800
Text GLabel 5300 3800 0    50   Input ~ 10
SPI1_CS
Wire Wire Line
	5700 3900 5600 3900
Wire Wire Line
	5700 4100 5600 4100
Wire Wire Line
	5600 4100 5600 4200
Wire Wire Line
	5600 4800 5700 4800
Wire Wire Line
	5600 4700 5700 4700
Connection ~ 5600 4700
Wire Wire Line
	5600 4700 5600 4800
Wire Wire Line
	5600 4600 5700 4600
Connection ~ 5600 4600
Wire Wire Line
	5600 4600 5600 4700
Wire Wire Line
	5600 4500 5700 4500
Connection ~ 5600 4500
Wire Wire Line
	5600 4500 5600 4600
Wire Wire Line
	5600 4400 5700 4400
Connection ~ 5600 4400
Wire Wire Line
	5600 4400 5600 4500
Wire Wire Line
	5600 4300 5700 4300
Connection ~ 5600 4300
Wire Wire Line
	5600 4300 5600 4400
Wire Wire Line
	5600 4200 5700 4200
Connection ~ 5600 4200
Wire Wire Line
	5600 4200 5600 4300
Wire Wire Line
	5600 4800 5600 5000
Connection ~ 5600 4800
Wire Wire Line
	5700 4900 5300 4900
Text GLabel 5300 4900 0    50   Input ~ 10
SPI1_MOSI
Wire Wire Line
	5700 5000 5600 5000
Connection ~ 5600 5000
Wire Wire Line
	5600 5000 5600 5200
Wire Wire Line
	5600 5200 5700 5200
Connection ~ 5600 5200
Wire Wire Line
	5600 5200 5600 5300
Wire Wire Line
	5600 5300 5700 5300
Connection ~ 5600 5300
Wire Wire Line
	5600 5300 5600 5400
Wire Wire Line
	5700 5100 5300 5100
Text GLabel 5300 5100 0    50   Input ~ 10
TFT_RST
$Comp
L power:GND #PWR?
U 1 1 5E5AA2A0
P 5600 5400
F 0 "#PWR?" H 5600 5150 50  0001 C CNN
F 1 "GND" H 5605 5227 50  0000 C CNN
F 2 "" H 5600 5400 50  0001 C CNN
F 3 "" H 5600 5400 50  0001 C CNN
	1    5600 5400
	1    0    0    -1  
$EndComp
Wire Wire Line
	5600 3050 5600 3200
Text Notes 4050 4200 0    50   ~ 10
/RD (9) and /WR (8) are tied to GND,\nassuming we are using 3-wire SPI
Wire Wire Line
	5600 4100 5600 4000
Connection ~ 5600 4100
Connection ~ 5600 4000
Wire Wire Line
	5600 4000 5700 4000
Wire Wire Line
	5600 3900 5600 4000
$Comp
L power:GND #PWR?
U 1 1 5E23A966
P 4450 3650
F 0 "#PWR?" H 4450 3400 50  0001 C CNN
F 1 "GND" H 4455 3477 50  0000 C CNN
F 2 "" H 4450 3650 50  0001 C CNN
F 3 "" H 4450 3650 50  0001 C CNN
	1    4450 3650
	1    0    0    -1  
$EndComp
$Comp
L Device:C C?
U 1 1 5E23AEBE
P 4450 3500
F 0 "C?" H 4565 3546 50  0000 L CNN
F 1 "1u" H 4565 3455 50  0000 L CNN
F 2 "" H 4488 3350 50  0001 C CNN
F 3 "~" H 4450 3500 50  0001 C CNN
	1    4450 3500
	1    0    0    -1  
$EndComp
$Comp
L power:+3.3V #PWR?
U 1 1 5E23B479
P 4450 3350
F 0 "#PWR?" H 4450 3200 50  0001 C CNN
F 1 "+3.3V" H 4465 3523 50  0000 C CNN
F 2 "" H 4450 3350 50  0001 C CNN
F 3 "" H 4450 3350 50  0001 C CNN
	1    4450 3350
	1    0    0    -1  
$EndComp
$EndSCHEMATC
