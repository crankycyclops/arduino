EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr USLetter 11000 8500
encoding utf-8
Sheet 1 1
Title "Blink"
Date ""
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L Device:LED D2
U 1 1 603583AE
P 5400 4400
F 0 "D2" H 5393 4617 50  0000 C CNN
F 1 "Yellow LED" H 5393 4526 50  0000 C CNN
F 2 "LED_THT:LED_D3.0mm" H 5400 4400 50  0001 C CNN
F 3 "~" H 5400 4400 50  0001 C CNN
	1    5400 4400
	-1   0    0    1   
$EndComp
$Comp
L Device:LED D1
U 1 1 60358733
P 5400 4000
F 0 "D1" H 5393 4217 50  0000 C CNN
F 1 "Red LED" H 5393 4126 50  0000 C CNN
F 2 "LED_THT:LED_D3.0mm" H 5400 4000 50  0001 C CNN
F 3 "~" H 5400 4000 50  0001 C CNN
	1    5400 4000
	-1   0    0    1   
$EndComp
$Comp
L Connector_Generic:Conn_01x04 J1
U 1 1 6035B6DA
P 3800 4400
F 0 "J1" H 3718 4717 50  0000 C CNN
F 1 "Arduino Pins" H 3718 4626 50  0000 C CNN
F 2 "Connector_PinSocket_2.54mm:PinSocket_1x04_P2.54mm_Vertical" H 3800 4400 50  0001 C CNN
F 3 "~" H 3800 4400 50  0001 C CNN
	1    3800 4400
	-1   0    0    -1  
$EndComp
$Comp
L Device:LED D3
U 1 1 60357728
P 5400 4800
F 0 "D3" H 5393 5017 50  0000 C CNN
F 1 "Green LED" H 5393 4926 50  0000 C CNN
F 2 "LED_THT:LED_D3.0mm" H 5400 4800 50  0001 C CNN
F 3 "~" H 5400 4800 50  0001 C CNN
	1    5400 4800
	-1   0    0    1   
$EndComp
$Comp
L Device:R_US R1
U 1 1 60356052
P 4550 4000
F 0 "R1" V 4449 4007 50  0000 C CNN
F 1 "220 Ω" V 4370 4007 50  0000 C CNN
F 2 "Resistor_THT:R_Axial_DIN0414_L11.9mm_D4.5mm_P15.24mm_Horizontal" V 4590 3990 50  0001 C CNN
F 3 "~" H 4550 4000 50  0001 C CNN
	1    4550 4000
	0    -1   -1   0   
$EndComp
$Comp
L Device:R_US R2
U 1 1 603851FD
P 4550 4400
F 0 "R2" V 4449 4407 50  0000 C CNN
F 1 "220 Ω" V 4370 4407 50  0000 C CNN
F 2 "Resistor_THT:R_Axial_DIN0414_L11.9mm_D4.5mm_P15.24mm_Horizontal" V 4590 4390 50  0001 C CNN
F 3 "~" H 4550 4400 50  0001 C CNN
	1    4550 4400
	0    -1   -1   0   
$EndComp
$Comp
L Device:R_US R3
U 1 1 603858EE
P 4550 4800
F 0 "R3" V 4449 4807 50  0000 C CNN
F 1 "220 Ω" V 4370 4807 50  0000 C CNN
F 2 "Resistor_THT:R_Axial_DIN0414_L11.9mm_D4.5mm_P15.24mm_Horizontal" V 4590 4790 50  0001 C CNN
F 3 "~" H 4550 4800 50  0001 C CNN
	1    4550 4800
	0    -1   -1   0   
$EndComp
Wire Wire Line
	4700 4000 5250 4000
Wire Wire Line
	4700 4400 5250 4400
Wire Wire Line
	4700 4800 5250 4800
Wire Wire Line
	4400 4000 4400 4300
Wire Wire Line
	4400 4300 4000 4300
Wire Wire Line
	4400 4400 4000 4400
Wire Wire Line
	4000 4500 4400 4500
Wire Wire Line
	4400 4500 4400 4800
Wire Wire Line
	5550 4000 5850 4000
Wire Wire Line
	5850 4000 5850 4400
Wire Wire Line
	5850 4400 5550 4400
Wire Wire Line
	5550 4800 5850 4800
Wire Wire Line
	5850 4800 5850 4400
Connection ~ 5850 4400
Text GLabel 4100 4600 2    50   Input ~ 0
GND
Wire Wire Line
	4100 4600 4000 4600
Text GLabel 6150 4400 2    50   Input ~ 0
GND
Wire Wire Line
	5850 4400 6150 4400
$EndSCHEMATC