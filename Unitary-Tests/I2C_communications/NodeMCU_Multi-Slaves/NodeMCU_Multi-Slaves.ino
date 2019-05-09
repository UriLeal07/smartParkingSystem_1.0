/****************************************************************************
* Description: Communication test between multiple arduinos Nano and Nodemcu
* over I2C protocol using timers
* Developer: Uri Leal
* Board: NodeMCU
* I/O pins: D1,D2
* 
* GNU GENERAL PUBLIC LICENSE Version 3, 29 June 2007
* Copyright (C) 2007 Free Software Foundation, Inc. <https://fsf.org/>
*****************************************************************************/

#include <Wire.h>
#include <Ticker.h>

Ticker acquire;                            // Timer to read from the I2C bus to acquire the data
Ticker print;                              // Timer to print the acquired data

#define SLAVE_ADDR1 0x02                   // Slave address associated
#define SLAVE_ADDR2 0x03                   // Slave address associated

const byte nBytes = 3;

byte buffer[nBytes];                        // Buffer used to read the message
volatile bool requesting;                   // Boolean variable to avoid overlapping task
volatile bool printing;                     // Boolean variable to avoid overlapping task
volatile byte spaces;                       // Contains which spaces are occupied and which are not
volatile byte stopIt;                       // Auxiliar variable to stop requesting data (for testing purposes)

void setup()
{
    Serial.begin(9600);           // Begin serial for debug
    Wire.begin(D1, D2);           // Join I2C bus with SDA = D1 and SCL = D2 of NodeMCU
    
    requesting = false;
    spaces = B00000000;         // Init byte
    
    Serial.println("NodeMCU Master started!");
    
    acquire.attach_scheduled(1.5, requestSystemData);         // Start scheduled timer to acquire the data every 1.5 seconds
    print.attach_scheduled(2, printData);                     // Start scheduled timer to print the acquired data every 2 seconds
    
    stopIt = 0;   
}

void loop()
{
    // Do other things
}

void requestSystemData()
{
    if(!requesting && stopIt < 5)    // Only enter if the previous (if exist) task was completed
    {
        requesting = true;    // Avoid overlapping
        stopIt++;
        
        Wire.requestFrom(SLAVE_ADDR1, 1);              // Request data of size 1 from slave device #1

        while(Wire.available())
            buffer[0] = Wire.read();                       // Read & store the data temporarily in the buffer
        
        Serial.print("Buffer= ");
        Serial.println(buffer[0], BIN);
        
        // Read and set the first 4 bits of data from the first slave device to the spaces variable
        bitWrite(spaces, 0, (bitRead(buffer[0], 0)));
        bitWrite(spaces, 1, (bitRead(buffer[0], 1)));
        bitWrite(spaces, 2, (bitRead(buffer[0], 2)));
        bitWrite(spaces, 3, (bitRead(buffer[0], 3)));
        
        buffer[0] = B00000000;                          // Clear buffer
        
        Wire.requestFrom(SLAVE_ADDR2, 1);              // Request data of size 1 from slave device #2

        while(Wire.available())
            buffer[0] = Wire.read();                       // Read & store the data temporarily in the buffer

        Serial.print("Buffer= ");
        Serial.println(buffer[0], BIN);
        
        // Read the first 4 bits of the buffer data from slave #2 and set it into the last 4 bits of the variable spaces
        bitWrite(spaces, 4, (bitRead(buffer[0], 0)));
        bitWrite(spaces, 5, (bitRead(buffer[0], 1)));
        bitWrite(spaces, 6, (bitRead(buffer[0], 2)));
        bitWrite(spaces, 7, (bitRead(buffer[0], 3)));
        
        buffer[0] = B00000000;                          // Clear buffer
        requesting = false;   // Work finished
    }
}

void printData()
{
    if(!printing && !requesting)
    {
        printing = true;    // Avoid overlapping
        
        Serial.print("Spaces= ");
        Serial.println(spaces, BIN);
        
        printing = false;   // Work finished
    }
}
