#include <SPI.h>
#include <RF24.h>

// CE and CSN pin assignments
RF24 radio(7, 8); // CE, CSN pins

const byte address[6] = "00001";
int joy1X, joy1Y, joy2X, joy2Y;

void setup() {
  // Set up Serial Monitor for debugging
  Serial.begin(9600);
  
  // Set SPI pins
  pinMode(9, INPUT);   // MISO as input
  pinMode(10, OUTPUT); // MOSI as output
  pinMode(11, OUTPUT); // SCK as output

  SPI.begin(); // Initialize SPI communication
  radio.begin(); // Initialize nRF24L01
  radio.openWritingPipe(address); // Open writing pipe with a specific address
  radio.setPALevel(RF24_PA_MIN); // Set power level
  radio.stopListening(); // Set nRF24L01 as transmitter
}

void loop() {
  // Read joystick positions
  joy1X = analogRead(A0); // Joystick 1 X-axis
  joy1Y = analogRead(A1); // Joystick 1 Y-axis
  joy2X = analogRead(A3); // Joystick 2 X-axis
  joy2Y = analogRead(A4); // Joystick 2 Y-axis
  
  // Create an array with joystick data
  int joystickData[4] = {joy1X, joy1Y, joy2X, joy2Y};

  // Print the joystick coordinates to Serial Monitor
  Serial.print("Joystick 1 - X: "); Serial.print(joy1X);
  Serial.print(" Y: "); Serial.println(joy1Y);
  Serial.print("Joystick 2 - X: "); Serial.print(joy2X);
  Serial.print(" Y: "); Serial.println(joy2Y);

  // Print the data being sent
  Serial.print("Sending: ");
  for(int i = 0; i < 4; i++) {
    Serial.print(joystickData[i]);
    if(i < 3) Serial.print(", ");
  }
  Serial.println();

  // Send joystick data over nRF24L01
  radio.write(&joystickData, sizeof(joystickData));
  delay(100);
}
