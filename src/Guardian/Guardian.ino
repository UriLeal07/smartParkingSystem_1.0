/************************************************************************
* Description: Component of the system that controls the I/O of the 
* parking lot
* Developer: Uri Leal
* Board: Arduino Nano/Uno
* I/O pins: D2-D12, A0-A6
* Sensors/Components: HC_SR04 (Ultrasonic D.S.), 28BYJ-48 (Stepper-Motor)
* Motor step sequence: 2 coils at time
* 
* GNU GENERAL PUBLIC LICENSE Version 3, 29 June 2007
* Copyright (C) 2007 Free Software Foundation, Inc. <https://fsf.org/>
************************************************************************/

#include "UDS.h"
#include <Wire.h>

// Motor controller pins
#define IN1  6
#define IN2  7
#define IN3  8
#define IN4  9

// Motor 2 controller pins
#define IN5  2
#define IN6  3
#define IN7  4
#define IN8  5

#define THRESHOLD_VAL 10.0    // Value used to ajust the detection bounds in centimeters, range [ 1.0 - 400.0 ] use 0 to set the default value

#define ADDRESS 0x04                      // Slave device address 0x00 to 0x80

UDS se(10, 11, A1, A2, THRESHOLD_VAL);
UDS ss(12, A0, A3, A6, THRESHOLD_VAL);

const byte nSteps = 125;              // Number of steps required to make a 90 degree turn (Aprox.), try it with different values from 0 to 4096
const byte speed = 10;                // Max. turn speed of 100Hz specified in the 28BYJ-48 datasheet, try it with different speeds around 10 - 150
int i;                                // Auxiliar counter
byte step;                            // Auxiliar step counter

volatile byte info;                     // byte that contains the information of wich space is occupied
byte buffer;                            // Buffer used to read the message

// Control sequence
const byte paso [4][4] = {
    {1, 0, 0, 0},
    {0, 1, 0, 0},
    {0, 0, 1, 0},
    {0, 0, 0, 1}
};

void setup()
{
    Wire.begin(ADDRESS);                        // Join I2C bus with a given address
    Wire.onReceive(receiveEvent);               // Link receive event to a function
    Wire.onRequest(requestEvent);               // Link request event to a function
    
    pinMode(IN1, OUTPUT);
    pinMode(IN2, OUTPUT);
    pinMode(IN3, OUTPUT);
    pinMode(IN4, OUTPUT);
    pinMode(IN5, OUTPUT);
    pinMode(IN6, OUTPUT);
    pinMode(IN7, OUTPUT);
    pinMode(IN8, OUTPUT);
    
    step = i = 0;
    info = buffer = B00000000;
}

void loop()
{
    if(se.isThresholdReached() && info > 0)
    {
        noInterrupts();
        openGate1();
        delay(1000);
        closeGate1();
        interrupts();
    }
    
    if(ss.isThresholdReached() && info > 0)
    {
        noInterrupts();
        openGate2();
        delay(1000);
        closeGate2();
        interrupts();
    }
}

void receiveEvent(int nBytes)
{
    while (Wire.available())                // Read & store the data temporarily in the buffer
        buffer = Wire.read();
    
    info = buffer; // Store the read byte
}

void requestEvent()
{
    Wire.write(info);                // Send byte on request
}

void openGate1()
{
    step = nSteps;
    
    while(step-- > 0)
    {
        for(i = 3; i >= 0; i--)
        {
            digitalWrite(IN1, paso[i][0]);
            delay(speed);
            digitalWrite(IN2, paso[i][1]);
            delay(speed);
            digitalWrite(IN3, paso[i][2]);
            delay(speed);
            digitalWrite(IN4, paso[i][3]);
            delay(speed);
        }
    }
}

void closeGate1()
{
    step = 0;
    
    while(step++ < nSteps)
    {
        for(i = 0; i < 4; i++)
        {
            digitalWrite(IN1, paso[i][0]);
            delay(speed);
            digitalWrite(IN2, paso[i][1]);
            delay(speed);
            digitalWrite(IN3, paso[i][2]);
            delay(speed);
            digitalWrite(IN4, paso[i][3]);
            delay(speed);
        }
    }
}

void openGate2()
{
    step = 0;
    
    while(step++ < nSteps)
    {
        for(i = 0; i < 4; i++)
        {
            digitalWrite(IN5, paso[i][0]);
            delay(speed);
            digitalWrite(IN6, paso[i][1]);
            delay(speed);
            digitalWrite(IN7, paso[i][2]);
            delay(speed);
            digitalWrite(IN8, paso[i][3]);
            delay(speed);
        }
    }
}

void closeGate2()
{
    step = nSteps;
    
    while(step-- > 0)
    {
        for(i = 3; i >= 0; i--)
        {
            digitalWrite(IN5, paso[i][0]);
            delay(speed);
            digitalWrite(IN6, paso[i][1]);
            delay(speed);
            digitalWrite(IN7, paso[i][2]);
            delay(speed);
            digitalWrite(IN8, paso[i][3]);
            delay(speed);
        }
    }
}
