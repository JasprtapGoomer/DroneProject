// Define PWM output pin and read pin
const int pwmOutputPin = 5;  // PWM output pin
const int pwmReadPin = 4;    // PWM read pin

// Variables to store PWM readings
unsigned long highTime;
unsigned long lowTime;

void setup() {
  // Start the Serial Monitor
  Serial.begin(115200);

  // Configure the PWM output pin
  analogWrite(pwmOutputPin, 128);  // 50% duty cycle

  // Configure the read pin as input
  pinMode(pwmReadPin, INPUT);
}

void loop() {
  // Generate PWM signal on the output pin
  analogWrite(pwmOutputPin, 128);  // 50% duty cycle

  // Read the PWM high time
  highTime = pulseIn(pwmReadPin, HIGH);
  // Read the PWM low time
  lowTime = pulseIn(pwmReadPin, LOW);

  // Calculate the PWM duty cycle percentage
  float dutyCycle = (highTime * 100.0) / (highTime + lowTime);

  // Print the PWM high time, low time, and duty cycle to the Serial Monitor
  Serial.print("High Time: ");
  Serial.print(highTime);
  Serial.print("\tLow Time: ");
  Serial.print(lowTime);
  Serial.print("\tDuty Cycle: ");
  Serial.print(dutyCycle);
  Serial.println("%");

  // Delay for a short period to avoid flooding the Serial Monitor
  delay(500);
}
