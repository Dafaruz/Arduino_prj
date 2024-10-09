#include <Robojax_WCS.h>
#include <Wire.h> 

#define MODEL 9 //see list above
#define SENSOR_PIN A0//pin for reading sensor
#define SENSOR_VCC_PIN 8 //pin for powring up the sensor
#define ZERO_CURRENT_LED_PIN 2 //zero current LED pin

#define ZERO_CURRENT_WAIT_TIME 500 //wait for 5 seconds to allow zero current measurement
#define CORRECTION_VLALUE 150 //mA
#define MEASUREMENT_ITERATION 100
#define VOLTAGE_REFERENCE 4900.0 //5000mv is for 5V
#define BIT_RESOLUTION 10
#define DEBUT_ONCE true


Robojax_WCS sensor(
          MODEL, SENSOR_PIN, SENSOR_VCC_PIN, 
          ZERO_CURRENT_WAIT_TIME, ZERO_CURRENT_LED_PIN,
          CORRECTION_VLALUE, MEASUREMENT_ITERATION, VOLTAGE_REFERENCE,
          BIT_RESOLUTION, DEBUT_ONCE           
          );

void setup()
{
  Serial.begin(9600);
  Serial.println("Robojax WCS Library");
  sensor.start();
  Serial.print("Sensor: "); Serial.println(sensor.getModel());
  
  // Afficher la valeur du courant zéro après l'initialisation
  Serial.print("Zero Current: ");
  Serial.print(sensor.getZeroCurrent(), 3);  // Affiche avec 3 décimales
  Serial.println(" A");
}

float vector[100];


void loop()
{ 
  sensor.readCurrent();
  
  // Afficher le courant actuel et le courant zéro
  Serial.print("Current: ");
  Serial.print(sensor.getCurrent(), 3);
  Serial.println("");
  
  /*
  // Fait quelque chose quand le courant est égal ou supérieur à 12.3A
  for (int i=0;i<100;i++)
  {
    vector[i] = sensor.getCurrent();
    delay(500);
  }
  
    
  if(sensor.getCurrent() >= max_vec(vector))
  {
    // Fait quelque chose ici
  }
  */
  
  //sensor.printDebug();
}


float max_vec(float vector[])
{
  float temp;
  temp=vector[0];
  for (int i=1;i<100;i++)
  {
    if (vector[i] > temp)
    {
      temp = vector[i];
    }
  }
  return temp;
}
