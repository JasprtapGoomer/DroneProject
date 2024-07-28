//@author - avinaash
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <ESP32Servo.h>  // Include the ESP32Servo library

const char* ssid = "ESP-DRONE";
const char* password = "12345678";

AsyncWebServer server(80);

Servo motor1;
Servo motor2;
Servo motor3;
Servo motor4;

const int motor1Pin = 32;
const int motor2Pin = 33;
const int motor3Pin = 25;
const int motor4Pin = 26;

// Initial motor values
int thrust = 0;
int yaw = 0;
int roll = 0;
int pitch = 0;

void setup() {
  Serial.begin(115200);

  // Attach the motors to the pins
  motor1.attach(motor1Pin);
  motor2.attach(motor2Pin);
  motor3.attach(motor3Pin);
  motor4.attach(motor4Pin);

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
          .slider { width: 80%; margin: 10px; }
          .slider-value { font-size: 20px; }
          .button {
            font-size: 24px;
            padding: 15px 30px;
            color: white;
            background-color: red;
            border: none;
            border-radius: 5px;
            cursor: pointer;
          }
        </style>
      </head>
      <body>
        <h1>Drone Control</h1>
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
          <button class="button" onclick="startUp()">Start Up</button>
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
  });

  // Handle startup request
  server.on("/startup", HTTP_GET, [](AsyncWebServerRequest *request){
    // Spool up all motors to 100% speed for 3 seconds
    motor1.write(180);
    motor2.write(180);
    motor3.write(180);
    motor4.write(180);
    delay(3000);
    // Reset motors to initial state
    motor1.write(thrust);
    motor2.write(thrust);
    motor3.write(thrust);
    motor4.write(thrust);

    request->send(200, "text/plain", "Startup sequence completed");
    Serial.println("Startup sequence completed");
  });

  server.begin();
  Serial.println("Server started");
}

void loop() {
  // Nothing to do here
}
