
#include <Robojax_WCS.h>
#include <Wire.h> 

#define CURRENT_METER_BAT_SENSOR A0
#define CURRENT_METER_CAP_SENSOR A1
#define VOLTAGE_METER_BAT_SENSOR A3
#define BAT_SWITCH 5
#define CAP_SWITCH 6

#define MODEL 9 //see list above
#define SENSOR_VCC_PIN 8 //pin for powring up the sensor
#define ZERO_CURRENT_LED_PIN_BAT 2 //zero current LED pin for the battery
#define ZERO_CURRENT_LED_PIN_CAP 3 //zero current LED pin for the capacitor

#define ZERO_CURRENT_WAIT_TIME 5000 //wait for 5 seconds to allow zero current measurement
#define CORRECTION_VALUE 164 //mA
#define MEASUREMENT_ITERATION 100
#define VOLTAGE_REFERENCE  5000.0 //5000mv is for 5V
#define BIT_RESOLUTION 10
#define DEBUT_ONCE true

Robojax_WCS sensor_bat(
          MODEL, CURRENT_METER_BAT_SENSOR, SENSOR_VCC_PIN, 
          ZERO_CURRENT_WAIT_TIME, ZERO_CURRENT_LED_PIN_BAT,
          CORRECTION_VALUE, MEASUREMENT_ITERATION, VOLTAGE_REFERENCE,
          BIT_RESOLUTION, DEBUT_ONCE           
          );

Robojax_WCS sensor_cap(
          MODEL, CURRENT_METER_CAP_SENSOR, SENSOR_VCC_PIN, 
          ZERO_CURRENT_WAIT_TIME, ZERO_CURRENT_LED_PIN_CAP,
          CORRECTION_VALUE, MEASUREMENT_ITERATION, VOLTAGE_REFERENCE,
          BIT_RESOLUTION, DEBUT_ONCE           
          );

void setup() {
 
  sensor_bat.start();
  sensor_cap.start();
  pinMode(VOLTAGE_METER_BAT_SENSOR, INPUT);
  pinMode(CAP_SWITCH, OUTPUT);
  pinMode(BAT_SWITCH, OUTPUT);
  Serial.begin(9600);
  /*
  Serial.print("Sensor: "); Serial.println(sensor.getModel());
  
  // Afficher la valeur du courant zéro après l'initialisation
  Serial.print("Zero Current: ");
  Serial.print(sensor.getZeroCurrent(), 3);  // Affiche avec 3 décimales
  Serial.println(" A");*/
}

bool cap_full = LOW;
float R1 = 9.5;
float R2 = 5.0;
bool  vec_check = LOW;
float voltage_bat;
int voltage_bat_bin;     // 
//float Rm = 100.0;        // Potentiometer that will give us the gain of the current meter
//int current_meter_bat_bin;
float current_meter_bat;
//int current_meter_cap_bin;
float current_meter_cap;
float vec_current[100];
int counter=0;


void loop() {
  sensor_bat.readCurrent();
  sensor_cap.readCurrent();
/*
  current_meter_bat_bin = analogRead(CURRENT_METER_BAT_SENSOR);    // Binary read current from the battery
  current_meter_bat = ((((float)current_meter_bat_bin / 1023.0) * 5.0) - 2.5) / Rm;  // Formula to get the current as float
*/
    current_meter_bat=sensor_bat.getCurrent();
    current_meter_cap=sensor_cap.getCurrent();

    voltage_bat_bin = analogRead(VOLTAGE_METER_BAT_SENSOR);                    // Binary read voltage on the battery
    voltage_bat = ((float)voltage_bat_bin / 1023.0) * 5.0 * ((R1 + R2) / R2);  // Convert binary to float representing battery voltage

    if (voltage_bat <= 13.0 && current_meter_bat < 3.0) {                     // Car is off, check the minimum current
        digitalWrite(BAT_SWITCH, HIGH);
        while(sensor_bat.getCurrent() <= 1)
            {                                                         
                digitalWrite(CAP_SWITCH, HIGH); 
                if(sensor_cap.getCurrent() <= 0.1)
                {
                    break;
                }                                                                         // Wait to discharge for 5 sec
            }
            digitalWrite(CAP_SWITCH, LOW);
            
            if(vec_check == LOW){
  
                    for (int i=0;i<100;i++)
                    {
                        sensor_bat.readCurrent();
                        vec_current[i] = sensor_bat.getCurrent();
                        delay(500);
                    }
                    counter++;
                vec_check = HIGH;
                } 
            cap_full = LOW;                                                // Bool indicates the status of the capacitor
     
    }

    if (voltage_bat <= 13.0 && current_meter_bat > max_vec(vec_current)) {   // Car switch is open, "someone will start the car soon"
        cap_full = LOW;
        digitalWrite(CAP_SWITCH, HIGH); 
        if(counter == 10)
        {
          digitalWrite(BAT_SWITCH, LOW); 
          counter = 0;
        }
        // Close the switch to help jump start the car
    }

    if (voltage_bat >= 13.8 && cap_full == LOW) {
        digitalWrite(BAT_SWITCH, HIGH); 
        digitalWrite(CAP_SWITCH, HIGH);
        vec_check = LOW;
    
        if (current_meter_cap < 0.1) {
            digitalWrite(CAP_SWITCH, LOW);
            cap_full = HIGH;
        }
    /*
     current_meter_cap_bin = analogRead(CURRENT_METER_CAP_SENSOR);
    current_meter_cap = (((float)current_meter_cap_bin / 1023.0) * 5.0 - 2.5) / Rm;
    */
    }
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
