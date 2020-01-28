EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 6 7
Title "Audio"
Date ""
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 "TR-9 Open Source Digital Radio"
$EndDescr
$Comp
L Mainboard-rescue:OPA340UA-linear U7
U 1 1 5DFC3191
P 1500 1700
F 0 "U7" H 1900 1850 50  0000 C CNN
F 1 "OPA340UA" H 1750 1550 50  0000 C CNN
F 2 "" H 1500 1700 50  0001 L CNN
F 3 "http://www.ti.com/lit/ds/symlink/opa340.pdf" H 1650 1850 50  0001 C CNN
	1    1500 1700
	-1   0    0    -1  
$EndComp
$Comp
L Mainboard-rescue:C_Small-device C49
U 1 1 5DFC3A53
P 2100 1800
F 0 "C49" H 2192 1846 50  0000 L CNN
F 1 "6n8" H 2192 1755 50  0000 L CNN
F 2 "" H 2100 1800 50  0001 C CNN
F 3 "~" H 2100 1800 50  0001 C CNN
	1    2100 1800
	1    0    0    -1  
$EndComp
Wire Wire Line
	1800 1600 2100 1600
Wire Wire Line
	2100 1600 2100 1700
$Comp
L Mainboard-rescue:R_Small-device R14
U 1 1 5DFC499C
P 2300 1600
F 0 "R14" V 2104 1600 50  0000 C CNN
F 1 "4k7" V 2195 1600 50  0000 C CNN
F 2 "" H 2300 1600 50  0001 C CNN
F 3 "~" H 2300 1600 50  0001 C CNN
	1    2300 1600
	0    1    1    0   
$EndComp
$Comp
L Mainboard-rescue:R_Small-device R12
U 1 1 5DFC52B7
P 2500 1400
F 0 "R12" H 2441 1354 50  0000 R CNN
F 1 "10k" H 2441 1445 50  0000 R CNN
F 2 "" H 2500 1400 50  0001 C CNN
F 3 "~" H 2500 1400 50  0001 C CNN
	1    2500 1400
	-1   0    0    1   
$EndComp
$Comp
L Mainboard-rescue:C_Small-device C47
U 1 1 5DFC5C9B
P 2700 1600
F 0 "C47" V 2800 1600 50  0000 C CNN
F 1 "1u" V 2900 1600 50  0000 C CNN
F 2 "" H 2700 1600 50  0001 C CNN
F 3 "~" H 2700 1600 50  0001 C CNN
	1    2700 1600
	0    1    1    0   
$EndComp
Wire Wire Line
	2400 1600 2500 1600
Wire Wire Line
	2500 1600 2500 1500
Wire Wire Line
	2500 1600 2600 1600
Connection ~ 2500 1600
Wire Wire Line
	2100 1600 2200 1600
Connection ~ 2100 1600
Wire Wire Line
	2800 1600 2900 1600
Text GLabel 2900 1600 2    50   Input ~ 10
MIC_AMP_IN
$Comp
L Mainboard-rescue:R_Small-device R11
U 1 1 5DFC6816
P 2700 1200
F 0 "R11" V 2504 1200 50  0000 C CNN
F 1 "47k" V 2595 1200 50  0000 C CNN
F 2 "" H 2700 1200 50  0001 C CNN
F 3 "~" H 2700 1200 50  0001 C CNN
	1    2700 1200
	0    1    1    0   
$EndComp
$Comp
L Mainboard-rescue:C_Small-device C46
U 1 1 5DFC70E9
P 2700 850
F 0 "C46" V 2471 850 50  0000 C CNN
F 1 "10u" V 2562 850 50  0000 C CNN
F 2 "" H 2700 850 50  0001 C CNN
F 3 "~" H 2700 850 50  0001 C CNN
	1    2700 850 
	0    1    1    0   
$EndComp
Wire Wire Line
	2800 850  2900 850 
Wire Wire Line
	2900 850  2900 1200
Wire Wire Line
	2900 1200 2800 1200
$Comp
L power:GND #PWR080
U 1 1 5DFC7656
P 2900 1300
F 0 "#PWR080" H 2900 1050 50  0001 C CNN
F 1 "GND" H 2905 1127 50  0000 C CNN
F 2 "" H 2900 1300 50  0001 C CNN
F 3 "" H 2900 1300 50  0001 C CNN
	1    2900 1300
	1    0    0    -1  
$EndComp
Wire Wire Line
	2900 1200 2900 1300
Connection ~ 2900 1200
Wire Wire Line
	2600 1200 2500 1200
Wire Wire Line
	2500 1200 2500 1300
Wire Wire Line
	2600 850  2500 850 
Wire Wire Line
	2500 850  2500 1200
Connection ~ 2500 1200
$Comp
L Mainboard-rescue:R_Small-device R10
U 1 1 5DFC81D3
P 2300 1200
F 0 "R10" V 2104 1200 50  0000 C CNN
F 1 "47k" V 2195 1200 50  0000 C CNN
F 2 "" H 2300 1200 50  0001 C CNN
F 3 "~" H 2300 1200 50  0001 C CNN
	1    2300 1200
	0    1    1    0   
$EndComp
Wire Wire Line
	2400 1200 2500 1200
Wire Wire Line
	2200 1200 1600 1200
Wire Wire Line
	1600 1200 1600 1400
Wire Wire Line
	1600 1200 1600 1100
Connection ~ 1600 1200
$Comp
L power:+3V3 #PWR079
U 1 1 5DFC938A
P 1600 1100
F 0 "#PWR079" H 1600 950 50  0001 C CNN
F 1 "+3V3" H 1615 1273 50  0000 C CNN
F 2 "" H 1600 1100 50  0001 C CNN
F 3 "" H 1600 1100 50  0001 C CNN
	1    1600 1100
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR083
U 1 1 5DFC97F9
P 1600 2100
F 0 "#PWR083" H 1600 1850 50  0001 C CNN
F 1 "GND" H 1605 1927 50  0000 C CNN
F 2 "" H 1600 2100 50  0001 C CNN
F 3 "" H 1600 2100 50  0001 C CNN
	1    1600 2100
	1    0    0    -1  
$EndComp
Wire Wire Line
	1600 2000 1600 2100
$Comp
L Mainboard-rescue:R_Small-device R16
U 1 1 5DFC9FAC
P 1450 2600
F 0 "R16" V 1254 2600 50  0000 C CNN
F 1 "820" V 1345 2600 50  0000 C CNN
F 2 "" H 1450 2600 50  0001 C CNN
F 3 "~" H 1450 2600 50  0001 C CNN
	1    1450 2600
	0    1    1    0   
$EndComp
Wire Wire Line
	1200 1700 1000 1700
Wire Wire Line
	1000 1700 1000 2600
$Comp
L Mainboard-rescue:R_Small-device R17
U 1 1 5DFCB1E3
P 1900 2800
F 0 "R17" H 1959 2846 50  0000 L CNN
F 1 "22" H 1959 2755 50  0000 L CNN
F 2 "" H 1900 2800 50  0001 C CNN
F 3 "~" H 1900 2800 50  0001 C CNN
	1    1900 2800
	1    0    0    -1  
$EndComp
Wire Wire Line
	1900 2600 1900 2700
$Comp
L Mainboard-rescue:C_Small-device C50
U 1 1 5DFCBE71
P 1900 3100
F 0 "C50" H 1992 3146 50  0000 L CNN
F 1 "22u" H 1992 3055 50  0000 L CNN
F 2 "" H 1900 3100 50  0001 C CNN
F 3 "~" H 1900 3100 50  0001 C CNN
	1    1900 3100
	1    0    0    -1  
$EndComp
Wire Wire Line
	1900 2900 1900 3000
$Comp
L power:GND #PWR087
U 1 1 5DFCC950
P 1900 3300
F 0 "#PWR087" H 1900 3050 50  0001 C CNN
F 1 "GND" H 1905 3127 50  0000 C CNN
F 2 "" H 1900 3300 50  0001 C CNN
F 3 "" H 1900 3300 50  0001 C CNN
	1    1900 3300
	1    0    0    -1  
$EndComp
Wire Wire Line
	1900 3200 1900 3300
Wire Wire Line
	1000 1700 900  1700
Connection ~ 1000 1700
Text GLabel 900  1700 0    50   Input ~ 10
ADC1_0
Wire Wire Line
	1800 1800 1900 1800
Wire Wire Line
	1550 2600 1650 2600
Wire Wire Line
	1900 1800 1900 2600
Connection ~ 1900 2600
Wire Wire Line
	1000 2600 1250 2600
$Comp
L power:GND #PWR084
U 1 1 5DFCFDA2
P 2100 2100
F 0 "#PWR084" H 2100 1850 50  0001 C CNN
F 1 "GND" H 2105 1927 50  0000 C CNN
F 2 "" H 2100 2100 50  0001 C CNN
F 3 "" H 2100 2100 50  0001 C CNN
	1    2100 2100
	1    0    0    -1  
$EndComp
Wire Wire Line
	2100 1900 2100 2100
$Comp
L Mainboard-rescue:Conn_01x02-conn J8
U 1 1 5DFD555B
P 7000 3000
F 0 "J8" H 7080 2992 50  0000 L CNN
F 1 "MIC_INT" H 7080 2901 50  0000 L CNN
F 2 "" H 7000 3000 50  0001 C CNN
F 3 "~" H 7000 3000 50  0001 C CNN
	1    7000 3000
	1    0    0    -1  
$EndComp
Wire Wire Line
	6200 3350 6200 3250
Connection ~ 6200 3350
Wire Wire Line
	6200 3350 6100 3350
Text GLabel 6200 3250 1    50   Input ~ 10
MIC_AMP_IN
$Comp
L Mainboard-rescue:R_Small-device R19
U 1 1 5DFD6733
P 6000 3350
F 0 "R19" V 5804 3350 50  0000 C CNN
F 1 "3k9" V 5895 3350 50  0000 C CNN
F 2 "" H 6000 3350 50  0001 C CNN
F 3 "~" H 6000 3350 50  0001 C CNN
	1    6000 3350
	0    1    1    0   
$EndComp
$Comp
L Mainboard-rescue:R_Small-device R18
U 1 1 5DFD6CC3
P 5800 3150
F 0 "R18" H 5742 3104 50  0000 R CNN
F 1 "1k" H 5742 3195 50  0000 R CNN
F 2 "" H 5800 3150 50  0001 C CNN
F 3 "~" H 5800 3150 50  0001 C CNN
	1    5800 3150
	1    0    0    1   
$EndComp
Wire Wire Line
	5800 3250 5800 3350
Wire Wire Line
	5800 3350 5900 3350
$Comp
L power:+3V3 #PWR085
U 1 1 5DFD7D34
P 5800 2950
F 0 "#PWR085" H 5800 2800 50  0001 C CNN
F 1 "+3V3" H 5815 3123 50  0000 C CNN
F 2 "" H 5800 2950 50  0001 C CNN
F 3 "" H 5800 2950 50  0001 C CNN
	1    5800 2950
	1    0    0    -1  
$EndComp
Wire Wire Line
	5800 2950 5800 3050
$Comp
L Mainboard-rescue:C_Small-device C51
U 1 1 5DFDA5FF
P 5800 3550
F 0 "C51" H 5708 3504 50  0000 R CNN
F 1 "10u" H 5708 3595 50  0000 R CNN
F 2 "" H 5800 3550 50  0001 C CNN
F 3 "~" H 5800 3550 50  0001 C CNN
	1    5800 3550
	1    0    0    1   
$EndComp
Wire Wire Line
	5800 3350 5800 3450
Connection ~ 5800 3350
Wire Wire Line
	5800 3650 5800 3750
$Comp
L power:GND #PWR089
U 1 1 5DFDC061
P 5800 3750
F 0 "#PWR089" H 5800 3500 50  0001 C CNN
F 1 "GND" H 5805 3577 50  0000 C CNN
F 2 "" H 5800 3750 50  0001 C CNN
F 3 "" H 5800 3750 50  0001 C CNN
	1    5800 3750
	1    0    0    -1  
$EndComp
$Comp
L Mainboard-rescue:SW_Push-switches SW1
U 1 1 5DFE1B21
P 10200 1600
F 0 "SW1" V 10154 1748 50  0000 L CNN
F 1 "INT_PTT_SW" V 10245 1748 50  0000 L CNN
F 2 "" H 10200 1800 50  0001 C CNN
F 3 "" H 10200 1800 50  0001 C CNN
	1    10200 1600
	0    1    1    0   
$EndComp
$Comp
L Mainboard-rescue:R_Small-device R15
U 1 1 5DFE1F5D
P 10200 1200
F 0 "R15" H 10259 1246 50  0000 L CNN
F 1 "10k" H 10259 1155 50  0000 L CNN
F 2 "" H 10200 1200 50  0001 C CNN
F 3 "~" H 10200 1200 50  0001 C CNN
	1    10200 1200
	1    0    0    -1  
$EndComp
$Comp
L power:+3V3 #PWR078
U 1 1 5DFE2751
P 10200 1000
F 0 "#PWR078" H 10200 850 50  0001 C CNN
F 1 "+3V3" H 10215 1173 50  0000 C CNN
F 2 "" H 10200 1000 50  0001 C CNN
F 3 "" H 10200 1000 50  0001 C CNN
	1    10200 1000
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR082
U 1 1 5DFE3FE9
P 10200 1900
F 0 "#PWR082" H 10200 1650 50  0001 C CNN
F 1 "GND" H 10205 1727 50  0000 C CNN
F 2 "" H 10200 1900 50  0001 C CNN
F 3 "" H 10200 1900 50  0001 C CNN
	1    10200 1900
	1    0    0    -1  
$EndComp
Wire Wire Line
	10200 1800 10200 1900
$Comp
L Mainboard-rescue:C_Small-device C48
U 1 1 5DFE4F3F
P 9850 1600
F 0 "C48" H 9758 1554 50  0000 R CNN
F 1 "100n" H 9758 1645 50  0000 R CNN
F 2 "" H 9850 1600 50  0001 C CNN
F 3 "~" H 9850 1600 50  0001 C CNN
	1    9850 1600
	-1   0    0    1   
$EndComp
$Comp
L power:GND #PWR081
U 1 1 5DFE6422
P 9850 1900
F 0 "#PWR081" H 9850 1650 50  0001 C CNN
F 1 "GND" H 9855 1727 50  0000 C CNN
F 2 "" H 9850 1900 50  0001 C CNN
F 3 "" H 9850 1900 50  0001 C CNN
	1    9850 1900
	1    0    0    -1  
$EndComp
Wire Wire Line
	9550 1400 9450 1400
Text GLabel 9450 1400 0    50   Input ~ 10
PTT
$Comp
L Mainboard-rescue:Conn_01x02-conn J10
U 1 1 5DFF9CB5
P 7000 4800
F 0 "J10" H 7080 4792 50  0000 L CNN
F 1 "SPK_INT" H 7080 4701 50  0000 L CNN
F 2 "" H 7000 4800 50  0001 C CNN
F 3 "~" H 7000 4800 50  0001 C CNN
	1    7000 4800
	1    0    0    -1  
$EndComp
$Comp
L power:+BATT #PWR093
U 1 1 5E039A23
P 2450 4900
F 0 "#PWR093" H 2450 4750 50  0001 C CNN
F 1 "+BATT" H 2465 5073 50  0000 C CNN
F 2 "" H 2450 4900 50  0001 C CNN
F 3 "" H 2450 4900 50  0001 C CNN
	1    2450 4900
	1    0    0    -1  
$EndComp
$Comp
L Mainboard-rescue:Ferrite_Bead_Small-device FB3
U 1 1 5E039F5C
P 2250 5000
F 0 "FB3" V 2013 5000 50  0000 C CNN
F 1 "TBD" V 2104 5000 50  0000 C CNN
F 2 "" V 2180 5000 50  0001 C CNN
F 3 "~" H 2250 5000 50  0001 C CNN
	1    2250 5000
	0    1    1    0   
$EndComp
Wire Wire Line
	2350 5000 2450 5000
Wire Wire Line
	2450 5000 2450 4900
$Comp
L Mainboard-rescue:C_Small-device C58
U 1 1 5E03E871
P 2450 5200
F 0 "C58" H 2542 5246 50  0000 L CNN
F 1 "10n" H 2542 5155 50  0000 L CNN
F 2 "" H 2450 5200 50  0001 C CNN
F 3 "~" H 2450 5200 50  0001 C CNN
	1    2450 5200
	1    0    0    -1  
$EndComp
Wire Wire Line
	2450 5000 2450 5100
Connection ~ 2450 5000
$Comp
L Mainboard-rescue:C_Small-device C57
U 1 1 5E0427F5
P 2050 5200
F 0 "C57" H 2142 5246 50  0000 L CNN
F 1 "1u" H 2142 5155 50  0000 L CNN
F 2 "" H 2050 5200 50  0001 C CNN
F 3 "~" H 2050 5200 50  0001 C CNN
	1    2050 5200
	1    0    0    -1  
$EndComp
$Comp
L Mainboard-rescue:C_Small-device C56
U 1 1 5E043006
P 1700 5200
F 0 "C56" H 1792 5246 50  0000 L CNN
F 1 "100n" H 1792 5155 50  0000 L CNN
F 2 "" H 1700 5200 50  0001 C CNN
F 3 "~" H 1700 5200 50  0001 C CNN
	1    1700 5200
	1    0    0    -1  
$EndComp
Wire Wire Line
	2150 5000 2050 5000
Wire Wire Line
	2050 5000 2050 5100
Wire Wire Line
	2050 5000 1700 5000
Wire Wire Line
	1700 5000 1700 5100
Connection ~ 2050 5000
Wire Wire Line
	1700 5300 1700 5400
Wire Wire Line
	1700 5400 2050 5400
Wire Wire Line
	2050 5400 2050 5300
Wire Wire Line
	2050 5400 2450 5400
Wire Wire Line
	2450 5400 2450 5300
Connection ~ 2050 5400
$Comp
L power:GND #PWR095
U 1 1 5E052B29
P 2450 5500
F 0 "#PWR095" H 2450 5250 50  0001 C CNN
F 1 "GND" H 2455 5327 50  0000 C CNN
F 2 "" H 2450 5500 50  0001 C CNN
F 3 "" H 2450 5500 50  0001 C CNN
	1    2450 5500
	1    0    0    -1  
$EndComp
Wire Wire Line
	2450 5400 2450 5500
Connection ~ 2450 5400
Text Label 1600 5000 2    50   ~ 10
PA_SUPP
Wire Wire Line
	1600 5000 1700 5000
Connection ~ 1700 5000
$Comp
L Mainboard-rescue:LED_Small-device D2
U 1 1 5E05C126
P 800 6800
F 0 "D2" V 754 6898 50  0000 L CNN
F 1 "RED" V 845 6898 50  0000 L CNN
F 2 "" V 800 6800 50  0001 C CNN
F 3 "~" V 800 6800 50  0001 C CNN
	1    800  6800
	0    -1   -1   0   
$EndComp
$Comp
L Mainboard-rescue:LED_Small-device D3
U 1 1 5E05C7DC
P 1300 6800
F 0 "D3" V 1254 6898 50  0000 L CNN
F 1 "GREEN" V 1345 6898 50  0000 L CNN
F 2 "" V 1300 6800 50  0001 C CNN
F 3 "~" V 1300 6800 50  0001 C CNN
	1    1300 6800
	0    -1   -1   0   
$EndComp
$Comp
L Mainboard-rescue:R_Small-device R27
U 1 1 5E05CD87
P 800 7100
F 0 "R27" H 859 7146 50  0000 L CNN
F 1 "470" H 859 7055 50  0000 L CNN
F 2 "" H 800 7100 50  0001 C CNN
F 3 "~" H 800 7100 50  0001 C CNN
	1    800  7100
	1    0    0    -1  
$EndComp
Wire Wire Line
	800  6900 800  7000
$Comp
L Mainboard-rescue:R_Small-device R28
U 1 1 5E061A09
P 1300 7100
F 0 "R28" H 1359 7146 50  0000 L CNN
F 1 "470" H 1359 7055 50  0000 L CNN
F 2 "" H 1300 7100 50  0001 C CNN
F 3 "~" H 1300 7100 50  0001 C CNN
	1    1300 7100
	1    0    0    -1  
$EndComp
Wire Wire Line
	1300 6900 1300 7000
Wire Wire Line
	1300 6600 1300 6700
$Comp
L power:+3V3 #PWR0103
U 1 1 5E06B1E5
P 800 6500
F 0 "#PWR0103" H 800 6350 50  0001 C CNN
F 1 "+3V3" H 815 6673 50  0000 C CNN
F 2 "" H 800 6500 50  0001 C CNN
F 3 "" H 800 6500 50  0001 C CNN
	1    800  6500
	1    0    0    -1  
$EndComp
Wire Wire Line
	800  7200 800  7300
Wire Wire Line
	1300 7200 1300 7300
Text GLabel 800  7300 3    50   Input ~ 10
LED_RED
Text GLabel 1300 7300 3    50   Input ~ 10
LED_GRN
Wire Wire Line
	800  6500 800  6600
Wire Wire Line
	800  6600 1300 6600
Connection ~ 800  6600
Wire Wire Line
	800  6600 800  6700
Wire Wire Line
	10200 1000 10200 1100
Wire Wire Line
	10200 1400 9850 1400
Wire Wire Line
	9850 1400 9850 1500
$Comp
L Mainboard-rescue:R_Small-device R13
U 1 1 5DF5BF76
P 9650 1400
F 0 "R13" V 9454 1400 50  0000 C CNN
F 1 "470" V 9545 1400 50  0000 C CNN
F 2 "" H 9650 1400 50  0001 C CNN
F 3 "~" H 9650 1400 50  0001 C CNN
	1    9650 1400
	0    1    1    0   
$EndComp
Wire Wire Line
	9750 1400 9850 1400
Connection ~ 9850 1400
Text Notes 8900 2250 0    50   ~ 10
Pushing PTT on power up forces STM32 into DFU mode
$Comp
L Mainboard-rescue:Audio-Jack-2_Switch-conn J11
U 1 1 5DF91E82
P 7000 5200
F 0 "J11" H 6712 5268 50  0000 R CNN
F 1 "SPK_EXT" H 6712 5177 50  0000 R CNN
F 2 "" H 7250 5300 50  0001 C CNN
F 3 "~" H 7250 5300 50  0001 C CNN
	1    7000 5200
	-1   0    0    -1  
$EndComp
$Comp
L Mainboard-rescue:Audio-Jack-3_2Switches-conn J9
U 1 1 5DF92AF3
P 7000 3450
F 0 "J9" H 6712 3518 50  0000 R CNN
F 1 "MIC_EXT" H 6712 3427 50  0000 R CNN
F 2 "" H 7250 3550 50  0001 C CNN
F 3 "~" H 7250 3550 50  0001 C CNN
	1    7000 3450
	-1   0    0    -1  
$EndComp
Wire Wire Line
	6500 3450 6500 2950
$Comp
L power:+5V #PWR086
U 1 1 5DFBB0A1
P 6500 2950
F 0 "#PWR086" H 6500 2800 50  0001 C CNN
F 1 "+5V" H 6515 3123 50  0000 C CNN
F 2 "" H 6500 2950 50  0001 C CNN
F 3 "" H 6500 2950 50  0001 C CNN
	1    6500 2950
	1    0    0    -1  
$EndComp
Wire Wire Line
	6600 3250 6600 3000
Wire Wire Line
	6500 3450 6800 3450
Wire Wire Line
	6600 3250 6800 3250
Wire Wire Line
	6600 3000 6800 3000
Wire Wire Line
	6800 3100 6700 3100
Wire Wire Line
	6700 3100 6700 3750
$Comp
L power:GND #PWR090
U 1 1 5DFE8E33
P 6700 3750
F 0 "#PWR090" H 6700 3500 50  0001 C CNN
F 1 "GND" H 6705 3577 50  0000 C CNN
F 2 "" H 6700 3750 50  0001 C CNN
F 3 "" H 6700 3750 50  0001 C CNN
	1    6700 3750
	1    0    0    -1  
$EndComp
NoConn ~ 6800 3550
Wire Wire Line
	6200 3350 6800 3350
Wire Wire Line
	7200 3650 7200 3750
Wire Wire Line
	7200 3750 7800 3750
$Comp
L Mainboard-rescue:R_Small-device R20
U 1 1 5E01220C
P 7800 3550
F 0 "R20" H 7859 3596 50  0000 L CNN
F 1 "10k" H 7859 3505 50  0000 L CNN
F 2 "" H 7800 3550 50  0001 C CNN
F 3 "~" H 7800 3550 50  0001 C CNN
	1    7800 3550
	1    0    0    -1  
$EndComp
$Comp
L power:+3V3 #PWR088
U 1 1 5E012809
P 7800 3350
F 0 "#PWR088" H 7800 3200 50  0001 C CNN
F 1 "+3V3" H 7815 3523 50  0000 C CNN
F 2 "" H 7800 3350 50  0001 C CNN
F 3 "" H 7800 3350 50  0001 C CNN
	1    7800 3350
	1    0    0    -1  
$EndComp
Wire Wire Line
	7800 3350 7800 3450
Wire Wire Line
	7800 3650 7800 3750
$Comp
L Mainboard-rescue:C_Small-device C52
U 1 1 5E01D57F
P 7800 3950
F 0 "C52" H 7708 3904 50  0000 R CNN
F 1 "100n" H 7708 3995 50  0000 R CNN
F 2 "" H 7800 3950 50  0001 C CNN
F 3 "~" H 7800 3950 50  0001 C CNN
	1    7800 3950
	-1   0    0    1   
$EndComp
$Comp
L power:GND #PWR091
U 1 1 5E01DBFF
P 7800 4150
F 0 "#PWR091" H 7800 3900 50  0001 C CNN
F 1 "GND" H 7805 3977 50  0000 C CNN
F 2 "" H 7800 4150 50  0001 C CNN
F 3 "" H 7800 4150 50  0001 C CNN
	1    7800 4150
	1    0    0    -1  
$EndComp
Wire Wire Line
	7800 4050 7800 4150
Wire Wire Line
	7800 3750 7800 3850
Connection ~ 7800 3750
$Comp
L Mainboard-rescue:R_Small-device R21
U 1 1 5E028C19
P 8150 3750
F 0 "R21" V 7954 3750 50  0000 C CNN
F 1 "470" V 8045 3750 50  0000 C CNN
F 2 "" H 8150 3750 50  0001 C CNN
F 3 "~" H 8150 3750 50  0001 C CNN
	1    8150 3750
	0    1    1    0   
$EndComp
Wire Wire Line
	7800 3750 8050 3750
Wire Wire Line
	8250 3750 8350 3750
Text GLabel 8350 3750 2    50   Input ~ 10
PTT
$Comp
L Mainboard-rescue:C_Small-device C54
U 1 1 5E08EDB0
P 4000 4600
F 0 "C54" V 3771 4600 50  0000 C CNN
F 1 "100n" V 3862 4600 50  0000 C CNN
F 2 "" H 4000 4600 50  0001 C CNN
F 3 "~" H 4000 4600 50  0001 C CNN
	1    4000 4600
	0    1    1    0   
$EndComp
$Comp
L Mainboard-rescue:R_Small-device R24
U 1 1 5E091FF6
P 4200 4800
F 0 "R24" H 4259 4846 50  0000 L CNN
F 1 "TBD" H 4259 4755 50  0000 L CNN
F 2 "" H 4200 4800 50  0001 C CNN
F 3 "~" H 4200 4800 50  0001 C CNN
	1    4200 4800
	1    0    0    -1  
$EndComp
Wire Wire Line
	4100 4600 4200 4600
Wire Wire Line
	4200 4600 4200 4700
$Comp
L Mainboard-rescue:R_Small-device R25
U 1 1 5E095AB1
P 4200 5200
F 0 "R25" H 4259 5246 50  0000 L CNN
F 1 "TBD" H 4259 5155 50  0000 L CNN
F 2 "" H 4200 5200 50  0001 C CNN
F 3 "~" H 4200 5200 50  0001 C CNN
	1    4200 5200
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR096
U 1 1 5E096151
P 4200 5800
F 0 "#PWR096" H 4200 5550 50  0001 C CNN
F 1 "GND" H 4205 5627 50  0000 C CNN
F 2 "" H 4200 5800 50  0001 C CNN
F 3 "" H 4200 5800 50  0001 C CNN
	1    4200 5800
	1    0    0    -1  
$EndComp
Wire Wire Line
	4200 4900 4200 5000
$Comp
L Mainboard-rescue:C_Small-device C59
U 1 1 5E09C5FA
P 4600 5200
F 0 "C59" H 4692 5246 50  0000 L CNN
F 1 "3.9n" H 4692 5155 50  0000 L CNN
F 2 "" H 4600 5200 50  0001 C CNN
F 3 "~" H 4600 5200 50  0001 C CNN
	1    4600 5200
	1    0    0    -1  
$EndComp
Wire Wire Line
	4200 5000 4600 5000
Wire Wire Line
	4600 5000 4600 5100
Connection ~ 4200 5000
Wire Wire Line
	4200 5000 4200 5100
$Comp
L power:GND #PWR097
U 1 1 5E0A05C2
P 4600 5800
F 0 "#PWR097" H 4600 5550 50  0001 C CNN
F 1 "GND" H 4605 5627 50  0000 C CNN
F 2 "" H 4600 5800 50  0001 C CNN
F 3 "" H 4600 5800 50  0001 C CNN
	1    4600 5800
	1    0    0    -1  
$EndComp
$Comp
L Mainboard-rescue:LM386-linear U8
U 1 1 5E0A39C1
P 5400 5100
F 0 "U8" H 5500 5400 50  0000 L CNN
F 1 "LM386" H 5500 5300 50  0000 L CNN
F 2 "" H 5500 5200 50  0001 C CNN
F 3 "http://www.ti.com/lit/ds/symlink/lm386.pdf" H 5600 5300 50  0001 C CNN
	1    5400 5100
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR098
U 1 1 5E0A49EB
P 5000 5800
F 0 "#PWR098" H 5000 5550 50  0001 C CNN
F 1 "GND" H 5005 5627 50  0000 C CNN
F 2 "" H 5000 5800 50  0001 C CNN
F 3 "" H 5000 5800 50  0001 C CNN
	1    5000 5800
	1    0    0    -1  
$EndComp
Wire Wire Line
	5100 5200 5000 5200
Wire Wire Line
	4600 5000 5100 5000
Connection ~ 4600 5000
NoConn ~ 5400 5400
NoConn ~ 5500 5400
NoConn ~ 5400 4800
$Comp
L power:GND #PWR099
U 1 1 5E0BB2B4
P 5300 5800
F 0 "#PWR099" H 5300 5550 50  0001 C CNN
F 1 "GND" H 5305 5627 50  0000 C CNN
F 2 "" H 5300 5800 50  0001 C CNN
F 3 "" H 5300 5800 50  0001 C CNN
	1    5300 5800
	1    0    0    -1  
$EndComp
$Comp
L Mainboard-rescue:R_Small-device R22
U 1 1 5E0CC55A
P 5100 4350
F 0 "R22" V 4904 4350 50  0000 C CNN
F 1 "4R7, 0.5W" V 4995 4350 50  0000 C CNN
F 2 "" H 5100 4350 50  0001 C CNN
F 3 "~" H 5100 4350 50  0001 C CNN
	1    5100 4350
	0    1    1    0   
$EndComp
Text Label 4850 4350 2    50   ~ 10
PA_SUPP
Wire Wire Line
	4850 4350 5000 4350
Wire Wire Line
	5200 4350 5300 4350
Wire Wire Line
	5300 4350 5300 4800
$Comp
L Mainboard-rescue:CP_Small-device C53
U 1 1 5E0D4D60
P 5500 4350
F 0 "C53" V 5725 4350 50  0000 C CNN
F 1 "220u" V 5634 4350 50  0000 C CNN
F 2 "" H 5500 4350 50  0001 C CNN
F 3 "~" H 5500 4350 50  0001 C CNN
	1    5500 4350
	0    -1   -1   0   
$EndComp
Wire Wire Line
	5300 4350 5400 4350
Connection ~ 5300 4350
Wire Wire Line
	5600 4350 5700 4350
Wire Wire Line
	5700 4350 5700 4450
$Comp
L power:GND #PWR092
U 1 1 5E0DCF52
P 5700 4450
F 0 "#PWR092" H 5700 4200 50  0001 C CNN
F 1 "GND" H 5705 4277 50  0000 C CNN
F 2 "" H 5700 4450 50  0001 C CNN
F 3 "" H 5700 4450 50  0001 C CNN
	1    5700 4450
	1    0    0    -1  
$EndComp
$Comp
L Mainboard-rescue:CP_Small-device C55
U 1 1 5E0DD2FE
P 6200 5100
F 0 "C55" V 6425 5100 50  0000 C CNN
F 1 "220u" V 6334 5100 50  0000 C CNN
F 2 "" H 6200 5100 50  0001 C CNN
F 3 "~" H 6200 5100 50  0001 C CNN
	1    6200 5100
	0    -1   -1   0   
$EndComp
Wire Wire Line
	6300 5100 6800 5100
Wire Wire Line
	6800 5200 6600 5200
Wire Wire Line
	6600 5200 6600 4800
Wire Wire Line
	6600 4800 6800 4800
Wire Wire Line
	6800 4900 6700 4900
$Comp
L power:GND #PWR0101
U 1 1 5E0EFD10
P 6700 5800
F 0 "#PWR0101" H 6700 5550 50  0001 C CNN
F 1 "GND" H 6705 5627 50  0000 C CNN
F 2 "" H 6700 5800 50  0001 C CNN
F 3 "" H 6700 5800 50  0001 C CNN
	1    6700 5800
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0102
U 1 1 5E0F018B
P 7200 5800
F 0 "#PWR0102" H 7200 5550 50  0001 C CNN
F 1 "GND" H 7205 5627 50  0000 C CNN
F 2 "" H 7200 5800 50  0001 C CNN
F 3 "" H 7200 5800 50  0001 C CNN
	1    7200 5800
	1    0    0    -1  
$EndComp
Text Notes 8050 5250 0    50   ~ 10
2.5mm MIC/PTT jack\n3.5mm SPK jack\nKenwood standard
Connection ~ 3800 4600
Wire Wire Line
	3800 4600 3900 4600
Wire Wire Line
	3800 4900 3800 5000
$Comp
L power:GND #PWR094
U 1 1 5E08BC64
P 3800 5000
F 0 "#PWR094" H 3800 4750 50  0001 C CNN
F 1 "GND" H 3805 4827 50  0000 C CNN
F 2 "" H 3800 5000 50  0001 C CNN
F 3 "" H 3800 5000 50  0001 C CNN
	1    3800 5000
	1    0    0    -1  
$EndComp
Wire Wire Line
	3800 4600 3800 4700
Wire Wire Line
	3800 4600 3700 4600
Text GLabel 3700 4600 0    50   Input ~ 10
DAC_OUT
$Comp
L Mainboard-rescue:R_Small-device R23
U 1 1 5E0886AA
P 3800 4800
F 0 "R23" H 3859 4846 50  0000 L CNN
F 1 "22k" H 3859 4755 50  0000 L CNN
F 2 "" H 3800 4800 50  0001 C CNN
F 3 "~" H 3800 4800 50  0001 C CNN
	1    3800 4800
	1    0    0    -1  
$EndComp
$Comp
L Mainboard-rescue:C_Small-device C60
U 1 1 5E1451EA
P 6000 5300
F 0 "C60" H 5908 5254 50  0000 R CNN
F 1 "47n" H 5908 5345 50  0000 R CNN
F 2 "" H 6000 5300 50  0001 C CNN
F 3 "~" H 6000 5300 50  0001 C CNN
	1    6000 5300
	1    0    0    1   
$EndComp
$Comp
L Mainboard-rescue:R_Small-device R26
U 1 1 5E1461CA
P 6000 5600
F 0 "R26" H 5942 5554 50  0000 R CNN
F 1 "10" H 5942 5645 50  0000 R CNN
F 2 "" H 6000 5600 50  0001 C CNN
F 3 "~" H 6000 5600 50  0001 C CNN
	1    6000 5600
	1    0    0    1   
$EndComp
Wire Wire Line
	6000 5400 6000 5500
Wire Wire Line
	6100 5100 6000 5100
Wire Wire Line
	6000 5100 6000 5200
$Comp
L power:GND #PWR0100
U 1 1 5E14E357
P 6000 5800
F 0 "#PWR0100" H 6000 5550 50  0001 C CNN
F 1 "GND" H 6005 5627 50  0000 C CNN
F 2 "" H 6000 5800 50  0001 C CNN
F 3 "" H 6000 5800 50  0001 C CNN
	1    6000 5800
	1    0    0    -1  
$EndComp
Wire Wire Line
	6000 5700 6000 5800
Wire Wire Line
	5700 5100 6000 5100
Connection ~ 6000 5100
Wire Wire Line
	6700 4900 6700 5800
Wire Wire Line
	7200 5300 7200 5800
Wire Wire Line
	5300 5400 5300 5800
Wire Wire Line
	5000 5200 5000 5800
Wire Wire Line
	4600 5300 4600 5800
Wire Wire Line
	4200 5300 4200 5800
$Comp
L Device:C_Small C?
U 1 1 5E069A61
P 1450 2950
F 0 "C?" V 1221 2950 50  0000 C CNN
F 1 "47n" V 1312 2950 50  0000 C CNN
F 2 "" H 1450 2950 50  0001 C CNN
F 3 "~" H 1450 2950 50  0001 C CNN
	1    1450 2950
	0    1    1    0   
$EndComp
Wire Wire Line
	1550 2950 1650 2950
Wire Wire Line
	1650 2950 1650 2600
Connection ~ 1650 2600
Wire Wire Line
	1650 2600 1900 2600
Wire Wire Line
	1350 2950 1250 2950
Wire Wire Line
	1250 2950 1250 2600
Connection ~ 1250 2600
Wire Wire Line
	1250 2600 1350 2600
Wire Wire Line
	10200 1400 10200 1300
Connection ~ 10200 1400
Wire Wire Line
	9850 1700 9850 1900
$EndSCHEMATC
