#include "Arduino.h"
#define DEAD_ZONE 15

// Joystick Pins
// TODO: Remove and replace by Radio/BT Input signal
const int joystickX = A4;
const int joystickY = A5;

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

    //Debugging
    //Serial.begin(9600);
}

void loop () {
    // Joystick value in int [0, 1023]
    x = analogRead(joystickX);
    y = analogRead(joystickY);

    // If Input is less than 511-DEAD_ZONE, it should move back
    // Else if Input is more than 511 + DEAD_ZONE, it should move forward
    // Else stationary
    // If moving, convert the 512-range value into PWM signal for motor speed control

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

    /*
    Serial.print("Joystick X: ");
    Serial.println(x);
    Serial.print("Joystick Y: ");
    Serial.println(y);
    */
    
    delay(100);  
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
