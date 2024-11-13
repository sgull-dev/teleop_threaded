#include "Arduino.h"
#include "SPI.h" 
#include "RF24.h" 
#include "nRF24L01.h" 
#define DEAD_ZONE 15
#define CE_PIN 9 
#define CSN_PIN 10 
#define INTERVAL_MS_SIGNAL_LOST 300
#define INTERVAL_MS_SIGNAL_RETRY 125

RF24 radio(CE_PIN, CSN_PIN); 
const byte address[6] = "10101"; 
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
int x1 = 0;
int x2 = 0;
void setup() {
    Serial.begin(115200); 
    setupRadio();
    
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

void setupRadio() {
    radio.begin(); 
    radio.setAutoAck(false);
    radio.setDataRate(RF24_250KBPS);
    radio.setPALevel(RF24_PA_MIN);
    radio.setPayloadSize(sizeof(payload)); 
    radio.openReadingPipe(0, address); 
    radio.startListening(); 
}


void loop () {
  
   unsigned long currentMillis = millis(); 
   if (radio.available() > 0) { 
     radio.read(&payload, sizeof(payload)); 
     Serial.println("Received"); 
     Serial.print("Controller 1:"); 
     Serial.println(payload.data1);
     Serial.print("Controller 2:"); 
     Serial.println(payload.data2);
     lastSignalMillis = currentMillis;
     x1 = payload.data1;
     x2 = payload.data2;

    // Left Motor Control
    if (x1 < 511 - DEAD_ZONE) {
      left_back();
      int speed = (511-x1)/2;
      analogWrite(leftPWM, speed);
    } else if (x1 > 511 + DEAD_ZONE) {
      left_forward();
      int speed = (x1-512)/2;
      analogWrite(leftPWM, speed);
    } else {
      left_stop();
    }

    // Right motor control 
    if (x2 < 511 - DEAD_ZONE) {
        right_back();
        int speed = (511 - x2) / 2;
        analogWrite(rightPWM, speed);
    } else if (x2 > 511 + DEAD_ZONE) {
        right_forward();
        int speed = (x2 - 512) / 2;
        analogWrite(rightPWM, speed);
    } else {
        right_stop();
    }
    delay(30); 
   } 
   if (currentMillis - lastSignalMillis > INTERVAL_MS_SIGNAL_LOST) { 
     //Swapped these back this way; It's better if we stop the motors only after a attempt at reconnection
     // If we instantly get a reconnection then we see no noticeable disconnect
     // Otherwise If we instantly stop it on disconnect, we'll always see a slight pause on disconnect
     lostConnection();
     left_stop();
     right_stop();
   }
}

void lostConnection() 
{ 
   Serial.println("We have lost connection, preventing unwanted behavior");
   reattemptConnection();
   delay(INTERVAL_MS_SIGNAL_RETRY);
} 

void reattemptConnection() {
  //TODO: Could it be possible to do this without Power Cycling? Could be faster.

  Serial.println("Connection lost. Attempting to reconnect...");
  // Power cycle the radio
  radio.powerDown();
  delay(50);
  radio.powerUp();
  
  // Re-initialize the radio
  setupRadio();
  
  Serial.println("Radio reinitialized. Listening for signals...");
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
