/************************************************************************
* Description: Test for a 28BYJ-48 stepper motor using port manipulation
* Developer: Uri Leal
* Board: Arduino Nano/Uno
* I/O pins: A2,A3,A4,A5 | PORTC = C2,C3,C4,C5 -- D13 | PORTB = B5
* Motor step sequence: "Wave drive" (1 coil at time)
* 
* GNU GENERAL PUBLIC LICENSE Version 3, 29 June 2007
* Copyright (C) 2007 Free Software Foundation, Inc. <https://fsf.org/>
************************************************************************/

const byte nSteps = 125;              // Number of steps required to make a 90 degree turn (Aprox.), try it with different values from 0 to 4096
const byte LED = PB5;                 // D13 pin | PORTB = PB5
const byte speed = 10;                // Max. turn speed of 100Hz specified in the 28BYJ-48 datasheet, try it with different speeds around 10 - 150

void setup()
{
    DDRC |= B00111100;          // Configure PC2-PC5 as OUTPUT (pins A2-A5 in UNO/Nano)
    DDRB |= (1 << LED);         // Configure PB5 (LED pin) as OUTPUT (pin 13 in UNO/Nano)
}

void openGate()
{
    for(int i = 0; i < nSteps; i++)
    {
        PORTC ^= B00100000; // Change state of PC5 to HIGH
        delay(speed);
        PORTC ^= B00110000; // Change state of PC4 to HIGH (change out1 a LOW)
        delay(speed);
        PORTC ^= B00011000; // Change state of PC3 to HIGH (change out2 a LOW)
        delay(speed);
        PORTC ^= B00001100; // Change state of PC2 to HIGH (change out3 a LOW)
        delay(speed);
        PORTC &= B11000011; // Output pins PC2 - PC5 (out1 - out4) set to LOW
        delay(speed);
    }
}

void closeGate()
{
    for(int i = 0; i < nSteps; i++)
    {
        PORTC ^= B00000100; // Change state of PC2 to HIGH
        delay(speed);
        PORTC ^= B00001100; // Change state of PC3 to HIGH & change PC2 to LOW
        delay(speed);
        PORTC ^= B00011000; // Change state of PC4 to HIGH & change PC3 to LOW
        delay(speed);
        PORTC ^= B00110000; // Change state of PC5 to HIGH & change PC4 to LOW
        delay(speed);
        PORTC &= B11000011; // Output pins PC2 - PC5 (out1 - out4) set to LOW
        delay(speed);
    }
}

void loop()
{
    openGate();                 // Execute forward test
    delay(1000);
    closeGate();                // Execute backward test
    
    PORTC &= B11000011;         // Turn LOW output pins (out1 - out4)
    
    // Finish the test program
    while(true)
    {
        PORTB ^= (1 << LED);  // Toggle LED pin, left the others untouched
        delay(2000);
        PORTB ^= (1 << LED);  // Toggle LED pin, left the others untouched
        delay(2000);
    }
}
