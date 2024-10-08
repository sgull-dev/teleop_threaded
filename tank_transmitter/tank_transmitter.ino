#include "Arduino.h"
#include "SPI.h" 
#include "RF24.h" 
#include "nRF24L01.h"
#define CE_PIN 9 
#define CSN_PIN 10 
#define INTERVAL_MS_TRANSMISSION 250

RF24 radio(CE_PIN, CSN_PIN); 
const byte address[6] = "00001"; 
//NRF24L01 buffer limit is 32 bytes (max struct size) 
struct payload { 
   int data1; 
   int data2; 
}; 
payload payload;


// Joystick Pins
const int joystick1X = A4;
const int joystick2X = A5;

// Data
int x1 = 0;
int x2 = 0;

void setup() {
  
   // transmitter adruino code
   
   Serial.begin(115200); 
   radio.begin(); 
   //Append ACK packet from the receiving radio back to the transmitting radio 
   radio.setAutoAck(false); //(true|false) 
   //Set the transmission datarate 
   radio.setDataRate(RF24_250KBPS); //(RF24_250KBPS|RF24_1MBPS|RF24_2MBPS) 
   //Greater level = more consumption = longer distance 
   radio.setPALevel(RF24_PA_MAX); //(RF24_PA_MIN|RF24_PA_LOW|RF24_PA_HIGH|RF24_PA_MAX) 
   //Default value is the maximum 32 bytes 
   radio.setPayloadSize(sizeof(payload)); 
   //Act as transmitter 
   radio.openWritingPipe(address); 
   radio.stopListening(); 
}

void loop () {
    // Joystick value in int [0, 1023]
    x1 = analogRead(joystick1X);
    x2 = analogRead(joystick2X);

    /*
    Serial.print("Joystick X: ");
    Serial.println(x);
    Serial.print("Joystick Y: ");
    Serial.println(y);
    */
   
   payload.data1 = x1; 
   payload.data2 = x2; 
   radio.write(&payload, sizeof(payload)); 
   Serial.print("joystick 1:"); 
   Serial.println(payload.data1); 
   Serial.print("joystick 2:"); 
   Serial.println(payload.data2); 
   Serial.println("Sent"); 
   delay(INTERVAL_MS_TRANSMISSION);  
}
