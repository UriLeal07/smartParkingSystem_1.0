/************************************************************************
* Description: Test for a 28BYJ-48 stepper motor
* Developer: Uri Leal
* Board: Arduino Nano/Uno
* I/O pins: D5-D8
* Motor step sequence: 2 coils at time
* 
* GNU GENERAL PUBLIC LICENSE Version 3, 29 June 2007
* Copyright (C) 2007 Free Software Foundation, Inc. <https://fsf.org/>
************************************************************************/

// Motor controller pins
#define IN1  5
#define IN2  6
#define IN3  7
#define IN4  8

const byte nSteps = 125;             // Number of steps required to make a 90 degree turn (Aprox.), try it with different values from 0 to 4096
const byte LED = 13;                 // D13 pin | PORTB = PB5
const byte speed = 5;               // Max. turn speed of 100Hz specified in the 28BYJ-48 datasheet, try it with different speeds around 10 - 150
int i;                               // Auxiliar counter
byte step;                           // Auxiliar step counter

// Control sequence
const byte paso [4][4] = {
    {1, 0, 0, 0},
    {0, 1, 0, 0},
    {0, 0, 1, 0},
    {0, 0, 0, 1}
};

void setup()
{
    // Init I/O Pins
    pinMode(IN1, OUTPUT);
    pinMode(IN2, OUTPUT);
    pinMode(IN3, OUTPUT);
    pinMode(IN4, OUTPUT);
    pinMode(LED, OUTPUT);

    // Init variables
    step = i = 0;
}

void loop()
{
    openGate();                 // Execute forward test
    delay(1000);
    closeGate();                // Execute backward test
    
    // Make sure all the motor-control pins are LOW
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, LOW);
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, LOW);
    
    // Finish the test program
    while(true)
    {
        // Blink an LED
        digitalWrite(LED, LOW);
        delay(2000);
        digitalWrite(LED, HIGH);
        delay(2000);
    }
}

void openGate()
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

void closeGate()
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
