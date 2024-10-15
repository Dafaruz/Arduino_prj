#define boutonPin A0 // Define the pin connected to the button or sensor

void setup() {
  // Initialize the pin mode for the button pin
  pinMode(boutonPin, INPUT);
  // Initialize serial communication at 9600 bits per second
  Serial.begin(9600);
}

// Variable to store the current state of the button
int etatBouton;
int R1=9.63;
int R2=4.47;

void loop() {
  // Read the analog input from boutonPin (A0)
  etatBouton = analogRead(boutonPin)-28;
  Serial.println(etatBouton);

  // Convert the analog reading (0 - 1023) to a voltage (0 - 5V)
  float voltage = (etatBouton / 1023.0)* 4.9 * (R1 + R2)/R2;

  // Print the voltage value in a format suitable for the Serial Plotter
  //Serial.print("Time: ");
 // Serial.print(millis() / 1000.0); // Print time in seconds
 delay(500);
  Serial.print(", Voltage: ");
  Serial.println(voltage,2); // Print voltage with 2 decimal places

  //Delay to slow down the serial output for better readability
  //delay(1000); // Wait for 1 second before the next reading
}
