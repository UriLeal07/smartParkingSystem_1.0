/***********************************************************************
* Ultrasonic sensor HC-SR04 library
* Developer: Uri Leal C.
* 
* GNU GENERAL PUBLIC LICENSE Version 3, 29 June 2007
* Copyright (C) 2007 Free Software Foundation, Inc. <https://fsf.org/>
************************************************************************/

#ifndef UDS_h
#define UDS_h

#include <Arduino.h>

class UDS
{
    public:
       /* Constructors
        * @param triggerPin [OUTPUT]: Digital pin that is used to trigger the ultrasonic waves of the sensor
        * @param echoPin    [ INPUT]: Digital pin that is used to recieve the pulse-wave to determine the distance
        * @param ledPin1/2  [OUTPUT]: Digital pin that is used to indicate (with a given threshold value) if the sensor reached the stablished limit or not yet
        * @param threshold  [OUTPUT]: The reference value used to toggle the LED's on/off
        */
        UDS(byte triggerPin, byte echoPin, byte ledPin1, byte ledPin2, int threshold);
        UDS(byte triggerPin, byte echoPin, byte ledPin1, byte ledPin2);
        UDS(byte triggerPin, byte echoPin);
        
        void checkDistance();               // Calculate and check the distance of the object and turn on/off the LED's acording to the given threshold value
        float getDistanceCm();              // Obtain the distance in centimeters between the sensor and an object situated on a valid detect-range of the sensor
        bool isThresholdReached();          // Check if the threshold value was reached
        
    private:
    
        int threshold;                                    // Reference value used to toggle the LED's on/off set by the user or by default in 15
        unsigned long calculatePulseDuration();           // Core funtion that controls the sensor directly to obtain a pulse wave duration that represents the distance between the object and the sensor
        byte ledPin1, ledPin2, triggerPin, echoPin;       // I/O Pins for the sensor an LED's
        bool ledOutput;                                   // Boolean variable that determines if the LED pins outputs are gonna be used
};

#endif