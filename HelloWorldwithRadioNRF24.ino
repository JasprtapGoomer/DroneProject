#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

RF24 radio(7, 8);  // CE, CSN
RF24 radio1(9, 10); // CE, CSN

const byte address[6] = "00001";

void setup() {
  Serial.begin(9600);  // Initialize serial communication
  
  // First radio as Transmitter
  radio.begin();
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_MIN);
  radio.stopListening();
  
  // Second radio as receiver
  radio1.begin();
  radio1.openReadingPipe(0, address);
  radio1.setPALevel(RF24_PA_MIN);
  radio1.startListening();
}

void loop() {
  const char text[] = "Hello World";
  
  // Transmit with first radio
  radio.write(&text, sizeof(text));
  

  
  // If data has been received, printout received text
  if (radio1.available()) {
    char receivedText[32] = "";
    radio1.read(&receivedText, sizeof(receivedText));
    Serial.println(receivedText);
  }
  
  delay(1000);
}
