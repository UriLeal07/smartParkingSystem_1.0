/**************************************************************************************
* Description: Communication test between arduino Nano and Nodemcu over I2C protocol
* Developer: Uri Leal
* Board: NodeMCU
* I/O pins: D1,D2
* 
* GNU GENERAL PUBLIC LICENSE Version 3, 29 June 2007
* Copyright (C) 2007 Free Software Foundation, Inc. <https://fsf.org/>
**************************************************************************************/

#include <Wire.h>

#define SLAVE_ADDR 0x03           // Slave address associated

String recBuffer = "";            // Buffer used to read the message

void setup()
{
    Serial.begin(9600);           // Begin serial for debug
    Wire.begin(D1, D2);           // Join I2C bus with SDA = D2 and SCL = D1 of NodeMCU
    
    recBuffer.reserve(30);        // Reserve 30 bytes of memory for the buffer string
    delay(1000);
}

void loop()
{
    requestSystemData();          // Get data from slave device
    Serial.println();
    delay(1000);
}

void requestSystemData()
{
    Wire.beginTransmission(SLAVE_ADDR);            // Start comunication with slave device
    Wire.write("Hello Arduino");                   // Start transmitting bytes
    Wire.endTransmission();                        // Stop transmission
    
    Wire.requestFrom(SLAVE_ADDR, 13);              // Request data of size 13 from slave device
    
    while(Wire.available())                        // Read & store the data temporarily in the buffer
        recBuffer += (char)Wire.read();
    
    Serial.print(recBuffer);                      // Show message through serial
    Serial.println();
    recBuffer = "";                               // Clear buffer
}
