#include <Arduino.h>

const int trigPin = 22;
const int echoPin = 23;

unsigned long duration;
float distance;

const int left1 = 26;
const int left2 = 25;
const int right1 = 33;
const int right2 = 32; 

void setup() {
  // Setup Distance Sensor
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  Serial.begin(9600);
  //Setup motor control pins
  pinMode(left1, OUTPUT);
  pinMode(left2, OUTPUT);
  pinMode(right1, OUTPUT);
  pinMode(right2, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(trigPin, LOW);

  delayMicroseconds(2);

  digitalWrite(trigPin, HIGH);

  delayMicroseconds(10);

  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH);

  distance = (duration*0.0343)/2;

  Serial.print("Distance: ");
  Serial.println(distance);
  
  if (distance > 50) {
    go_forward();
  } else {
    turn_right();
  }
  delay(33);
}

void go_forward() {
  digitalWrite(left1, HIGH);
  digitalWrite(left2, LOW);

  digitalWrite(right1, HIGH);
  digitalWrite(right2, LOW);
}

void turn_right() {
  digitalWrite(left1, LOW);
  digitalWrite(left2, HIGH);

  digitalWrite(right1, HIGH);
  digitalWrite(right2, LOW);
}