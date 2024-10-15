volatile unsigned long lastPulseTime = 0; // To store the time of the last pulse
volatile unsigned long currentPulseTime = 0; // To store the time of the current pulse
volatile bool pulseDetected = false; // To indicate when 2 pulses are detected

int sensorPin = 2; // Hall effect sensor connected to digital pin 2
float rpm = 0; // To store the calculated RPM
float lastValidRPM = 0; // To store the last valid RPM for filtering

// Threshold values for filtering noise
const float minRPM = 0.1;  // Minimum plausible RPM (ignore anything less)
const float maxRPM = 5000; // Maximum plausible RPM (ignore unrealistic values)

// Interrupt Service Routine for the Hall effect sensor
void pulseCounter() {
  if (!pulseDetected) {
    // First pulse
    lastPulseTime = micros(); // Store the time of the first pulse
    pulseDetected = true; // Mark that the first pulse has been detected
  } else {
    // Second pulse
    currentPulseTime = micros(); // Store the time of the second pulse
    pulseDetected = false; // Reset for the next pulse detection
    calculateRPM(); // Calculate the RPM after the second pulse
  }
}

// Function to calculate RPM based on the time difference between 2 pulses
void calculateRPM() {
  unsigned long timeDifference = currentPulseTime - lastPulseTime; // Time between 2 pulses in microseconds
  float timePerRevolution = timeDifference / 1000000.0; // Convert time to seconds (from microseconds)
  
  // Calculate the rotational speed in RPM (Rounds Per Minute)
  rpm = (1.0 / timePerRevolution) * 60.0;

  // Apply filtering to remove noise
  if (rpm >= minRPM && rpm <= maxRPM) {
    lastValidRPM = rpm; // Update the last valid RPM if the current RPM is plausible
    Serial.print("RPM: ");
    Serial.println(rpm);
  } else {
    // Ignore the noisy value and keep the last valid RPM
    Serial.print("Filtered noisy reading. Last valid RPM: ");
    Serial.println(lastValidRPM);
  }
}

void setup() {
  Serial.begin(9600); // Start the serial communication
  attachInterrupt(digitalPinToInterrupt(sensorPin), pulseCounter, RISING); // Attach the interrupt to the Hall effect sensor pin
}

void loop() {
  // The loop does nothing; all the work is handled in the interrupt
}
