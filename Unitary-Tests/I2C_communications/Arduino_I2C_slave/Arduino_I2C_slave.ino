/***************************************************************************************
* Description: Communication test between arduino Nano and Nodemcu over I2C protocol 
* Developer: Uri Leal
* Board: Arduino Nano/Uno
* I/O pins: A4,A5
* 
* GNU GENERAL PUBLIC LICENSE Version 3, 29 June 2007
* Copyright (C) 2007 Free Software Foundation, Inc. <https://fsf.org/>
***************************************************************************************/

#include <Wire.h>

#define ADDRESS 0x03            // Slave device address 0x00 to 0x80
String recBuffer = "";          // Buffer used to read the message

void setup()
{
    Wire.begin(ADDRESS);                        // Join I2C bus with a known address
    Wire.onReceive(receiveEvent);               // Link receive event to a function
    Wire.onRequest(requestEvent);               // Link request event to a function
    
    recBuffer.reserve(30);                      // Reserve 30 bytes of memory for the buffer string
    Serial.begin(9600);                         // Begin serial for debug
}

void loop()
{
    delay(10);                                 // Do something
}

void receiveEvent(int howMany)
{
    while (0 < Wire.available())                // Read & store the data temporarily in the buffer
        recBuffer += (char)Wire.read();
    
    Serial.print(recBuffer);                    // Show message through serial
    Serial.println();
    recBuffer = "";                             // Clear buffer
}

void requestEvent()
{
    Wire.write("Hello NodeMCU");                //Send string on request
}
