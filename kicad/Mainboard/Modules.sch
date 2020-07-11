EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 8 8
Title "Modules"
Date "2020-01-20"
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 "Author: DB9MAT Mathis, SO3ALG Nikoloz, SP5WWP Wojciech"
Comment4 "TR-9 Open Source Digital Radio"
$EndDescr
$Comp
L Device:C C103
U 1 1 5E4E2C01
P 5800 4600
F 0 "C103" H 5450 4650 50  0000 L CNN
F 1 "100n" H 5450 4550 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric" H 5838 4450 50  0001 C CNN
F 3 "~" H 5800 4600 50  0001 C CNN
	1    5800 4600
	1    0    0    -1  
$EndComp
Text GLabel 7200 4450 2    50   Input ~ 10
UART2_TX
Text GLabel 5650 4150 0    50   Input ~ 10
UART2_RX
Text GLabel 5650 4250 0    50   Input ~ 10
WIFI_EN
Text GLabel 5650 4350 0    50   Input ~ 10
WIFI_RST
$Comp
L power:+3.3V #PWR0215
U 1 1 5F0624C4
P 5700 3900
F 0 "#PWR0215" H 5700 3750 50  0001 C CNN
F 1 "+3.3V" V 5715 4028 50  0000 L CNN
F 2 "" H 5700 3900 50  0001 C CNN
F 3 "" H 5700 3900 50  0001 C CNN
	1    5700 3900
	1    0    0    -1  
$EndComp
$Comp
L Connector_Generic:Conn_02x04_Odd_Even J1
U 1 1 5F063A2F
P 6400 4250
F 0 "J1" H 6450 4567 50  0000 C CNN
F 1 "ESP8266 module" H 6450 4476 50  0000 C CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_2x04_P2.54mm_Vertical" H 6400 4250 50  0001 C CNN
F 3 "~" H 6400 4250 50  0001 C CNN
	1    6400 4250
	1    0    0    -1  
$EndComp
Wire Wire Line
	6200 4350 5650 4350
Wire Wire Line
	5650 4250 6200 4250
Wire Wire Line
	5700 3900 5700 4450
$Comp
L Device:C C94
U 1 1 5F07B859
P 6000 4600
F 0 "C94" H 6115 4646 50  0000 L CNN
F 1 "10n" H 6115 4555 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric" H 6038 4450 50  0001 C CNN
F 3 "~" H 6000 4600 50  0001 C CNN
	1    6000 4600
	1    0    0    -1  
$EndComp
Connection ~ 6000 4450
Wire Wire Line
	6000 4450 6200 4450
$Comp
L power:GND #PWR0216
U 1 1 5F07DDAF
P 6000 4850
F 0 "#PWR0216" H 6000 4600 50  0001 C CNN
F 1 "GND" H 6005 4677 50  0000 C CNN
F 2 "" H 6000 4850 50  0001 C CNN
F 3 "" H 6000 4850 50  0001 C CNN
	1    6000 4850
	1    0    0    -1  
$EndComp
Wire Wire Line
	6000 4750 6000 4800
Wire Wire Line
	6700 4150 7050 4150
Wire Wire Line
	7050 4150 7050 4800
Wire Wire Line
	7050 4800 6000 4800
Connection ~ 6000 4800
Wire Wire Line
	6000 4800 6000 4850
Wire Wire Line
	6200 4150 5650 4150
Wire Wire Line
	7200 4450 6700 4450
Wire Wire Line
	5700 4450 5800 4450
Connection ~ 5800 4450
Wire Wire Line
	5800 4450 6000 4450
Wire Wire Line
	5800 4750 5800 4800
Wire Wire Line
	5800 4800 6000 4800
Text Notes 5550 5250 0    50   ~ 0
This is a place for ESP8266 module
Wire Notes Line
	5400 5100 5400 5350
Wire Notes Line
	7100 5100 7100 5350
Wire Notes Line
	7100 5100 5400 5100
Wire Notes Line
	5400 5350 7100 5350
$EndSCHEMATC
