/*
 * Robojax_WCS_OverCurrent
 * Read current and control a relay to disconnect load at certain current value
 (there is another version of this with LCD. Check library)
 * This is Arduino code based on Robojax WCS library 
 * for Winson WSC Current Sensor to measure current
 get the Robojax WCS Arduino library: http://robojax.com/L/?id=230
 * 
 * Watch video instruction for this code: https://youtu.be/vs7Nw107AOo


 * Related Vidsos:
 * Introduction to Winson WCS Sensors: https://youtu.be/z-s8UvCWGxY
 * Display current on LCD: https://youtu.be/-pg7jbkaB6A
 * Overcurrent protection with WCS Current sensors: https://youtu.be/vs7Nw107AOo
 * Measure Current using ESP8266 NodeMCU, D1 Mino over Wifi:
 * Measure current using ESP32 Bluetooth on mobile devices:
 
 
 The LCD library is taken from: https://github.com/fdebrabander/Arduino-LiquidCrystal-I2C-library
 
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
#define MODEL 12
#define SENSOR_PIN A0 //pin for reading sensor
#define SENSOR_VCC_PIN 8 //pin for powring up the sensor
#define ZERO_CURRENT_LED_PIN 2 //zero current LED pin

#define ZERO_CURRENT_WAIT_TIME 5000 //wait for 5 seconds to allow zero current measurement
#define CORRECTION_VLALUE 164 //mA
#define MEASUREMENT_ITERATION 100
#define VOLTAGE_REFERENCE  5000.0 //5000mv is for 5V
#define BIT_RESOLUTION 10
#define DEBUG_ONCE true

//Settings for relay control
const int RELAY_PIN = 3;
const float CURRENT = 12.5;
const int relayTrigerType = HIGH;
int relayON, relayOFF;
const int HOLD_FOR = 6000;
bool STOP=false;
//////////// end of relay control settings


Robojax_WCS sensor(
          MODEL, SENSOR_PIN, SENSOR_VCC_PIN, 
          ZERO_CURRENT_WAIT_TIME, ZERO_CURRENT_LED_PIN,
          CORRECTION_VLALUE, MEASUREMENT_ITERATION, VOLTAGE_REFERENCE,
          BIT_RESOLUTION, DEBUG_ONCE           
          );

void setup()
{
  //Robojax.com WCS Current Sensor Relay Control 20200726
  Serial.begin(9600);
  Serial.println("Robojax WCS Library Relay Control");
  sensor.start();
  Serial.print("Sensor: "); Serial.println(sensor.getModel());
  Serial.print("Library Version:");Serial.println(sensor.version());
  //sensor.printModels();//prints all supported WCS models
  
    pinMode(RELAY_PIN, OUTPUT);
  if(relayTrigerType ==HIGH)
  {
    digitalWrite(RELAY_PIN,LOW); //define pin for relay 
    relayON =HIGH;//set relayON for HIGH trigger
    relayOFF=LOW; //set realyOFF for HIGH trigger 
  }else{
    digitalWrite(RELAY_PIN,HIGH);  
    relayON =LOW;//set relayON for LOW trigger
    relayOFF=HIGH;//set relayOFF for LOW trigger
  }
  //Robojax.com WCS Current Sensor Relay Control 20200726
}//setup() end

void loop()
{ 
    //Robojax.com WCS Current Sensor Relay Control 20200726
  sensor.printCurrent();
  relayControl();

  delay(1000);//wait for 1 second
  
   if(sensor.getCurrent() > 8.5 )
  {
     //do something when current is greater than 8.5A
  }  
	//sensor.printDebug();
  //Robojax.com WCS Current Sensor Relay Control 20200726  
}//loop end


/*
 * relayControl()
 * @brief turns relay ON or OFF when CURRENT is equal sensor.getCurrent()
 * @param none
 * @return none
 * Written by Ahmad Shamshiri for robojax.com
 * on July 26, 2020 in Ajax, Ontario, Canada
 */

void relayControl()
{
    //Robojax.com WCS Current Sensor Relay Control 20200726
  if(sensor.getCurrent() >= CURRENT )
  {
    digitalWrite(RELAY_PIN, relayON);
    Serial.print("Current reached: ");
    Serial.print(CURRENT);Serial.println("A");
    
    Serial.println("Relay turned ON");
    if(STOP)
    {
      Serial.println("Waiting for ever");
      while(1);//wait for ever
    }else{
    Serial.print("Waiting for ");
    Serial.print(HOLD_FOR/1000);Serial.println(" Seconds");
      delay(HOLD_FOR);//wait for this much time
    }
  }else{
    digitalWrite(RELAY_PIN, relayOFF);   
  }  
    //Robojax.com WCS Current Sensor Relay Control 20200726
}//relayControl() end
