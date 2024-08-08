const int pwmPin = 3;
const int pwmPinRead = 5;
unsigned long pulseWidth;

void setup() {
  Serial.begin(9600);
  pinMode(pwmPin, OUTPUT);
  pinMode(pwmPinRead, INPUT);
}

void loop() {
  // f = 490Hz, T = 1/f = 0.00204s = 2.04ms for the full period.
  // for 50% duty cycle, we take T/2 in us which is approx 1020 microseconds.
  analogWrite(pwmPin, 128);  
  delay(5000);

  pulseWidth = pulseIn(pwmPinRead, HIGH);
  Serial.print("Pulse Width: ");
  Serial.print(pulseWidth);
  Serial.println(" microseconds");

  // For 100% duty cycle it should be fully 0.00204s or 2040 microseconds.
  // There is an error that this reads 0us instead of 2020, I don't know why right now.
  analogWrite(pwmPin, 191); // 75% Duty cycle, should output 1.53 milliseconds
  delay(1000);

  pulseWidth = pulseIn(pwmPinRead, HIGH); 
  Serial.print("Pulse Width: ");
  Serial.print(pulseWidth);
  Serial.println(" microseconds");


  // for 100% duty cycle it should be fully 0.00204s or 2040 microseconds.
  // There is an error that this reads 0us instead of 2020, I don't know why right now.
  analogWrite(pwmPin, 254); // 100% Duty cycle, cannot use 255, outputs 2020 or 2.02ms
  delay(1000);

  pulseWidth = pulseIn(pwmPinRead, HIGH); 
  Serial.print("Pulse Width: ");
  Serial.print(pulseWidth);
  Serial.println(" microseconds");
  delay(1000);               
}
