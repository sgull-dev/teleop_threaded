#include "Arduino.h"
#include "SPI.h" 
#include "RF24.h" 
#include "nRF24L01.h" 
#define DEAD_ZONE 15
#define CE_PIN 9 
#define CSN_PIN 10 
#define INTERVAL_MS_SIGNAL_LOST 1000 
#define INTERVAL_MS_SIGNAL_RETRY 250 

RF24 radio(CE_PIN, CSN_PIN); 
const byte address[6] = "00001"; 
//NRF24L01 buffer limit is 32 bytes (max struct size) 
struct payload { 
   int data1; 
   int data2; 
}; 
payload payload; 
unsigned long lastSignalMillis = 0; 


// L298N Motor Controller Pins
const int left1 = 2;
const int left2 = 4;
const int leftPWM = 3;
const int right1 = 5;
const int right2 = 7;
const int rightPWM = 6;

// Data
int x = 0;
int y = 0;

void setup() {

    Serial.begin(115200); 
    radio.begin(); 
    //Append ACK packet from the receiving radio back to the transmitting radio 
    radio.setAutoAck(false); //(true|false) 
    //Set the transmission datarate 
    radio.setDataRate(RF24_250KBPS); //(RF24_250KBPS|RF24_1MBPS|RF24_2MBPS) 
    //Greater level = more consumption = longer distance 
    radio.setPALevel(RF24_PA_MIN); //(RF24_PA_MIN|RF24_PA_LOW|RF24_PA_HIGH|RF24_PA_MAX) 
    //Default value is the maximum 32 bytes1 
    radio.setPayloadSize(sizeof(payload)); 
    //Act as receiver 
    radio.openReadingPipe(0, address); 
    radio.startListening(); 
   
    // Set all the motor control pins to outputs
    pinMode(leftPWM, OUTPUT);
    pinMode(rightPWM, OUTPUT);
    pinMode(left1, OUTPUT);
    pinMode(left2, OUTPUT);
    pinMode(right1, OUTPUT);
    pinMode(right2, OUTPUT);
    
    // Turn off motors - Initial state
    digitalWrite(left1, LOW);
    digitalWrite(left2, LOW);
    digitalWrite(right1, LOW);
    digitalWrite(right2, LOW);


}

void loop () {
  
   unsigned long currentMillis = millis(); 
   if (radio.available() > 0) { 
     radio.read(&payload, sizeof(payload)); 
     Serial.println("Received"); 
     Serial.print("x-axis:"); 
     Serial.println(payload.data1);
     Serial.print("y-axis:"); 
     Serial.println(payload.data2);
     lastSignalMillis = currentMillis;
     x = payload.data1
     y = payload.data2
   } 
   if (currentMillis - lastSignalMillis > INTERVAL_MS_SIGNAL_LOST) { 
     lostConnection(); 
   }
   


    // Left Motor Control
    if (x < 511 - DEAD_ZONE) {
  left_back();
  int speed = (511-x)/2;
  analogWrite(leftPWM, speed);
    } else if (x > 511 + DEAD_ZONE) {
  left_forward();
  int speed = (x-512)/2;
  analogWrite(leftPWM, speed);
    } else {
  left_stop();
    }

    // Right motor control 
    if (y < 511 - DEAD_ZONE) {
        right_back();
        int speed = (511 - y) / 2;
        analogWrite(rightPWM, speed);
    } else if (y > 511 + DEAD_ZONE) {
        right_forward();
        int speed = (y - 512) / 2;
        analogWrite(rightPWM, speed);
    } else {
        right_stop();
    }
    
    delay(100);  
}

void lostConnection() 
{ 
   Serial.println("We have lost connection, preventing unwanted behavior"); 
   delay(INTERVAL_MS_SIGNAL_RETRY); 
} 


// Functions for controlling motor directions
void left_forward() {
    digitalWrite(left1, HIGH);
    digitalWrite(left2, LOW);
}

void left_back() {
    digitalWrite(left1, LOW);
    digitalWrite(left2, HIGH);

}

void left_stop() {
    digitalWrite(left1, LOW);
    digitalWrite(left2, LOW);
}

void right_forward() {
    digitalWrite(right1, HIGH);
    digitalWrite(right2, LOW);
}

void right_back() {
    digitalWrite(right1, LOW);
    digitalWrite(right2, HIGH);
}

void right_stop() {
    digitalWrite(right1, LOW);
    digitalWrite(right2, LOW);
}
