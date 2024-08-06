#include <Wire.h>
#include <ESP32Servo.h>

Servo motor1, motor2, motor3, motor4;
const int motor1Pin = 32;
const int motor2Pin = 33;
const int motor3Pin = 18; 
const int motor4Pin = 26;

void setup() {
  Serial.begin(115200);
  motor1.attach(motor1Pin);
  motor2.attach(motor2Pin);
  motor3.attach(motor3Pin);
  motor4.attach(motor4Pin);

  motor1.writeMicroseconds(1000);  
  motor2.writeMicroseconds(1000);  
  motor3.writeMicroseconds(1000);  
  motor4.writeMicroseconds(1000);  
}

void loop() {
    delay(3000);
    int motor1Speed = 2000;  
    int motor2Speed = 2000;  
    int motor3Speed = 2000; 
    int motor4Speed = 2000; 

    motor1.writeMicroseconds(motor1Speed);
    delay(1500);
    motor2.writeMicroseconds(motor2Speed);
    delay(1500);
    motor3.writeMicroseconds(motor3Speed);
    delay(1500);
    motor4.writeMicroseconds(motor4Speed);
    delay(1500);
}
