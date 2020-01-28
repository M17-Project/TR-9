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
Comment3 ""
Comment4 "TR-9 Open Source Digital Radio"
$EndDescr
$Comp
L ESP8266:ESP-12F U12
U 1 1 5E25657C
P 5750 3700
F 0 "U12" H 5750 4465 50  0000 C CNN
F 1 "ESP-12F" H 5750 4374 50  0000 C CNN
F 2 "" H 5750 3700 50  0001 C CNN
F 3 "" H 5750 3700 50  0001 C CNN
	1    5750 3700
	1    0    0    -1  
$EndComp
$Comp
L power:+3.3V #PWR0171
U 1 1 5E2571F1
P 4700 4100
F 0 "#PWR0171" H 4700 3950 50  0001 C CNN
F 1 "+3.3V" V 4715 4228 50  0000 L CNN
F 2 "" H 4700 4100 50  0001 C CNN
F 3 "" H 4700 4100 50  0001 C CNN
	1    4700 4100
	0    -1   -1   0   
$EndComp
Wire Wire Line
	4700 4100 4750 4100
$Comp
L Device:C 10u1
U 1 1 5E258338
P 4750 4350
F 0 "10u1" H 4865 4396 50  0000 L CNN
F 1 "C" H 4865 4305 50  0000 L CNN
F 2 "" H 4788 4200 50  0001 C CNN
F 3 "~" H 4750 4350 50  0001 C CNN
	1    4750 4350
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0172
U 1 1 5E259225
P 4750 4550
F 0 "#PWR0172" H 4750 4300 50  0001 C CNN
F 1 "GND" H 4755 4377 50  0000 C CNN
F 2 "" H 4750 4550 50  0001 C CNN
F 3 "" H 4750 4550 50  0001 C CNN
	1    4750 4550
	1    0    0    -1  
$EndComp
Wire Wire Line
	4750 4200 4750 4100
Connection ~ 4750 4100
$Comp
L power:GND #PWR0174
U 1 1 5E259705
P 6750 4550
F 0 "#PWR0174" H 6750 4300 50  0001 C CNN
F 1 "GND" H 6755 4377 50  0000 C CNN
F 2 "" H 6750 4550 50  0001 C CNN
F 3 "" H 6750 4550 50  0001 C CNN
	1    6750 4550
	1    0    0    -1  
$EndComp
Wire Wire Line
	4750 4100 4850 4100
Wire Wire Line
	4700 3400 4850 3400
Wire Wire Line
	4850 3600 4700 3600
Wire Wire Line
	6650 4100 6750 4100
Text GLabel 4700 3600 0    50   Input ~ 10
WIFI_EN
Text GLabel 6750 3500 2    50   Input ~ 10
USART3_TX
Text GLabel 6750 3400 2    50   Input ~ 10
USART3_RX
$Comp
L power:+3.3V #PWR0173
U 1 1 5E25DB0A
P 6750 3800
F 0 "#PWR0173" H 6750 3650 50  0001 C CNN
F 1 "+3.3V" V 6765 3928 50  0000 L CNN
F 2 "" H 6750 3800 50  0001 C CNN
F 3 "" H 6750 3800 50  0001 C CNN
	1    6750 3800
	0    1    1    0   
$EndComp
Wire Wire Line
	6750 3800 6650 3800
Text GLabel 4700 3400 0    50   Input ~ 10
WIFI_RST
Wire Wire Line
	4750 4500 4750 4550
Wire Wire Line
	6750 4100 6750 4550
Wire Wire Line
	6650 3400 6750 3400
Wire Wire Line
	6650 3500 6750 3500
$EndSCHEMATC
