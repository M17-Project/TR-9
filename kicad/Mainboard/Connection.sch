EESchema Schematic File Version 4
LIBS:Mainboard-cache
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
P 3300 1950
F 0 "J12" H 3380 1942 50  0000 L CNN
F 1 "GPS_CONN" H 3380 1851 50  0000 L CNN
F 2 "" H 3300 1950 50  0001 C CNN
F 3 "~" H 3300 1950 50  0001 C CNN
	1    3300 1950
	1    0    0    -1  
$EndComp
$Comp
L power:+5V #PWR0104
U 1 1 5E07BDC5
P 3000 1750
F 0 "#PWR0104" H 3000 1600 50  0001 C CNN
F 1 "+5V" H 3015 1923 50  0000 C CNN
F 2 "" H 3000 1750 50  0001 C CNN
F 3 "" H 3000 1750 50  0001 C CNN
	1    3000 1750
	1    0    0    -1  
$EndComp
Wire Wire Line
	3000 1750 3000 1850
Wire Wire Line
	3000 1850 3100 1850
$Comp
L power:GND #PWR0106
U 1 1 5E07C3FA
P 3000 2250
F 0 "#PWR0106" H 3000 2000 50  0001 C CNN
F 1 "GND" H 3005 2077 50  0000 C CNN
F 2 "" H 3000 2250 50  0001 C CNN
F 3 "" H 3000 2250 50  0001 C CNN
	1    3000 2250
	1    0    0    -1  
$EndComp
Wire Wire Line
	3000 2250 3000 2150
Wire Wire Line
	3000 2150 3100 2150
Wire Wire Line
	3100 1950 3000 1950
Wire Wire Line
	3100 2050 3000 2050
Text GLabel 3000 1950 0    50   Input ~ 10
USART1_TX
Text GLabel 3000 2050 0    50   Input ~ 10
USART1_RX
$Comp
L Mainboard-rescue:Conn_01x03-conn J13
U 1 1 5E07CB74
P 4750 2050
F 0 "J13" H 4830 2092 50  0000 L CNN
F 1 "DBG_CONN" H 4830 2001 50  0000 L CNN
F 2 "" H 4750 2050 50  0001 C CNN
F 3 "~" H 4750 2050 50  0001 C CNN
	1    4750 2050
	1    0    0    -1  
$EndComp
Text GLabel 4450 1950 0    50   Input ~ 10
USART2_TX
Text GLabel 4450 2050 0    50   Input ~ 10
USART2_RX
$Comp
L power:GND #PWR0107
U 1 1 5E07D3F6
P 4450 2250
F 0 "#PWR0107" H 4450 2000 50  0001 C CNN
F 1 "GND" H 4455 2077 50  0000 C CNN
F 2 "" H 4450 2250 50  0001 C CNN
F 3 "" H 4450 2250 50  0001 C CNN
	1    4450 2250
	1    0    0    -1  
$EndComp
Wire Wire Line
	4450 2250 4450 2150
Wire Wire Line
	4450 2150 4550 2150
Wire Wire Line
	4450 2050 4550 2050
Wire Wire Line
	4450 1950 4550 1950
$Comp
L Mainboard-rescue:Micro_SD_Card-conn J15
U 1 1 5E0856B5
P 3950 3900
F 0 "J15" H 3900 4617 50  0000 C CNN
F 1 "Micro_SD_Card" H 3900 4526 50  0000 C CNN
F 2 "" H 5100 4200 50  0001 C CNN
F 3 "http://katalog.we-online.de/em/datasheet/693072010801.pdf" H 3950 3900 50  0001 C CNN
	1    3950 3900
	1    0    0    -1  
$EndComp
Wire Wire Line
	3050 3900 2950 3900
Wire Wire Line
	2950 3900 2950 3500
Wire Wire Line
	3050 4100 2950 4100
$Comp
L power:GND #PWR0114
U 1 1 5E08619F
P 2950 4600
F 0 "#PWR0114" H 2950 4350 50  0001 C CNN
F 1 "GND" H 2955 4427 50  0000 C CNN
F 2 "" H 2950 4600 50  0001 C CNN
F 3 "" H 2950 4600 50  0001 C CNN
	1    2950 4600
	1    0    0    -1  
$EndComp
Wire Wire Line
	2950 4100 2950 4600
$Comp
L power:GND #PWR0115
U 1 1 5E086C12
P 4850 4600
F 0 "#PWR0115" H 4850 4350 50  0001 C CNN
F 1 "GND" H 4855 4427 50  0000 C CNN
F 2 "" H 4850 4600 50  0001 C CNN
F 3 "" H 4850 4600 50  0001 C CNN
	1    4850 4600
	1    0    0    -1  
$EndComp
Wire Wire Line
	4750 4500 4850 4500
Wire Wire Line
	4850 4500 4850 4600
$Comp
L power:+3V3 #PWR0109
U 1 1 5E087582
P 2950 3500
F 0 "#PWR0109" H 2950 3350 50  0001 C CNN
F 1 "+3V3" H 2965 3673 50  0000 C CNN
F 2 "" H 2950 3500 50  0001 C CNN
F 3 "" H 2950 3500 50  0001 C CNN
	1    2950 3500
	1    0    0    -1  
$EndComp
Text GLabel 2850 3600 0    50   Input ~ 10
SD_D2
Text GLabel 2850 3700 0    50   Input ~ 10
SD_D3
Text GLabel 2850 3800 0    50   Input ~ 10
SD_CMD
Text GLabel 2850 4000 0    50   Input ~ 10
SD_CLK
Text GLabel 2850 4200 0    50   Input ~ 10
SD_D0
Text GLabel 2850 4300 0    50   Input ~ 10
SD_D1
Wire Wire Line
	2850 4300 3050 4300
Wire Wire Line
	2850 4200 3050 4200
Wire Wire Line
	2850 4000 3050 4000
Wire Wire Line
	2850 3800 3050 3800
Wire Wire Line
	2850 3700 3050 3700
Wire Wire Line
	2850 3600 3050 3600
Text GLabel 6600 3550 0    50   Input ~ 10
I2C1_SDA
Text GLabel 6600 3450 0    50   Input ~ 10
I2C1_SCL
Text Notes 6700 3950 0    50   ~ 10
ADDR 0x6B AG, 0x1E M
Text Notes 3750 1600 0    50   ~ 10
TODO: Replace GPS connector
$Comp
L Sensor_Motion:LSM9DS1 U?
U 1 1 5E067250
P 8400 3850
F 0 "U?" H 8400 3900 50  0000 C CNN
F 1 "LSM9DS1" H 8400 3800 50  0000 C CNN
F 2 "Package_LGA:LGA-24L_3x3.5mm_P0.43mm" H 9900 4600 50  0001 C CNN
F 3 "http://www.st.com/content/ccc/resource/technical/document/datasheet/1e/3f/2a/d6/25/eb/48/46/DM00103319.pdf/files/DM00103319.pdf/jcr:content/translations/en.DM00103319.pdf" H 8400 3950 50  0001 C CNN
	1    8400 3850
	1    0    0    -1  
$EndComp
$Comp
L power:+3V3 #PWR?
U 1 1 5E06CB2E
P 8400 2850
F 0 "#PWR?" H 8400 2700 50  0001 C CNN
F 1 "+3V3" H 8415 3023 50  0000 C CNN
F 2 "" H 8400 2850 50  0001 C CNN
F 3 "" H 8400 2850 50  0001 C CNN
	1    8400 2850
	1    0    0    -1  
$EndComp
Wire Wire Line
	8400 2850 8400 2950
Wire Wire Line
	8400 2950 8500 2950
Wire Wire Line
	8500 2950 8500 3050
Connection ~ 8400 2950
Wire Wire Line
	8400 2950 8400 3050
Wire Wire Line
	8500 2950 8700 2950
Wire Wire Line
	8700 2950 8700 3050
Connection ~ 8500 2950
Wire Wire Line
	8700 2950 8800 2950
Wire Wire Line
	8800 2950 8800 3050
Connection ~ 8700 2950
$Comp
L power:GND #PWR?
U 1 1 5E06F5C7
P 8300 4850
F 0 "#PWR?" H 8300 4600 50  0001 C CNN
F 1 "GND" H 8305 4677 50  0000 C CNN
F 2 "" H 8300 4850 50  0001 C CNN
F 3 "" H 8300 4850 50  0001 C CNN
	1    8300 4850
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 5E06FFF0
P 8500 4850
F 0 "#PWR?" H 8500 4600 50  0001 C CNN
F 1 "GND" H 8505 4677 50  0000 C CNN
F 2 "" H 8500 4850 50  0001 C CNN
F 3 "" H 8500 4850 50  0001 C CNN
	1    8500 4850
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 5E070F61
P 9200 4850
F 0 "#PWR?" H 9200 4600 50  0001 C CNN
F 1 "GND" H 9205 4677 50  0000 C CNN
F 2 "" H 9200 4850 50  0001 C CNN
F 3 "" H 9200 4850 50  0001 C CNN
	1    9200 4850
	1    0    0    -1  
$EndComp
Wire Wire Line
	9100 3950 9200 3950
Wire Wire Line
	9200 3950 9200 4050
Wire Wire Line
	9100 4350 9200 4350
Connection ~ 9200 4350
Wire Wire Line
	9100 4250 9200 4250
Connection ~ 9200 4250
Wire Wire Line
	9200 4250 9200 4350
Wire Wire Line
	9100 4150 9200 4150
Connection ~ 9200 4150
Wire Wire Line
	9200 4150 9200 4250
Wire Wire Line
	9100 4050 9200 4050
Connection ~ 9200 4050
Wire Wire Line
	9200 4050 9200 4150
$Comp
L Device:R_Small R?
U 1 1 5E07451A
P 7500 3250
F 0 "R?" H 7559 3296 50  0000 L CNN
F 1 "4k7" H 7559 3205 50  0000 L CNN
F 2 "" H 7500 3250 50  0001 C CNN
F 3 "~" H 7500 3250 50  0001 C CNN
	1    7500 3250
	1    0    0    -1  
$EndComp
$Comp
L Device:R_Small R?
U 1 1 5E075066
P 7250 3250
F 0 "R?" H 7309 3296 50  0000 L CNN
F 1 "4k7" H 7309 3205 50  0000 L CNN
F 2 "" H 7250 3250 50  0001 C CNN
F 3 "~" H 7250 3250 50  0001 C CNN
	1    7250 3250
	1    0    0    -1  
$EndComp
Wire Wire Line
	7250 3450 7250 3350
Wire Wire Line
	7250 3450 7700 3450
Connection ~ 7250 3450
Wire Wire Line
	7500 3550 7500 3350
Wire Wire Line
	7500 3550 7700 3550
Connection ~ 7500 3550
$Comp
L power:+3V3 #PWR?
U 1 1 5E0788C1
P 7500 3050
F 0 "#PWR?" H 7500 2900 50  0001 C CNN
F 1 "+3V3" H 7515 3223 50  0000 C CNN
F 2 "" H 7500 3050 50  0001 C CNN
F 3 "" H 7500 3050 50  0001 C CNN
	1    7500 3050
	1    0    0    -1  
$EndComp
$Comp
L power:+3V3 #PWR?
U 1 1 5E078F90
P 7250 3050
F 0 "#PWR?" H 7250 2900 50  0001 C CNN
F 1 "+3V3" H 7265 3223 50  0000 C CNN
F 2 "" H 7250 3050 50  0001 C CNN
F 3 "" H 7250 3050 50  0001 C CNN
	1    7250 3050
	1    0    0    -1  
$EndComp
Wire Wire Line
	7250 3050 7250 3150
Wire Wire Line
	7500 3050 7500 3150
$Comp
L Device:C_Small C?
U 1 1 5E07AC5C
P 7500 4650
F 0 "C?" H 7592 4696 50  0000 L CNN
F 1 "100n" H 7592 4605 50  0000 L CNN
F 2 "" H 7500 4650 50  0001 C CNN
F 3 "~" H 7500 4650 50  0001 C CNN
	1    7500 4650
	1    0    0    -1  
$EndComp
$Comp
L Device:C_Small C?
U 1 1 5E07BBD5
P 7150 4650
F 0 "C?" H 7242 4696 50  0000 L CNN
F 1 "10n" H 7242 4605 50  0000 L CNN
F 2 "" H 7150 4650 50  0001 C CNN
F 3 "~" H 7150 4650 50  0001 C CNN
	1    7150 4650
	1    0    0    -1  
$EndComp
Wire Wire Line
	7700 4350 7150 4350
Wire Wire Line
	7150 4350 7150 4550
Wire Wire Line
	7700 4450 7500 4450
Wire Wire Line
	7500 4450 7500 4550
$Comp
L power:GND #PWR?
U 1 1 5E07DD63
P 7500 4850
F 0 "#PWR?" H 7500 4600 50  0001 C CNN
F 1 "GND" H 7505 4677 50  0000 C CNN
F 2 "" H 7500 4850 50  0001 C CNN
F 3 "" H 7500 4850 50  0001 C CNN
	1    7500 4850
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 5E07E2EE
P 7150 4850
F 0 "#PWR?" H 7150 4600 50  0001 C CNN
F 1 "GND" H 7155 4677 50  0000 C CNN
F 2 "" H 7150 4850 50  0001 C CNN
F 3 "" H 7150 4850 50  0001 C CNN
	1    7150 4850
	1    0    0    -1  
$EndComp
Wire Wire Line
	7150 4750 7150 4850
Wire Wire Line
	7500 4750 7500 4850
Wire Wire Line
	8300 4650 8300 4850
Wire Wire Line
	8500 4650 8500 4850
Wire Wire Line
	9200 4350 9200 4850
$Comp
L Device:R_Small R?
U 1 1 5E085D7F
P 6950 3250
F 0 "R?" H 7009 3296 50  0000 L CNN
F 1 "10k" H 7009 3205 50  0000 L CNN
F 2 "" H 6950 3250 50  0001 C CNN
F 3 "~" H 6950 3250 50  0001 C CNN
	1    6950 3250
	1    0    0    -1  
$EndComp
$Comp
L Device:R_Small R?
U 1 1 5E086057
P 9200 3250
F 0 "R?" H 9259 3296 50  0000 L CNN
F 1 "10k" H 9259 3205 50  0000 L CNN
F 2 "" H 9200 3250 50  0001 C CNN
F 3 "~" H 9200 3250 50  0001 C CNN
	1    9200 3250
	1    0    0    -1  
$EndComp
Wire Wire Line
	7700 3750 6950 3750
Wire Wire Line
	6950 3750 6950 3350
Wire Wire Line
	9100 3450 9200 3450
Wire Wire Line
	9200 3450 9200 3350
$Comp
L power:+3V3 #PWR?
U 1 1 5E089278
P 6950 3050
F 0 "#PWR?" H 6950 2900 50  0001 C CNN
F 1 "+3V3" H 6965 3223 50  0000 C CNN
F 2 "" H 6950 3050 50  0001 C CNN
F 3 "" H 6950 3050 50  0001 C CNN
	1    6950 3050
	1    0    0    -1  
$EndComp
Wire Wire Line
	6950 3050 6950 3150
$Comp
L power:+3V3 #PWR?
U 1 1 5E08A7A1
P 9200 3050
F 0 "#PWR?" H 9200 2900 50  0001 C CNN
F 1 "+3V3" H 9215 3223 50  0000 C CNN
F 2 "" H 9200 3050 50  0001 C CNN
F 3 "" H 9200 3050 50  0001 C CNN
	1    9200 3050
	1    0    0    -1  
$EndComp
Wire Wire Line
	9200 3050 9200 3150
NoConn ~ 7700 4150
NoConn ~ 7700 4050
NoConn ~ 7700 3950
NoConn ~ 9100 3750
NoConn ~ 9100 3650
$Comp
L Device:R_Small R?
U 1 1 5E0918A1
P 9450 3250
F 0 "R?" H 9509 3296 50  0000 L CNN
F 1 "10k" H 9509 3205 50  0000 L CNN
F 2 "" H 9450 3250 50  0001 C CNN
F 3 "~" H 9450 3250 50  0001 C CNN
	1    9450 3250
	1    0    0    -1  
$EndComp
Wire Wire Line
	9100 3550 9450 3550
Wire Wire Line
	9450 3550 9450 3350
$Comp
L Device:R_Small R?
U 1 1 5E092F27
P 6700 3250
F 0 "R?" H 6759 3296 50  0000 L CNN
F 1 "10k" H 6759 3205 50  0000 L CNN
F 2 "" H 6700 3250 50  0001 C CNN
F 3 "~" H 6700 3250 50  0001 C CNN
	1    6700 3250
	1    0    0    -1  
$EndComp
Wire Wire Line
	7700 3850 6700 3850
Wire Wire Line
	6700 3850 6700 3350
$Comp
L power:+3V3 #PWR?
U 1 1 5E09497A
P 6700 3050
F 0 "#PWR?" H 6700 2900 50  0001 C CNN
F 1 "+3V3" H 6715 3223 50  0000 C CNN
F 2 "" H 6700 3050 50  0001 C CNN
F 3 "" H 6700 3050 50  0001 C CNN
	1    6700 3050
	1    0    0    -1  
$EndComp
Wire Wire Line
	6700 3050 6700 3150
$Comp
L power:+3V3 #PWR?
U 1 1 5E09620D
P 9450 3050
F 0 "#PWR?" H 9450 2900 50  0001 C CNN
F 1 "+3V3" H 9465 3223 50  0000 C CNN
F 2 "" H 9450 3050 50  0001 C CNN
F 3 "" H 9450 3050 50  0001 C CNN
	1    9450 3050
	1    0    0    -1  
$EndComp
Wire Wire Line
	9450 3150 9450 3050
Wire Wire Line
	6600 3450 7250 3450
Wire Wire Line
	6600 3550 7500 3550
$EndSCHEMATC
