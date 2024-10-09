/*
 * Read the current Winson WCS Current sensor and display it on Serial monitor
 * This is Arduino code based on Robojax WCS library 
 * for Winson WSC Current Sensor to measure current
 
 get the Robojax WCS Arduino library: http://robojax.com/L/?id=230
 
 * Watch video instructions for this code: https://youtu.be/z-s8UvCWGxY

 * 
 * Related Vidsos:
 * Introduction to Winson WCS Sensors: https://youtu.be/z-s8UvCWGxY
 * Display current on LCD: https://youtu.be/-pg7jbkaB6A
 * Overcurrent protection with WCS Current sensors: https://youtu.be/vs7Nw107AOo
 * Measure Current using ESP8266 NodeMCU, D1 Mino over Wifi:
 * Measure current using ESP32 Bluetooth on mobile devices:

 * Written on July 26, 2020 by Ahmad Shamshiri in Ajax, Ontario, Canada
 * www. Robojax.com
  Model of the sensor to select 
     //dirct wiring series
    0  "WCS38A25",//0
    1  "WCS37A50",//1
    2  "WCS2801",//2
    3  "WCS2702",//3
    4  "WCS2705",//4
    5  "WCS2810",//5
    6  "WCS2720",//6
    7  "WCS2750",//7
    8  "WCS3740",//8
      
      //through hole sensor
    9  "WCS1500",//9
    10  "WCS1600",//10
    11  "WCS1700",//11
    12  "WCS1800",//12
    13  "WCS2800",//13
    14  "WCS6800",//14
      
      //AC to DC Current sensor
    15  "WCS2202",//15


 * Get this code and other Arduino codes from Robojax.com
Learn Arduino step by step in structured course with all material, wiring diagram and library
all in once place. Purchase My course on Udemy.com http://robojax.com/L/?id=62

If you found this tutorial helpful, please support me so I can continue creating 
content like this. You can support me on Patreon http://robojax.com/L/?id=63

or make donation using PayPal http://robojax.com/L/?id=64

 *  * This code is "AS IS" without warranty or liability. Free to be used as long as you keep this note intact.* 
 * This code has been download from Robojax.com
    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.

*/
#include <Wire.h> 
#include <Robojax_WCS.h>
#define MODEL 12 //see list above
#define SENSOR_PIN A0 //pin for reading sensor
#define SENSOR_VCC_PIN 8 //pin for powring up the sensor
#define ZERO_CURRENT_LED_PIN 2 //zero current LED pin

#define ZERO_CURRENT_WAIT_TIME 5000 //wait for 5 seconds to allow zero current measurement
#define CORRECTION_VLALUE 164 //mA
#define MEASUREMENT_ITERATION 100
#define VOLTAGE_REFERENCE  5000.0 //5000mv is for 5V
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
  Serial.print("Library Version:");Serial.println(sensor.version());
  //sensor.printModels();//prints all supported WCS models

}

void loop()
{ 
  sensor.readCurrent();//this must be inside loop
  sensor.printCurrent();

  //does somethig when current is equal or greator than 12.3A
  if(sensor.getCurrent() >= 12.3)
  {
    // does somethig here
  }
  delay(500);
	//sensor.printDebug();
}