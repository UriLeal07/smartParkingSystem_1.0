/************************************************************************************************
* Description: Main component of the system, upload the information to Firebase and communicates
* with the 3 slave arduinos
* Developer: Uri Leal
* Board: NodeMCU
* Sensors/Components: None
* I/O pins: D1, D2
* 
* GNU GENERAL PUBLIC LICENSE Version 3, 29 June 2007
* Copyright (C) 2007 Free Software Foundation, Inc. <https://fsf.org/>
************************************************************************************************/

#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>
#include <Wire.h>
#include <Ticker.h>

/////////////////////////////////////////////////////////////////////////////////////////
// WiFi and Firebase setup
/////////////////////////////////////////////////////////////////////////////////////////
#define FIREBASE_HOST "XXXXXXXXXXXXXXXXXXXX.firebaseio.com"                              // Hostname of the database
#define FIREBASE_AUTH "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx"                         // Authentication token
#define WIFI_SSID "MyWiFi_Name"                                                          // WiFi name
#define WIFI_PASSWORD "MyWiFi_PWD"                                                       // WiFi password

/////////////////////////////////////////////////////////////////////////////////////////
// Timer control objects to schedule tasks
/////////////////////////////////////////////////////////////////////////////////////////
Ticker acquire;                               // Timer to read from the I2C bus to acquire the data
Ticker update;                                // Timer to update the acquired data to Firebase
Ticker send;                                  // Timer to print the acquired data

/////////////////////////////////////////////////////////////////////////////////////////
// Address of the slave devices that collaborate in the system
/////////////////////////////////////////////////////////////////////////////////////////
#define SLAVE_ADDR1 0x02                      // First 4 spaces of the parking lot
#define SLAVE_ADDR2 0x03                      // Last 4 spaces of the parking lot
#define SLAVE_ADDR3 0x04                      // Control gates of the parking lot

byte buffer;                                  // Buffer used to read the message

volatile bool updating;                       // Boolean variable to avoid overlapping task

volatile byte spaces;                         // Contains which spaces are occupied and which are not

// Function prototypes
void updateData();
void requestData();
void sendData();

void setup()
{
    Serial.begin(9600);
    delayMicroseconds(10);
    
    // Set up WiFi
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    
    while(WiFi.status() != WL_CONNECTED)
        delay(500);
    
    Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);     // Setup connection
    
    Serial.println("WiFi Ready");
    // Set variable "var" with 0 as intial value
    Firebase.setInt("spaces", 0);
    
    if (Firebase.failed())
    {
        Serial.println("Firebase Error");
        delay(1500);
    }
    
    delay(500);
    
    Wire.begin(D1, D2);               // Join I2C bus with SDA = D1 and SCL = D2 of NodeMCU

    // Init variables
    updating = false;
    spaces = B00000000;
    
    update.attach_scheduled(1, updateData);                // Start scheduled timer to upload the acquired data to firebase every 1 second
    acquire.attach_scheduled(.5, requestData);             // Start scheduled timer to acquire the data every 1.5 seconds
    send.attach_scheduled(1.7, sendData);                  // Start scheduled timer to send the acquired data over I2C every 2 seconds
    
    Serial.println("Start!");
}

void loop()
{
    // Do other things
}

void updateData()
{
    updating = true;      // Work started
    
    Firebase.setInt("spaces", spaces);      // Upload to firebase
    Serial.print("Send: ");
    Serial.println(spaces);
    
    if (Firebase.failed())
    {
        Serial.print("Error: ");
        Serial.println(Firebase.error());
    }
    
    updating = false;     // Work finished
}

void requestData()
{
    if(!updating)    // Only if the updating task is not active
    {
        Wire.requestFrom(SLAVE_ADDR1, 1);               // Request data of size 1 from slave device #1
        
        while(Wire.available())                         // Read & store the data temporarily in the buffer
            buffer = Wire.read();
        
        // Read and set the first 4 bits of data from the first slave device to the spaces variable
        bitWrite(spaces, 0, (bitRead(buffer, 0)));
        bitWrite(spaces, 1, (bitRead(buffer, 1)));
        bitWrite(spaces, 2, (bitRead(buffer, 2)));
        bitWrite(spaces, 3, (bitRead(buffer, 3)));
        
        buffer = B00000000;                             // Clear buffer
        
        Wire.requestFrom(SLAVE_ADDR2, 1);               // Request data of size 1 from slave device #2
        
        while(Wire.available())                         // Read & store the data temporarily in the buffer
            buffer = Wire.read();
        
        // Read the first 4 bits of the buffer data from slave #2 and set it into the last 4 bits of the variable spaces
        bitWrite(spaces, 4, (bitRead(buffer, 0)));
        bitWrite(spaces, 5, (bitRead(buffer, 1)));
        bitWrite(spaces, 6, (bitRead(buffer, 2)));
        bitWrite(spaces, 7, (bitRead(buffer, 3)));
        
        buffer = B00000000;                          // Clear buffer
    }
}

void sendData()
{
    if(!updating)   // Only if the updating task is not active
    {
        Wire.beginTransmission(SLAVE_ADDR3);        // Begin communication
        Wire.write(spaces);                         // Send the data
        Wire.endTransmission();                     // Stop transmission
    }
}
