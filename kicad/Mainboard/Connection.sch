EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 7 7
Title "Connections"
Date ""
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 "TR-9 Open Source Digital Radio"
$EndDescr
$Comp
L Mainboard-rescue:Conn_01x04-conn J12
U 1 1 5E07BAA3
P 1750 1200
F 0 "J12" H 1830 1192 50  0000 L CNN
F 1 "GPS_CONN" H 1830 1101 50  0000 L CNN
F 2 "" H 1750 1200 50  0001 C CNN
F 3 "~" H 1750 1200 50  0001 C CNN
	1    1750 1200
	1    0    0    -1  
$EndComp
$Comp
L power:+5V #PWR0104
U 1 1 5E07BDC5
P 1450 1000
F 0 "#PWR0104" H 1450 850 50  0001 C CNN
F 1 "+5V" H 1465 1173 50  0000 C CNN
F 2 "" H 1450 1000 50  0001 C CNN
F 3 "" H 1450 1000 50  0001 C CNN
	1    1450 1000
	1    0    0    -1  
$EndComp
Wire Wire Line
	1450 1000 1450 1100
Wire Wire Line
	1450 1100 1550 1100
$Comp
L power:GND #PWR0106
U 1 1 5E07C3FA
P 1450 1500
F 0 "#PWR0106" H 1450 1250 50  0001 C CNN
F 1 "GND" H 1455 1327 50  0000 C CNN
F 2 "" H 1450 1500 50  0001 C CNN
F 3 "" H 1450 1500 50  0001 C CNN
	1    1450 1500
	1    0    0    -1  
$EndComp
Wire Wire Line
	1450 1500 1450 1400
Wire Wire Line
	1450 1400 1550 1400
Wire Wire Line
	1550 1200 1450 1200
Wire Wire Line
	1550 1300 1450 1300
Text GLabel 1450 1200 0    50   Input ~ 10
USART1_TX
Text GLabel 1450 1300 0    50   Input ~ 10
USART1_RX
$Comp
L Mainboard-rescue:Conn_01x03-conn J13
U 1 1 5E07CB74
P 3200 1300
F 0 "J13" H 3280 1342 50  0000 L CNN
F 1 "DBG_CONN" H 3280 1251 50  0000 L CNN
F 2 "" H 3200 1300 50  0001 C CNN
F 3 "~" H 3200 1300 50  0001 C CNN
	1    3200 1300
	1    0    0    -1  
$EndComp
Text GLabel 2900 1200 0    50   Input ~ 10
USART2_TX
Text GLabel 2900 1300 0    50   Input ~ 10
USART2_RX
$Comp
L power:GND #PWR0107
U 1 1 5E07D3F6
P 2900 1500
F 0 "#PWR0107" H 2900 1250 50  0001 C CNN
F 1 "GND" H 2905 1327 50  0000 C CNN
F 2 "" H 2900 1500 50  0001 C CNN
F 3 "" H 2900 1500 50  0001 C CNN
	1    2900 1500
	1    0    0    -1  
$EndComp
Wire Wire Line
	2900 1500 2900 1400
Wire Wire Line
	2900 1400 3000 1400
Wire Wire Line
	2900 1300 3000 1300
Wire Wire Line
	2900 1200 3000 1200
$Comp
L Mainboard-rescue:Micro_SD_Card-conn J15
U 1 1 5E0856B5
P 3350 2900
F 0 "J15" H 3300 3617 50  0000 C CNN
F 1 "Micro_SD_Card" H 3300 3526 50  0000 C CNN
F 2 "" H 4500 3200 50  0001 C CNN
F 3 "http://katalog.we-online.de/em/datasheet/693072010801.pdf" H 3350 2900 50  0001 C CNN
	1    3350 2900
	1    0    0    -1  
$EndComp
Wire Wire Line
	2450 2900 2350 2900
Wire Wire Line
	2350 2900 2350 2500
Wire Wire Line
	2450 3100 2350 3100
$Comp
L power:GND #PWR0114
U 1 1 5E08619F
P 2350 3600
F 0 "#PWR0114" H 2350 3350 50  0001 C CNN
F 1 "GND" H 2355 3427 50  0000 C CNN
F 2 "" H 2350 3600 50  0001 C CNN
F 3 "" H 2350 3600 50  0001 C CNN
	1    2350 3600
	1    0    0    -1  
$EndComp
Wire Wire Line
	2350 3100 2350 3600
$Comp
L power:GND #PWR0115
U 1 1 5E086C12
P 4250 3600
F 0 "#PWR0115" H 4250 3350 50  0001 C CNN
F 1 "GND" H 4255 3427 50  0000 C CNN
F 2 "" H 4250 3600 50  0001 C CNN
F 3 "" H 4250 3600 50  0001 C CNN
	1    4250 3600
	1    0    0    -1  
$EndComp
Wire Wire Line
	4150 3500 4250 3500
Wire Wire Line
	4250 3500 4250 3600
$Comp
L power:+3V3 #PWR0109
U 1 1 5E087582
P 2350 2500
F 0 "#PWR0109" H 2350 2350 50  0001 C CNN
F 1 "+3V3" H 2365 2673 50  0000 C CNN
F 2 "" H 2350 2500 50  0001 C CNN
F 3 "" H 2350 2500 50  0001 C CNN
	1    2350 2500
	1    0    0    -1  
$EndComp
Text GLabel 2250 2600 0    50   Input ~ 10
SD_D2
Text GLabel 2250 2700 0    50   Input ~ 10
SD_D3
Text GLabel 2250 2800 0    50   Input ~ 10
SD_CMD
Text GLabel 2250 3000 0    50   Input ~ 10
SD_CLK
Text GLabel 2250 3200 0    50   Input ~ 10
SD_D0
Text GLabel 2250 3300 0    50   Input ~ 10
SD_D1
Wire Wire Line
	2250 3300 2450 3300
Wire Wire Line
	2250 3200 2450 3200
Wire Wire Line
	2250 3000 2450 3000
Wire Wire Line
	2250 2800 2450 2800
Wire Wire Line
	2250 2700 2450 2700
Wire Wire Line
	2250 2600 2450 2600
Text GLabel 5950 3450 0    50   Input ~ 10
I2C1_SDA
Text GLabel 5950 3350 0    50   Input ~ 10
I2C1_SCL
Text Notes 6050 3850 0    50   ~ 10
ADDR 0x6B AG, 0x1E M
Text Notes 2200 850  0    50   ~ 10
TODO: Replace GPS connector
$Comp
L Sensor_Motion:LSM9DS1 U?
U 1 1 5E067250
P 7750 3750
F 0 "U?" H 7750 3800 50  0000 C CNN
F 1 "LSM9DS1" H 7750 3700 50  0000 C CNN
F 2 "Package_LGA:LGA-24L_3x3.5mm_P0.43mm" H 9250 4500 50  0001 C CNN
F 3 "http://www.st.com/content/ccc/resource/technical/document/datasheet/1e/3f/2a/d6/25/eb/48/46/DM00103319.pdf/files/DM00103319.pdf/jcr:content/translations/en.DM00103319.pdf" H 7750 3850 50  0001 C CNN
	1    7750 3750
	1    0    0    -1  
$EndComp
$Comp
L power:+3V3 #PWR?
U 1 1 5E06CB2E
P 7750 2750
F 0 "#PWR?" H 7750 2600 50  0001 C CNN
F 1 "+3V3" H 7765 2923 50  0000 C CNN
F 2 "" H 7750 2750 50  0001 C CNN
F 3 "" H 7750 2750 50  0001 C CNN
	1    7750 2750
	1    0    0    -1  
$EndComp
Wire Wire Line
	7750 2750 7750 2850
Wire Wire Line
	7750 2850 7850 2850
Wire Wire Line
	7850 2850 7850 2950
Connection ~ 7750 2850
Wire Wire Line
	7750 2850 7750 2950
Wire Wire Line
	7850 2850 8050 2850
Wire Wire Line
	8050 2850 8050 2950
Connection ~ 7850 2850
Wire Wire Line
	8050 2850 8150 2850
Wire Wire Line
	8150 2850 8150 2950
Connection ~ 8050 2850
$Comp
L power:GND #PWR?
U 1 1 5E06F5C7
P 7650 4750
F 0 "#PWR?" H 7650 4500 50  0001 C CNN
F 1 "GND" H 7655 4577 50  0000 C CNN
F 2 "" H 7650 4750 50  0001 C CNN
F 3 "" H 7650 4750 50  0001 C CNN
	1    7650 4750
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 5E06FFF0
P 7850 4750
F 0 "#PWR?" H 7850 4500 50  0001 C CNN
F 1 "GND" H 7855 4577 50  0000 C CNN
F 2 "" H 7850 4750 50  0001 C CNN
F 3 "" H 7850 4750 50  0001 C CNN
	1    7850 4750
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 5E070F61
P 8550 4750
F 0 "#PWR?" H 8550 4500 50  0001 C CNN
F 1 "GND" H 8555 4577 50  0000 C CNN
F 2 "" H 8550 4750 50  0001 C CNN
F 3 "" H 8550 4750 50  0001 C CNN
	1    8550 4750
	1    0    0    -1  
$EndComp
Wire Wire Line
	8450 3850 8550 3850
Wire Wire Line
	8550 3850 8550 3950
Wire Wire Line
	8450 4250 8550 4250
Connection ~ 8550 4250
Wire Wire Line
	8450 4150 8550 4150
Connection ~ 8550 4150
Wire Wire Line
	8550 4150 8550 4250
Wire Wire Line
	8450 4050 8550 4050
Connection ~ 8550 4050
Wire Wire Line
	8550 4050 8550 4150
Wire Wire Line
	8450 3950 8550 3950
Connection ~ 8550 3950
Wire Wire Line
	8550 3950 8550 4050
$Comp
L Device:R_Small R?
U 1 1 5E07451A
P 6850 3150
F 0 "R?" H 6909 3196 50  0000 L CNN
F 1 "4k7" H 6909 3105 50  0000 L CNN
F 2 "" H 6850 3150 50  0001 C CNN
F 3 "~" H 6850 3150 50  0001 C CNN
	1    6850 3150
	1    0    0    -1  
$EndComp
$Comp
L Device:R_Small R?
U 1 1 5E075066
P 6600 3150
F 0 "R?" H 6659 3196 50  0000 L CNN
F 1 "4k7" H 6659 3105 50  0000 L CNN
F 2 "" H 6600 3150 50  0001 C CNN
F 3 "~" H 6600 3150 50  0001 C CNN
	1    6600 3150
	1    0    0    -1  
$EndComp
Wire Wire Line
	6600 3350 6600 3250
Wire Wire Line
	6600 3350 7050 3350
Connection ~ 6600 3350
Wire Wire Line
	6850 3450 6850 3250
Wire Wire Line
	6850 3450 7050 3450
Connection ~ 6850 3450
$Comp
L power:+3V3 #PWR?
U 1 1 5E0788C1
P 6850 2950
F 0 "#PWR?" H 6850 2800 50  0001 C CNN
F 1 "+3V3" H 6865 3123 50  0000 C CNN
F 2 "" H 6850 2950 50  0001 C CNN
F 3 "" H 6850 2950 50  0001 C CNN
	1    6850 2950
	1    0    0    -1  
$EndComp
$Comp
L power:+3V3 #PWR?
U 1 1 5E078F90
P 6600 2950
F 0 "#PWR?" H 6600 2800 50  0001 C CNN
F 1 "+3V3" H 6615 3123 50  0000 C CNN
F 2 "" H 6600 2950 50  0001 C CNN
F 3 "" H 6600 2950 50  0001 C CNN
	1    6600 2950
	1    0    0    -1  
$EndComp
Wire Wire Line
	6600 2950 6600 3050
Wire Wire Line
	6850 2950 6850 3050
$Comp
L Device:C_Small C?
U 1 1 5E07AC5C
P 6850 4550
F 0 "C?" H 6942 4596 50  0000 L CNN
F 1 "100n" H 6942 4505 50  0000 L CNN
F 2 "" H 6850 4550 50  0001 C CNN
F 3 "~" H 6850 4550 50  0001 C CNN
	1    6850 4550
	1    0    0    -1  
$EndComp
$Comp
L Device:C_Small C?
U 1 1 5E07BBD5
P 6500 4550
F 0 "C?" H 6592 4596 50  0000 L CNN
F 1 "10n" H 6592 4505 50  0000 L CNN
F 2 "" H 6500 4550 50  0001 C CNN
F 3 "~" H 6500 4550 50  0001 C CNN
	1    6500 4550
	1    0    0    -1  
$EndComp
Wire Wire Line
	7050 4250 6500 4250
Wire Wire Line
	6500 4250 6500 4450
Wire Wire Line
	7050 4350 6850 4350
Wire Wire Line
	6850 4350 6850 4450
$Comp
L power:GND #PWR?
U 1 1 5E07DD63
P 6850 4750
F 0 "#PWR?" H 6850 4500 50  0001 C CNN
F 1 "GND" H 6855 4577 50  0000 C CNN
F 2 "" H 6850 4750 50  0001 C CNN
F 3 "" H 6850 4750 50  0001 C CNN
	1    6850 4750
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 5E07E2EE
P 6500 4750
F 0 "#PWR?" H 6500 4500 50  0001 C CNN
F 1 "GND" H 6505 4577 50  0000 C CNN
F 2 "" H 6500 4750 50  0001 C CNN
F 3 "" H 6500 4750 50  0001 C CNN
	1    6500 4750
	1    0    0    -1  
$EndComp
Wire Wire Line
	6500 4650 6500 4750
Wire Wire Line
	6850 4650 6850 4750
Wire Wire Line
	7650 4550 7650 4750
Wire Wire Line
	7850 4550 7850 4750
Wire Wire Line
	8550 4250 8550 4750
$Comp
L Device:R_Small R?
U 1 1 5E085D7F
P 6300 3150
F 0 "R?" H 6359 3196 50  0000 L CNN
F 1 "10k" H 6359 3105 50  0000 L CNN
F 2 "" H 6300 3150 50  0001 C CNN
F 3 "~" H 6300 3150 50  0001 C CNN
	1    6300 3150
	1    0    0    -1  
$EndComp
$Comp
L Device:R_Small R?
U 1 1 5E086057
P 8550 3150
F 0 "R?" H 8609 3196 50  0000 L CNN
F 1 "10k" H 8609 3105 50  0000 L CNN
F 2 "" H 8550 3150 50  0001 C CNN
F 3 "~" H 8550 3150 50  0001 C CNN
	1    8550 3150
	1    0    0    -1  
$EndComp
Wire Wire Line
	7050 3650 6300 3650
Wire Wire Line
	6300 3650 6300 3250
Wire Wire Line
	8450 3350 8550 3350
Wire Wire Line
	8550 3350 8550 3250
$Comp
L power:+3V3 #PWR?
U 1 1 5E089278
P 6300 2950
F 0 "#PWR?" H 6300 2800 50  0001 C CNN
F 1 "+3V3" H 6315 3123 50  0000 C CNN
F 2 "" H 6300 2950 50  0001 C CNN
F 3 "" H 6300 2950 50  0001 C CNN
	1    6300 2950
	1    0    0    -1  
$EndComp
Wire Wire Line
	6300 2950 6300 3050
$Comp
L power:+3V3 #PWR?
U 1 1 5E08A7A1
P 8550 2950
F 0 "#PWR?" H 8550 2800 50  0001 C CNN
F 1 "+3V3" H 8565 3123 50  0000 C CNN
F 2 "" H 8550 2950 50  0001 C CNN
F 3 "" H 8550 2950 50  0001 C CNN
	1    8550 2950
	1    0    0    -1  
$EndComp
Wire Wire Line
	8550 2950 8550 3050
NoConn ~ 7050 4050
NoConn ~ 7050 3950
NoConn ~ 7050 3850
NoConn ~ 8450 3650
NoConn ~ 8450 3550
$Comp
L Device:R_Small R?
U 1 1 5E0918A1
P 8800 3150
F 0 "R?" H 8859 3196 50  0000 L CNN
F 1 "10k" H 8859 3105 50  0000 L CNN
F 2 "" H 8800 3150 50  0001 C CNN
F 3 "~" H 8800 3150 50  0001 C CNN
	1    8800 3150
	1    0    0    -1  
$EndComp
Wire Wire Line
	8450 3450 8800 3450
Wire Wire Line
	8800 3450 8800 3250
$Comp
L Device:R_Small R?
U 1 1 5E092F27
P 6050 3150
F 0 "R?" H 6109 3196 50  0000 L CNN
F 1 "10k" H 6109 3105 50  0000 L CNN
F 2 "" H 6050 3150 50  0001 C CNN
F 3 "~" H 6050 3150 50  0001 C CNN
	1    6050 3150
	1    0    0    -1  
$EndComp
Wire Wire Line
	7050 3750 6050 3750
Wire Wire Line
	6050 3750 6050 3250
$Comp
L power:+3V3 #PWR?
U 1 1 5E09497A
P 6050 2950
F 0 "#PWR?" H 6050 2800 50  0001 C CNN
F 1 "+3V3" H 6065 3123 50  0000 C CNN
F 2 "" H 6050 2950 50  0001 C CNN
F 3 "" H 6050 2950 50  0001 C CNN
	1    6050 2950
	1    0    0    -1  
$EndComp
Wire Wire Line
	6050 2950 6050 3050
$Comp
L power:+3V3 #PWR?
U 1 1 5E09620D
P 8800 2950
F 0 "#PWR?" H 8800 2800 50  0001 C CNN
F 1 "+3V3" H 8815 3123 50  0000 C CNN
F 2 "" H 8800 2950 50  0001 C CNN
F 3 "" H 8800 2950 50  0001 C CNN
	1    8800 2950
	1    0    0    -1  
$EndComp
Wire Wire Line
	8800 3050 8800 2950
Wire Wire Line
	5950 3350 6600 3350
Wire Wire Line
	5950 3450 6850 3450
$EndSCHEMATC
