/***********************************************************************
* Ultrasonic sensor HC-SR04 library
* Developer: Uri Leal C.
* 
* GNU GENERAL PUBLIC LICENSE Version 3, 29 June 2007
* Copyright (C) 2007 Free Software Foundation, Inc. <https://fsf.org/>
************************************************************************/

#include <Arduino.h>
#include "UDS.h"

UDS::UDS(byte triggerPin, byte echoPin, byte ledPin1, byte ledPin2, int threshold)
{
    // Prevent unreachable limits
    if(threshold <= 0)
        threshold = 5;
    else if(threshold > 400)
        threshold = 400;
    
    this->triggerPin = triggerPin;
    this->echoPin = echoPin;
    this->ledPin1 = ledPin1;
    this->ledPin2 = ledPin2;
    this->threshold = threshold;
    ledOutput = true;                 // Output to LED pins active
    
    pinMode(triggerPin, OUTPUT);
    pinMode(echoPin, INPUT);
    pinMode(ledPin1, OUTPUT);   // Green led pin
    pinMode(ledPin2, OUTPUT);   // Red led pin
}

UDS::UDS(byte triggerPin, byte echoPin, byte ledPin1, byte ledPin2)
{
    this->triggerPin = triggerPin;
    this->echoPin = echoPin;
    this->ledPin1 = ledPin1;
    this->ledPin2 = ledPin2;
    threshold = 15;           // Default threshold
    ledOutput = true;         // Output to LED pins active
    
    pinMode(triggerPin, OUTPUT);
    pinMode(echoPin, INPUT);
    pinMode(ledPin1, OUTPUT);   // Green led pin
    pinMode(ledPin2, OUTPUT);   // Red led pin
}

UDS::UDS(byte triggerPin, byte echoPin)
{
    this->triggerPin = triggerPin;
    this->echoPin = echoPin;
    this->ledPin1 = ledPin1;
    this->ledPin2 = ledPin2;
    threshold = 0;            // Needless
    ledOutput = false;        // Output to LED pins inactive
    
    pinMode(triggerPin, OUTPUT);
    pinMode(echoPin, INPUT);
}

void UDS::checkDistance()
{
    // Calculate the distance and convert it to centimeters
    float distanceCm = calculatePulseDuration() / 58;
    
    if(ledOutput)
    {
        // Turn on/off the LED acording to the threshold value
        if(distanceCm >= 0 && distanceCm <= threshold)
        {
            digitalWrite(ledPin2, LOW);       // Turn off red led
            digitalWrite(ledPin1, HIGH);      // Turn on green led
        }
        else // the value is higher than the threshold or is out of bounds
        {
            digitalWrite(ledPin1, LOW);         // Turn off green led
            digitalWrite(ledPin2, HIGH);        // Turn on red led
        }
    }
}

float UDS::getDistanceCm()
{
    // Calculate the distance and convert it to centimeters
    float distanceCm = calculatePulseDuration() / 58;
    
    // Incorrect reading
    if (distanceCm <= 0 || distanceCm > 400)
        return -1.0 ;    
    
    return distanceCm;
}

bool UDS::isThresholdReached()
{
    // Calculate the distance and convert it to centimeters
    float distanceCm = calculatePulseDuration() / 58; 
    
    // Return true if the threshold is reached otherwise return false
    return (distanceCm >= 0 && distanceCm <= threshold);
}

unsigned long UDS::calculatePulseDuration()
{
    // Make sure that trigger pin is LOW.
    digitalWrite(triggerPin, LOW);
    delayMicroseconds(5);
    
    // Hold trigger for 10 microseconds, which is signal for sensor to measure distance.
    digitalWrite(triggerPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(triggerPin, LOW);
    
    // Measure the length of echo signal, which is equal to the time needed for sound to go there and back. And return the result
    return pulseIn(echoPin, HIGH);
}