#include <Servo.h>  // Include the Servo library

const int currentPin = A7; // Pin for current sensor
const int voltPin = A6; // Pin for voltage sensor
const int hallPin = 2; // Pin for Hall effect sensor
const int servoPin = 3; // Pin for servo motor

volatile unsigned long pulseStartTime = 0; // Time of the first pulse
volatile unsigned long pulseEndTime = 0;   // Time of the second pulse
volatile unsigned long lastPulseTime = 0;  // Time of the last pulse for reset
volatile bool secondPulseDetected = false; // Flag to indicate second pulse detected

float speed = 0.0;
float speed_prev = 0.0;
float current = 0.0;
float voltage = 0.0;
float power = 0.0;
int servoPosition = 180; // Initial servo position
unsigned long lastTime = 0;

float R1 = 29850.0; // Resistance of R1 in ohms
float R2 = 7440.0; // Resistance of R2 in ohms
float Vref = 4.70;
float sensitivity = 0.066;
const int numReadings = 100; // Number of power readings to keep track of
float powerReadings[numReadings]; // Array to store power readings
int powerIndex = 0; // Index for the current power reading
int MEASUREMENT_ITERATION = 1000;

Servo myServo; // Create a Servo object
float offset = 0;
bool set_zero_current = LOW;
float ZeroCurrentvalu = 0;

const unsigned long rpmTimeout = 5000; // Timeout in ms for RPM reset if no pulses are detected
const unsigned long debounceTime = 500; // Minimum time in microseconds between pulses to avoid noise
const float maxRPM = 1000.0; // Maximum realistic RPM threshold






///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//          setup function to init the arduino                                                                   /////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



void setup() {

  delay(10000);

  Serial.begin(9600); // Open serial monitor

//////////////////////////////////////////////////////////////////////
//////////          pin mode sector                 /////////////////
//////////////////////////////////////////////////////////////////////


  pinMode(voltPin, INPUT); // Voltage sensor pin
  pinMode(currentPin, INPUT); // Current sensor pin
  pinMode(hallPin, INPUT_PULLUP); // RPM measurement
  myServo.attach(servoPin); // Attach the servo to the pin


/////////////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////////////////
//            initializee vectors and zeroo current  and   interrupt to Hall effect     /////////
/////////////////////////////////////////////////////////////////////////////////////////////////

  attachInterrupt(digitalPinToInterrupt(hallPin), countPulse, FALLING); // Attach interrupt to Hall effect sensor

  // Initialize power readings array
  for (int i = 0; i < numReadings; i++) {
    powerReadings[i] = 0.0;
  }

  myServo.write(servoPosition); // Initialize servo position


  // Initialize zero current and offset 
  offset = setZeroCurent();
  ZeroCurrentvalu = getcurrent();
  set_zero_current = HIGH;

}

////////////////////////////////////////////////////////////////////////////
/////////////////////     end of setup function        /////////////////////
////////////////////////////////////////////////////////////////////////////



///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                       the main loop function                                                                   /////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void loop() {
  unsigned long currentTime = millis(); // Get the current time

  current = getcurrent(); // Read current value
  voltage = voltage_meter(); // Read voltage
  power = current * voltage; // Calculate power (V * I)


  // Store the new power reading in the array
  powerReadings[powerIndex] = power;
  powerIndex = (powerIndex + 1) % numReadings; // Update index, wrap around if necessary
  float maxPower = maximum_finder();        // Find the maximum power reading
  speed = RPM_calc(currentTime);


  // Calculate and update the servo position based on power and speed
  servoPosition = calculateServoPosition(servoPosition,maxPower, power, speed, speed_prev);
  myServo.write(servoPosition); // Update servo position

  
  // Print values to the serial monitor
  print(speed,current,voltage,power,maxPower);
  lastTime = currentTime;
  speed_prev = speed;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//        RPM calc function                                                                                       /////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


float RPM_calc(unsigned long currentTime){

  if (currentTime - lastPulseTime > rpmTimeout) {    // Reset speed to 0 if no pulse detected within timeout
    speed = 0.0; 
    return speed;                                     // Reset RPM if fan is not running
  }

    // Check if two pulses were detected
  if (secondPulseDetected) {
    // Calculate RPM based on the time difference between two pulses
    unsigned long pulseDuration = pulseEndTime - pulseStartTime;
    if (pulseDuration > 0) {
      float calculatedRPM = (1000000.0 / pulseDuration) * 60.0; // Convert to RPM

      // Only update speed if within realistic range
      if (calculatedRPM <= maxRPM && abs(calculatedRPM - speed) <= 100) {
        speed = calculatedRPM;
      } else {
        Serial.println("Noise detected - RPM ignored");
      }
    }
    // Update pulseStartTime for next measurement and reset the flag
    pulseStartTime = pulseEndTime; // The last pulse becomes the new "first pulse"
    secondPulseDetected = false;
    lastPulseTime = currentTime; // Update the last pulse time for the timeout check
  }

  return speed;


}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//          print function                                                                                        /////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void print(float speed, float current, float voltage ,float power, float maxPower ){

  // Print values to the serial monitor
  Serial.print("Speed: ");
  Serial.print(speed);
  Serial.print(" RPM, Current: ");
  Serial.print(current, 3);
  Serial.print(" A, Voltage: ");
  Serial.print(voltage);
  Serial.print(" V, Power: ");
  Serial.print(power);
  Serial.print(" W, Max Power: ");
  Serial.println(maxPower);

}



///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//           Maximum Finder for Power Calculation                                                                 /////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

float maximum_finder() {
  float maxPower = powerReadings[0];
  for (int i = 1; i < numReadings; i++) {
    if (powerReadings[i] > maxPower) {
      maxPower = powerReadings[i];
    }
  }
  return maxPower;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//           Servo Motor Position Calculation Function                                                            /////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


int calculateServoPosition(int currentServoPosition, float maxPower, float power, float speed, float speed_prev) {
   Serial.println(currentServoPosition);
  if (power < maxPower && speed > speed_prev) {
    Serial.println("Power decreased and speed increased, moving servo -2 degrees");
    currentServoPosition -= 2; // Decrease position by 2 degrees
    if (currentServoPosition <= 0) {
      currentServoPosition = 270; // Reset to start position if it reaches 0
    }
  } else if (power < maxPower && speed < speed_prev) {
    Serial.println("Power decreased and speed decreased, moving servo +2 degrees");
    currentServoPosition += 2; // Increase position by 2 degrees
    if (currentServoPosition >= 270) {
      currentServoPosition = 0; // Reset to start position if it reaches 270
    }
  }
  return currentServoPosition;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//           Pulse Counter to Detect the Time Difference Between 2 Pulses                                         /////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



void countPulse() {
  unsigned long currentMicros = micros();

  // Ignore pulses that are too close together (debounce)
  if (currentMicros - pulseEndTime < debounceTime) {
    return; // Ignore pulse if within debounce time
  }

  if (!secondPulseDetected) {
    pulseEndTime = currentMicros; // Capture the time of the current pulse
    secondPulseDetected = true;
  } else {
    pulseStartTime = pulseEndTime; // Set the last pulse as the first
    pulseEndTime = currentMicros; // Capture the time of the new pulse
    secondPulseDetected = true;
  }
  lastPulseTime = millis(); // Update the time of the last detected pulse
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//           Zero Current Function with Noise Reduction                                                           /////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



float setZeroCurent() {
  float offset_l;
  float ZeroCurrentvalue;
  float total_offset = 0;
  for (int i = 0; i < MEASUREMENT_ITERATION; i++) {
    offset_l = ((float)analogRead(currentPin) / 1024) * Vref;
    if (offset_l <= 1 || offset_l >= 4.9) {
      i--;
      continue;
    }
    total_offset += offset_l;
  }
  total_offset = total_offset / MEASUREMENT_ITERATION;
  ZeroCurrentvalue = (((float)analogRead(currentPin) / 1024) * Vref - total_offset) / sensitivity;

  Serial.println();
  Serial.println("Initial values:");
  Serial.print("The offset in sensor is: ");
  Serial.println(total_offset, 4);
  Serial.print("The zero current offset is: ");
  Serial.println(ZeroCurrentvalue);
  return total_offset;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//           Current Function with Noise Reduction                                                                /////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
float getcurrent() {
  float totalCurrent = 0;
  for (int i = 0; i < MEASUREMENT_ITERATION; i++) {
    float voltage = ((analogRead(currentPin) / 1024.0) * Vref);
    if (voltage <= 1 || voltage >= 4.9) {
      i--;
      continue;
    }
    totalCurrent += (voltage - offset) / sensitivity;
    delay(1);
  }
  float averageCurrent = totalCurrent / MEASUREMENT_ITERATION;
  if (set_zero_current) {
    averageCurrent -= ZeroCurrentvalu;
  }
  return averageCurrent;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//           Voltage Reading Function                                                                              /////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
float voltage_meter() {
  int sensorValue = analogRead(voltPin);
  float Vout = sensorValue * (Vref / 1024.0);
  float Vin = Vout * (R1 + R2) / R2;
  return Vin;
}
