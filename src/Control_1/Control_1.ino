/**********************************************************************************
* Description: Sensor array control using the HC_SR04 Ultrasonic Distance Sensors
* with the ATMEGA 328P, send the information of the sensors when it's requested
* by the main system component (Master NodeMCU)
* Developer: Uri Leal
* Board: Arduino Nano
* Sensors/Components: HC_SR04 (Ultrasonic D.S.)
* I/O pins: D2-D12, A0-A6
* 
* GNU GENERAL PUBLIC LICENSE Version 3, 29 June 2007
* Copyright (C) 2007 Free Software Foundation, Inc. <https://fsf.org/>
***********************************************************************************/

#include "UDS.h"
#include <Wire.h>

String recBuffer;                       // Buffer used to read the message
volatile byte info;                     // byte that contains the information of wich space is occupied

#define ADDRESS 0x02                    // Slave device address 0x00 to 0x80
#define THRESHOLD_VAL 10.0              // Value used to ajust the detection bounds in centimeters, range [ 1.0 - 400.0 ] use 0 to set the default value

UDS s1(2, 3, 10, 11, THRESHOLD_VAL);
UDS s2(4, 5, 12, A2, THRESHOLD_VAL);
UDS s3(6, 7, A0, A1, THRESHOLD_VAL);
UDS s4(8, 9, A3, A6, THRESHOLD_VAL);

void setup()
{
    Wire.begin(ADDRESS);                        // Join I2C bus with a given address
    Wire.onReceive(receiveEvent);               // Link receive event to a function
    Wire.onRequest(requestEvent);               // Link request event to a function
    
    recBuffer.reserve(4);                      // Reserve 4 bytes of memory for the buffer string
    
    recBuffer = "";
    
    info = B00000000;  
}

void loop()
{
    if(s1.isThresholdReached())
        info |= (1 << 0);
    
    else
        info &= ~(1 << 0);

    if(s2.isThresholdReached())
        info |= (1 << 1);
        
    else
        info &= ~(1 << 1);
    
    if(s3.isThresholdReached())
        info |= (1 << 2);

    else
        info &= ~(1 << 2);

    if(s4.isThresholdReached())
        info |= (1 << 3);
    
    else
        info &= ~(1 << 3);
}

void receiveEvent(int nBytes)
{
    while (Wire.available())                // Read & store the data temporarily in the buffer
        recBuffer.concat(Wire.read());
    
    recBuffer = "";                             // Clear buffer
}

void requestEvent()
{
    Wire.write(info);                // Send byte on request
}
