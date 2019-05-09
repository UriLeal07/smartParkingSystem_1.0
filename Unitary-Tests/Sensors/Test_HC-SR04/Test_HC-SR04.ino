/*******************************************************************************
* Description: Sensor array test using the HC_SR04 Ultrasonic Distance Sensors
* Developer: Uri Leal
* Board: Arduino Nano
* I/O pins: D2-D5, D10-12, A0
* 
* GNU GENERAL PUBLIC LICENSE Version 3, 29 June 2007
* Copyright (C) 2007 Free Software Foundation, Inc. <https://fsf.org/>
********************************************************************************/

#include "UDS.h"
#define THRESHOLD_VAL 15.0    // Value used to ajust the detection bounds in centimeters, range [ 1.0 - 400.0 ] use 0 to set the default value

// Initialize sensors that we are gonna use pins D2-D9
//UDS uSensors[4] = {UDS(2, 3, 10, 11, THRESHOLD_VAL), null, null, null};
UDS s1(2, 3, 10, 11, THRESHOLD_VAL);
UDS s2(4, 5, 12, A0, THRESHOLD_VAL);

void setup()
{
    Serial.begin(9600);  // We initialize serial connection so that we could print values from the sensors
}

void loop()
{
    s1.checkDistance();
    delayMicroseconds(50);
    s2.checkDistance();
    delayMicroseconds(50);
}
