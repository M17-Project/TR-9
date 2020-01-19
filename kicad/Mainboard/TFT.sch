EESchema Schematic File Version 4
LIBS:Mainboard-cache
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 4 7
Title "HMI Board Connector"
Date ""
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 "TR-9 Open Source Digital Radio"
$EndDescr
$Comp
L Mainboard-rescue:BSS138-transistors Q1
U 1 1 5DF0AD3D
P 3400 4100
F 0 "Q1" H 3606 4146 50  0000 L CNN
F 1 "BSS138" H 3606 4055 50  0000 L CNN
F 2 "TO_SOT_Packages_SMD:SOT-23" H 3600 4025 50  0001 L CIN
F 3 "https://www.fairchildsemi.com/datasheets/BS/BSS138.pdf" H 3400 4100 50  0001 L CNN
	1    3400 4100
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR051
U 1 1 5DF0BBF9
P 3500 4400
F 0 "#PWR051" H 3500 4150 50  0001 C CNN
F 1 "GND" H 3505 4227 50  0000 C CNN
F 2 "" H 3500 4400 50  0001 C CNN
F 3 "" H 3500 4400 50  0001 C CNN
	1    3500 4400
	1    0    0    -1  
$EndComp
Wire Wire Line
	3500 4300 3500 4400
Wire Wire Line
	3200 4100 3100 4100
Text GLabel 3100 4100 0    50   Input ~ 10
BLIGHT_CTRL
Wire Wire Line
	3500 3900 3500 3800
Wire Wire Line
	3500 3800 3600 3800
Text GLabel 3600 3800 2    50   Input ~ 10
TFT_LED
Text GLabel 6100 3300 2    50   Input ~ 10
TFT_LED
Text GLabel 6100 3400 2    50   Input ~ 10
TFT_RST
Text GLabel 6100 3500 2    50   Input ~ 10
SPI1_CS
Text GLabel 6100 3600 2    50   Input ~ 10
SPI1_MOSI
Text GLabel 6100 3700 2    50   Input ~ 10
SPI1_SCK
Wire Wire Line
	5800 3300 6100 3300
Wire Wire Line
	5800 3400 6100 3400
Wire Wire Line
	5800 3500 6100 3500
Wire Wire Line
	5800 3600 6100 3600
Wire Wire Line
	5800 3700 6100 3700
Wire Wire Line
	5800 3200 6000 3200
Wire Wire Line
	6000 3200 6000 3050
$Comp
L power:+3V3 #PWR049
U 1 1 5DF0DB74
P 6000 3050
F 0 "#PWR049" H 6000 2900 50  0001 C CNN
F 1 "+3V3" H 6015 3223 50  0000 C CNN
F 2 "" H 6000 3050 50  0001 C CNN
F 3 "" H 6000 3050 50  0001 C CNN
	1    6000 3050
	1    0    0    -1  
$EndComp
Wire Wire Line
	5800 4100 6000 4100
Wire Wire Line
	6000 4100 6000 4200
$Comp
L power:GND #PWR050
U 1 1 5DF0E0D2
P 6000 4200
F 0 "#PWR050" H 6000 3950 50  0001 C CNN
F 1 "GND" H 6005 4027 50  0000 C CNN
F 2 "" H 6000 4200 50  0001 C CNN
F 3 "" H 6000 4200 50  0001 C CNN
	1    6000 4200
	1    0    0    -1  
$EndComp
Wire Wire Line
	5800 4000 6100 4000
Text GLabel 6100 4000 2    50   Input ~ 10
USART3_RX
$Comp
L power:GND #PWR?
U 1 1 5E1E62B4
P 6650 3900
F 0 "#PWR?" H 6650 3650 50  0001 C CNN
F 1 "GND" H 6655 3727 50  0000 C CNN
F 2 "" H 6650 3900 50  0001 C CNN
F 3 "" H 6650 3900 50  0001 C CNN
	1    6650 3900
	0    -1   -1   0   
$EndComp
Wire Wire Line
	5800 3900 6200 3900
Wire Wire Line
	5800 3800 6200 3800
Wire Wire Line
	6200 3800 6200 3900
Connection ~ 6200 3900
Wire Wire Line
	6200 3900 6650 3900
$Comp
L Connector_Generic:Conn_01x10 J?
U 1 1 5E1EAE64
P 5600 3700
F 0 "J?" H 5518 2975 50  0000 C CNN
F 1 "Conn_01x10" H 5518 3066 50  0000 C CNN
F 2 "" H 5600 3700 50  0001 C CNN
F 3 "~" H 5600 3700 50  0001 C CNN
	1    5600 3700
	-1   0    0    1   
$EndComp
$EndSCHEMATC
