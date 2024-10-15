const int sensorPin = A0;  // analog pin for the signal sensor
const int ITERATION = 100;  // Nunmber of iteration for precision
const float sensitivity = 0.0133333;  // Sensitivity of the sensor in V/A (to adjust per sensor)
const float vRef = 4.86;  // référence Tension of Arduino (5V in general)
float correction_value = -0.135; // correction_value in mV to sdjust the tension for the output sensor 
//float correction_value = 0;
float ZeroCurrentvalue; // the first current output when the sensor is not connected to the system
bool set_zero_cureent_value = LOW;
float offset;

void setup() {
  Serial.begin(9600);
  analogReference(DEFAULT);
  set_zero_cureent_value = setZeroCurent();
}

void loop() {
  
  Serial.print("Courant : ");
  Serial.print(getcurrent(), 3);  // Afficher avec 3 décimales
  Serial.println(" A");
  delay(500);
}


float getcurrent()
{
    float totalCurrent = 0;
  
    for (int i = 0; i < ITERATION; i++) {
        int rawValue = analogRead(sensorPin);
        float voltage = (((rawValue / 1024.0) * vRef) - 2.495) +  correction_value;
        float current1 = voltage / sensitivity;
        totalCurrent += current1;
        delayMicroseconds(100);
  
    //Serial.print(current1,3);
    //Serial.print(voltage,4);
    //Serial.print("\n");
    //delay(1000); 
    
    }
  
    float averageCurrent = totalCurrent / ITERATION;
    if(set_zero_cureent_value)
    {
        averageCurrent -= ZeroCurrentvalue;
    }
    return averageCurrent;
}


bool setZeroCurent()
{   
    float total_offset = 0;
    for (int i = 0; i < ITERATION; i++) {
      offset = ((float)analogRead(sensorPin)/1024)*vRef;
      total_offset += offset;
    }

    offset =total_offset/ITERATION;
    offset = ((float)analogRead(sensorPin)/1024)*vRef;
    ZeroCurrentvalue=getcurrent();
    Serial.println();
    Serial.println("value init:\n");
    Serial.print("the offset in sensor is :");
    Serial.println(offset,4);
    Serial.print("the zero current offset is:");
    Serial.println(ZeroCurrentvalue);
    return HIGH;
}
