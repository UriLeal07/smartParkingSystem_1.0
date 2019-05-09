/************************************************************************************
* Description: Communication test between arduino Nano and Nodemcu over I2C protocol
* using timers to update a variable that it is send when Nodemcu request it.
* Developer: Uri Leal
* Board: Arduino Nano/Uno
* I/O pins: A4,A5
* 
* GNU GENERAL PUBLIC LICENSE Version 3, 29 June 2007
* Copyright (C) 2007 Free Software Foundation, Inc. <https://fsf.org/>
************************************************************************************/

#include <Wire.h>
#include <avr/io.h>
#include <avr/interrupt.h>

#define ADDRESS 0x03                 // Slave device address 0x00 to 0x80
String recBuffer;                    // Buffer used to read the message
volatile byte info;                  // Example byte of information
volatile bool doingTask;             // Boolean variable to avoid overlapping task
byte randNumber;

void setup()
{
    Wire.begin(ADDRESS);                        // Join I2C bus with a given address
    Wire.onReceive(receiveEvent);               // Link receive event to a function
    Wire.onRequest(requestEvent);               // Link request event to a function
    
    recBuffer.reserve(10);                      // Reserve 10 bytes of memory for the buffer string
    Serial.begin(9600);                         // Begin serial for debug
    randomSeed(analogRead(A7));                 // For testing propouses
    
    doingTask = false;                          // Initialize variables
    recBuffer = "";

    info = B00000000;
    
    Serial.print("Arduino slave started!\n\n");
    
    timer_init();                               // Initialize Timer
}

void loop()
{
      // Do something else
}

void receiveEvent(int nBytes)
{
    while (0 < Wire.available())                // Read & store the data temporarily in the buffer
        recBuffer.concat(Wire.read());
    
    Serial.print(recBuffer);                    // Show message through serial
    Serial.print(" recieved!\n");
    recBuffer = "";                             // Clear buffer
}

void requestEvent()
{
    Wire.write(info);                // Send byte on request
    
    Serial.print(info, BIN);        // Show the data that was sent through serial
    Serial.print(" sent!\n");
}

// Interrupt Service Routine is executed every 1s
ISR(TIMER1_OVF_vect)
{
    if(!doingTask)
    {
        doingTask = true;
        //stopTimer();
        //noInterrupts();
        
        randNumber = random(0, 7);
        
        if(bitRead(info, randNumber))       // if the bit is one
            info &= ~(1 << randNumber);     // Set a cero in the given position
        else
            info |= (1 << randNumber);      // Set a one in the given position
        
        //interrupts();
        //startTimer();
        TCNT1 = 3036;                       // Set counter in 3036
        doingTask = false;
    }
}

void timer_init()
{
    startTimer();
    sei();                        // Enable global interrupts
}

void stopTimer()
{
    TCCR1B &= ~(1 << CS12);         // Turn off Timer1
    TIMSK1 &= ~(1 << TOIE1);        // Disable Timer1 overflow flag
}

void startTimer()
{
    // 65536 - System clock / (Preescaler * Desired Frequency)
    //
    // 65536 - 16MHz / (256 * 1s) = 3036
    // 3036 is the indicated value to get 1s timer interrupt
    
    TCCR1A = 0;
    TCCR1B = 0;                      // Set timers control registers all to 0
    TCNT1 = 3036;                    // Set counter in 3036
    TCCR1B |= (1 << CS12);           // Set prescaler of 256
    TIMSK1 |= (1 << TOIE1);          // Enable timer1 overflow flag
}
