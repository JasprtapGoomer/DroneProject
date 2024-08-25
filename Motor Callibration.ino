#include <Wire.h>
#include <ESP32Servo.h>

Servo motor1, motor2, motor3, motor4;
const int motor1Pin = 16;
const int motor2Pin = 17;
const int motor3Pin = 18; 
const int motor4Pin = 19;

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

  for (int i =0; i<5; i++){
  //Setting all the motors to max throttle
  motor1.writeMicroseconds(2000);
  motor2.writeMicroseconds(2000);
  motor3.writeMicroseconds(2000);
  motor4.writeMicroseconds(2000);
  delay(2000);//running the motors for 2 secs at max throttle

  //setting all the motors to min throttle
  motor1.writeMicroseconds(1000);
  motor2.writeMicroseconds(1000);
  motor3.writeMicroseconds(1000);
  motor4.writeMicroseconds(1000);
  //turniong all the motors off and turning them on again
  }

  Serial.print("callibration done Successfully!!");
}

void loop() {

}
