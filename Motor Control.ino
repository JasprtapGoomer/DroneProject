Jaspartap
jaspartap.goomer
Online

Kaushik — 07/26/2024 2:20 PM
thats some phd level shit
Victor — 07/26/2024 2:21 PM
I already have a phd
ask Avinaash
pretty huge dick
Kaushik — 07/26/2024 2:21 PM
wow thats amazing! 
Victor — 07/26/2024 2:22 PM
I know right
Victor — 07/27/2024 12:15 PM
Just so you know guys, when you're programming to prototype with Arduino, you can use the Servo.h library to control them
it should make things a lot simpler
I also believe that the gyroscope Im giving you tomorrow has its own library as well, MPU6050
But im not sure how we can connect multiple of them due to arduino pin limit, I guess for now we can do one in the center of the drone and use that information for control
The main challenge will probably be taking input PWM signals from the receiver of the RC kit and mapping it out to something useful on the drone. Especially since that RC kit is used for mainly small planes, not quadcopters.
Victor — 07/27/2024 4:10 PM
Arduino register level programming code snippet for I2C:

Wire.beginTransmission(/* address */);
Wire.write(/* register */);
Wire.endTransmission();

Wire.requestFrom(/* address */, 1);
If (Wire.available) {
  int data = Wire.read();
}
 
Victor — Yesterday at 3:39 PM
@Avinaash are you available tonight at 7?
Victor — Yesterday at 3:58 PM
Great @Kaushik
Kaushik — Yesterday at 4:03 PM
i won't be available tonight
Avinaash — Yesterday at 4:04 PM
How about Saturday? @Victor @Kaushik
Jaspartap — Yesterday at 4:05 PM
I can’t do Saturday, I can do today
Kaushik — Yesterday at 4:05 PM
I can do Sunday
Jaspartap — Yesterday at 4:05 PM
That works too
Avinaash — Yesterday at 4:05 PM
Yeah Sunday works for me
Victor — Yesterday at 4:07 PM
You’re fine I just wanted the vc lol
Kaushik — Yesterday at 4:07 PM
oh lol
Victor — Yesterday at 4:07 PM
Brooo 😭
Kaushik — Yesterday at 4:07 PM
aight then y'all meet tonight if @Victor can't make it on Sunday
Victor — Yesterday at 4:07 PM
That’s fine tho it’s not gonna be anything super important, I can just send you the stuff in a message
Kaushik — Yesterday at 4:08 PM
sure sounds good
Victor — Yesterday at 4:08 PM
I prefer to talk to 1 guy atleast tho lol
Kaushik — Yesterday at 4:08 PM
just send a text in this channel
Victor — Yesterday at 4:08 PM
yeah fo sho
Jaspartap — Yesterday at 4:08 PM
I will join in around 7
Victor — Yesterday at 4:14 PM
Fiya
Victor — Yesterday at 6:59 PM
@Jaspartap
let me know when you're in the VC
Jaspartap — Yesterday at 7:18 PM
can you hear me
Victor — Yesterday at 7:18 PM
I can't hear you
are you on pc
Jaspartap — Yesterday at 7:20 PM
yeah
Victor — Yesterday at 7:20 PM
Join phone?
Jaspartap — Yesterday at 7:20 PM
probably lemme connect my mac
Victor — Yesterday at 7:20 PM
Ok
Brodie
Im down to do a phone call at this point
fawk
just give me your cell number
Jaspartap — Yesterday at 7:28 PM
yeah i am down
i cant hear you
Victor — Yesterday at 7:28 PM
Ok
DM me
Avinaash — Today at 11:22 AM
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <ESP32Servo.h>
#include <Wire.h>
#include "rgb_lcd.h"

const char* ssid = "ESP-DRONE";
const char* password = "12345678";

AsyncWebServer server(80);
Servo motor1, motor2, motor3, motor4;

const int motor1Pin = 32;
const int motor2Pin = 33;
const int motor3Pin = 18; // Changed pin for motor3
const int motor4Pin = 26;
const int speakerPin = 27;

int thrust = 0;
int yaw = 0;
int roll = 0;
int pitch = 0;

int previousRPM = 0;

rgb_lcd lcd;

void setup() {
  Serial.begin(115200);

  // Attach the motors to the pins
  motor1.attach(motor1Pin);
  motor2.attach(motor2Pin);
  motor3.attach(motor3Pin);
  motor4.attach(motor4Pin);

  pinMode(speakerPin, OUTPUT);

  // Initialize the LCD
  lcd.begin(16, 2); // Initialize the 16x2 LCD
  lcd.setRGB(255, 255, 255); // Set LCD backlight to white

  WiFi.softAP(ssid, password);
  IPAddress IP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(IP);

  // Serve the HTML page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(200, "text/html", R"rawliteral(
      <!DOCTYPE html>
      <html>
      <head>
        <title>Drone Control</title>
        <style>
          body { font-family: Arial; text-align: center; }
          .slider { width: 80%; height: 40px; margin: 20px; }
          .slider-value { font-size: 24px; }
          .button {
            font-size: 24px;
            padding: 15px 30px;
            color: white;
            border: none;
            border-radius: 5px;
            cursor: pointer;
          }
          .button-start {
            background-color: green;
          }
          .button-stop {
            background-color: red;
          }
          .rpm-display {
            font-size: 18px;
            margin: 10px 0;
          }
        </style>
      </head>
      <body>
        <h1>Drone Control</h1>
        <div class="rpm-display">
          <p>Motor 1 RPM: <span id="motor1RPM">0</span></p>
          <p>Motor 2 RPM: <span id="motor2RPM">0</span></p>
          <p>Motor 3 RPM: <span id="motor3RPM">0</span></p>
          <p>Motor 4 RPM: <span id="motor4RPM">0</span></p>
        </div>
        <div>
          <label>Thrust: <span id="thrustValue">0</span></label><br>
          <input type="range" id="thrust" class="slider" min="0" max="180" value="0" oninput="updateSlider('thrust')"><br>
          
          <label>Yaw: <span id="yawValue">0</span></label><br>
          <input type="range" id="yaw" class="slider" min="0" max="180" value="90" oninput="updateSlider('yaw')"><br>
          
          <label>Roll: <span id="rollValue">0</span></label><br>
          <input type="range" id="roll" class="slider" min="0" max="180" value="90" oninput="updateSlider('roll')"><br>
          
          <label>Pitch: <span id="pitchValue">0</span></label><br>
          <input type="range" id="pitch" class="slider" min="0" max="180" value="90" oninput="updateSlider('pitch')"><br>
        </div>
... (188 lines left)
Collapse
newdronecode.ino
9 KB
@Jaspartap this is the current drone code
﻿
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <ESP32Servo.h>
#include <Wire.h>
#include "rgb_lcd.h"

const char* ssid = "ESP-DRONE";
const char* password = "12345678";

AsyncWebServer server(80);
Servo motor1, motor2, motor3, motor4;

const int motor1Pin = 32;
const int motor2Pin = 33;
const int motor3Pin = 18; // Changed pin for motor3
const int motor4Pin = 26;
const int speakerPin = 27;

int thrust = 0;
int yaw = 0;
int roll = 0;
int pitch = 0;

int previousRPM = 0;

rgb_lcd lcd;

void setup() {
  Serial.begin(115200);

  // Attach the motors to the pins
  motor1.attach(motor1Pin);
  motor2.attach(motor2Pin);
  motor3.attach(motor3Pin);
  motor4.attach(motor4Pin);

  pinMode(speakerPin, OUTPUT);

  // Initialize the LCD
  lcd.begin(16, 2); // Initialize the 16x2 LCD
  lcd.setRGB(255, 255, 255); // Set LCD backlight to white

  WiFi.softAP(ssid, password);
  IPAddress IP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(IP);

  // Serve the HTML page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(200, "text/html", R"rawliteral(
      <!DOCTYPE html>
      <html>
      <head>
        <title>Drone Control</title>
        <style>
          body { font-family: Arial; text-align: center; }
          .slider { width: 80%; height: 40px; margin: 20px; }
          .slider-value { font-size: 24px; }
          .button {
            font-size: 24px;
            padding: 15px 30px;
            color: white;
            border: none;
            border-radius: 5px;
            cursor: pointer;
          }
          .button-start {
            background-color: green;
          }
          .button-stop {
            background-color: red;
          }
          .rpm-display {
            font-size: 18px;
            margin: 10px 0;
          }
        </style>
      </head>
      <body>
        <h1>Drone Control</h1>
        <div class="rpm-display">
          <p>Motor 1 RPM: <span id="motor1RPM">0</span></p>
          <p>Motor 2 RPM: <span id="motor2RPM">0</span></p>
          <p>Motor 3 RPM: <span id="motor3RPM">0</span></p>
          <p>Motor 4 RPM: <span id="motor4RPM">0</span></p>
        </div>
        <div>
          <label>Thrust: <span id="thrustValue">0</span></label><br>
          <input type="range" id="thrust" class="slider" min="0" max="180" value="0" oninput="updateSlider('thrust')"><br>
          
          <label>Yaw: <span id="yawValue">0</span></label><br>
          <input type="range" id="yaw" class="slider" min="0" max="180" value="90" oninput="updateSlider('yaw')"><br>
          
          <label>Roll: <span id="rollValue">0</span></label><br>
          <input type="range" id="roll" class="slider" min="0" max="180" value="90" oninput="updateSlider('roll')"><br>
          
          <label>Pitch: <span id="pitchValue">0</span></label><br>
          <input type="range" id="pitch" class="slider" min="0" max="180" value="90" oninput="updateSlider('pitch')"><br>
        </div>
        <div>
          <button class="button button-start" onclick="startUp()">Ignition</button>
          <button class="button button-stop" onclick="shutOff()">Shut Off</button>
        </div>
        
        <script>
          function updateSlider(id) {
            var slider = document.getElementById(id);
            var output = document.getElementById(id + 'Value');
            output.innerHTML = slider.value;
            fetch(`/control?${id}=` + slider.value);
          }
          
          function startUp() {
            fetch('/startup');
          }
          
          function shutOff() {
            fetch('/shutdown');
          }

          function updateRPMs(motor1, motor2, motor3, motor4) {
            document.getElementById('motor1RPM').innerText = motor1;
            document.getElementById('motor2RPM').innerText = motor2;
            document.getElementById('motor3RPM').innerText = motor3;
            document.getElementById('motor4RPM').innerText = motor4;
          }

          setInterval(() => {
            fetch('/rpms')
              .then(response => response.json())
              .then(data => {
                updateRPMs(data.motor1, data.motor2, data.motor3, data.motor4);
              });
          }, 1000);
        </script>
      </body>
      </html>
    )rawliteral");
  });

  // Handle control requests
  server.on("/control", HTTP_GET, [](AsyncWebServerRequest *request){
    if (request->hasParam("thrust")) {
      thrust = request->getParam("thrust")->value().toInt();
    }
    if (request->hasParam("yaw")) {
      yaw = request->getParam("yaw")->value().toInt();
    }
    if (request->hasParam("roll")) {
      roll = request->getParam("roll")->value().toInt();
    }
    if (request->hasParam("pitch")) {
      pitch = request->getParam("pitch")->value().toInt();
    }

    // Calculate motor speeds based on thrust, yaw, roll, and pitch
    int motor1Speed = constrain(thrust + yaw + roll + pitch, 0, 180);
    int motor2Speed = constrain(thrust - yaw - roll + pitch, 0, 180);
    int motor3Speed = constrain(thrust - yaw + roll - pitch, 0, 180);
    int motor4Speed = constrain(thrust + yaw - roll - pitch, 0, 180);

    motor1.write(motor1Speed);
    motor2.write(motor2Speed);
    motor3.write(motor3Speed);
    motor4.write(motor4Speed);

    request->send(200, "text/plain", "Control updated");
    Serial.println("Control updated with values:");
    Serial.print("Thrust: "); Serial.println(thrust);
    Serial.print("Yaw: "); Serial.println(yaw);
    Serial.print("Roll: "); Serial.println(roll);
    Serial.print("Pitch: "); Serial.println(pitch);
    
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("T:"); lcd.print(thrust);
    lcd.setCursor(0, 1);
    lcd.print("Y:"); lcd.print(yaw);
    lcd.print(" R:"); lcd.print(roll);
    lcd.print(" P:"); lcd.print(pitch);
  });

  // Handle startup request
  server.on("/startup", HTTP_GET, [](AsyncWebServerRequest *request){
    // Calibrate ESCs
    motor1.write(180);
    motor2.write(180);
    motor3.write(180);
    motor4.write(180);
    tone(speakerPin, 2000); // Play a tone for calibration
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Calibrating ESCs");
    delay(2000);
    
    motor1.write(0);
    motor2.write(0);
    motor3.write(0);
    motor4.write(0);
    noTone(speakerPin); // Stop the tone
    lcd.setCursor(0, 1);
    lcd.print("ESCs Calibrated");
    delay(2000);

    // Spool up all motors to 100% speed for 3 seconds
    motor1.write(180);
    motor2.write(180);
    motor3.write(180);
    motor4.write(180);
    tone(speakerPin, 1000); // Play a tone on startup
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Startup sequence");
    lcd.setCursor(0, 1);
    lcd.print("initiated");
    delay(3000);
    noTone(speakerPin); // Stop the tone
    lcd.clear();
    // Reset motors to initial state
    motor1.write(thrust);
    motor2.write(thrust);
    motor3.write(thrust);
    motor4.write(thrust);

    request->send(200, "text/plain", "Startup sequence completed");
    Serial.println("Startup sequence completed");
  });

  // Handle shutdown request
  server.on("/shutdown", HTTP_GET, [](AsyncWebServerRequest *request){
    thrust = 0;
    motor1.write(thrust);
    motor2.write(thrust);
    motor3.write(thrust);
    motor4.write(thrust);
    tone(speakerPin, 500); // Play a tone on shutdown
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Thrust Zero,");
    lcd.setCursor(0, 1);
    lcd.print("Shutting Down");
    delay(3000);
    noTone(speakerPin); // Stop the tone
    lcd.clear();

    request->send(200, "text/plain", "Shutdown sequence completed");
    Serial.println("Shutdown sequence completed");
  });

  // Handle RPMs request
  server.on("/rpms", HTTP_GET, [](AsyncWebServerRequest *request){
    int motor1RPM = map(motor1.read(), 0, 180, 0, 10000); // Example mapping
    int motor2RPM = map(motor2.read(), 0, 180, 0, 10000);
    int motor3RPM = map(motor3.read(), 0, 180, 0, 10000);
    int motor4RPM = map(motor4.read(), 0, 180, 0, 10000);

    String json = "{";
    json += "\"motor1\":" + String(motor1RPM) + ",";
    json += "\"motor2\":" + String(motor2RPM) + ",";
    json += "\"motor3\":" + String(motor3RPM) + ",";
    json += "\"motor4\":" + String(motor4RPM);
    json += "}";

    request->send(200, "application/json", json);

    // Check if RPM has increased by 1000
    int currentRPM = max(motor1RPM, max(motor2RPM, max(motor3RPM, motor4RPM)));
    if (currentRPM / 1000 > previousRPM / 1000) {
      String rpmMessage = "Current RPM: " + String(currentRPM);
      tone(speakerPin, 1500); // Play a tone for RPM announcement
      delay(1000);
      noTone(speakerPin);
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print(rpmMessage);
      previousRPM = currentRPM;
    }
  });

  server.begin();
  Serial.println("Server started");
}

void loop() {
  // Nothing to do here
}
