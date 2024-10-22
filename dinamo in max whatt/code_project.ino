#include <Servo.h>  // Include the Servo library

const int currentPin = A0; // Pin for current sensor
const int voltPin = A1; // Pin for voltage sensor
const int hallPin = 2; // Pin for Hall effect sensor
const int servoPin = 9; // Pin for servo motor
volatile byte pulseCount = 0;
unsigned long lastTime = 0;
float speed = 0.0;
float speed_prev = 0.0;
float current = 0.0;
float voltage = 0.0;
float power = 0.0;
int servoPos = 270; // Initial servo position
unsigned long startTime = 0;
float R1 = 30000.0; // Resistance of R1 in ohms
float R2 = 7500.0; // Resistance of R2 in ohms

const int numReadings = 100; // Number of power readings to keep track of
float powerReadings[numReadings]; // Array to store power readings
int powerIndex = 0; // Index for the current power reading

Servo myServo; // Create a Servo object

void setup() {
  Serial.begin(9600); // Open serial monitor

  pinMode(voltPin, INPUT); // Voltage sensor pin
  pinMode(currentPin, INPUT); // Current sensor pin
  pinMode(hallPin, INPUT_PULLUP); // RPM measurement
  myServo.attach(servoPin); // Attach the servo to the pin

  attachInterrupt(digitalPinToInterrupt(hallPin), countPulse, FALLING); // Attach interrupt to Hall effect sensor

  // Initialize power readings array
  for (int i = 0; i < numReadings; i++) {
    powerReadings[i] = 0.0;
  }

  myServo.write(servoPos); // Initialize servo position
}

void loop() {
  unsigned long currentTime = millis(); // Get the current time

  int sensorValue = analogRead(voltPin); // Read the voltage from the sensor
  float Vout = sensorValue * (5.0 / 1023.0); // Convert the analog reading to voltage
  
  // Calculate the input voltage using the voltage divider formula
  float Vin = Vout * (R1 + R2) / R2;
  
  if (currentTime - lastTime >= 1000) {
    noInterrupts();
    float pulseFrequency = pulseCount;
    pulseCount = 0;
    interrupts();

    speed = (pulseFrequency / 2.0) * 60.0;
    current = analogRead(currentPin) * 5.0 / 1023.0; // Read current value
    voltage = analogRead(voltPin) * 5.0 / 1023.0; // Read voltage value

    power = current * Vin; // Calculate power (V * I)

    // Store the new power reading in the array
    powerReadings[powerIndex] = power;
    powerIndex = (powerIndex + 1) % numReadings; // Update index, wrap around if necessary

    // Find the maximum power reading
    float maxPower = powerReadings[0];
    for (int i = 1; i < numReadings; i++) {
      if (powerReadings[i] > maxPower) {
        maxPower = powerReadings[i];
      }
    }

    if (power < maxPower && speed > speed_prev) {
      servoPos -= 2; // Decrease servo position by 2 degrees

      if (servoPos <= 0) {
        servoPos = 270; // Reset to start position if it reaches 0
      }
    } else if (power < maxPower && speed < speed_prev) {
      servoPos += 2; // Increase servo position by 2 degrees

      if (servoPos >= 270) {
        servoPos = 0; // Reset to start position if it reaches 270
      }
    }

    myServo.write(servoPos); // Update servo position

    // Print values to the serial monitor
    Serial.print("Speed: ");
    Serial.print(speed);
    Serial.print(" RPM, Current: ");
    Serial.print(current);
    Serial.print(" A, Voltage: ");
    Serial.print(Vin);
    Serial.print(" V, Power: ");
    Serial.print(power);
    Serial.print(" W, Max Power: ");
    Serial.print(maxPower);
    Serial.println(" W");

    lastTime = currentTime;
    if (startTime == 0) {
      startTime = currentTime;
    }
  }

  speed_prev = speed;
}

void countPulse() {
  pulseCount++;
}