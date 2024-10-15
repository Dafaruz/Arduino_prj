// ================================================
// Project: arduino_prj -  system that smooths the shock effect in a car battery. 
// Date: October 8, 2024
// Author: Daniel faruz
//         Natan  sabag
// ================================================

/*
README:

Project Overview:
-----------------
project, which focuses on creating a system that smooths the shock effect in a car battery. 
The system utilizes a supercapacitor, a high-current Hall effect sensor, relays, and a D-latch to regulate and manage the power distribution effectively, 
ensuring a more stable electrical flow during vehicle starts.

*/



#define CURRENT_METER_BAT_SENSOR A4
#define CURRENT_METER_CAP_SENSOR A5
#define VOLTAGE_METER_BAT_SENSOR A2
#define BAT_SWITCH_ON 2
#define BAT_SWITCH_OFF 3 
#define CAP_SWITCH_ON 4
#define CAP_SWITCH_OFF 5


const float vRef = 5.07;  // référence Tension of Arduino (5V in general)
#define MEASUREMENT_ITERATION 100


const float sensitivity_bat = 0.0133333;  // Sensitivity of the sensor in V/A (to adjust per sensor)
float correction_value_bat = 0.146; // correction_value in mV to sdjust the tension for the output sensor 
float ZeroCurrentvalue_bat; // the first current output when the sensor is not connected to the system
float offset_bat;
bool set_zero_current_value_bat = LOW;

const float sensitivity_cap = 0.01;  // Sensitivity of the sensor in V/A (to adjust per sensor)
float correction_value_cap = 0.146; // correction_value in mV to sdjust the tension for the output sensor 
float ZeroCurrentvalue_cap; // the first current output when the sensor is not connected to the system
float offset_cap;
bool set_zero_current_value_cap = LOW;






////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////                  global values for loop function  and creating 1st sector                             ///////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


bool cap_full = LOW;
float R1 = 9.63;
float R2 = 4.47;
float voltage_bat;  
float current_meter_bat;
float current_meter_cap;
int counter=0;
int off_to_on=HIGH;

float vec_current_bat[100];          
float vec_current_cap[100];



/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////                     setup sector and                                                      ////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void setup() 
{


  Serial.begin(9600);
  analogReference(EXTERNAL);

  vec_current_bat[0]=100;
  vec_current_cap[0]=100;
  set_zero_current_value_cap = setZeroCurent_cap();
  set_zero_current_value_bat = setZeroCurent_bat();
  pinMode(VOLTAGE_METER_BAT_SENSOR, INPUT);
  pinMode(CAP_SWITCH_ON, OUTPUT);
  pinMode(BAT_SWITCH_OFF, OUTPUT);

  Serial.print("enter veck check\n");        
  for (int i=0;i<100;i++)
  {
    vec_current_bat[i] = getcurrent_bat();
    vec_current_cap[i] = getcurrent_cap();

  }
  
  Serial.print("out veck check\n");
  Serial.println(max_vec(vec_current_bat));
  Serial.println(max_vec(vec_current_cap));

   digitalWrite(BAT_SWITCH_ON, 0);
   digitalWrite(BAT_SWITCH_OFF,0);
   digitalWrite(CAP_SWITCH_ON, 0);
   digitalWrite(CAP_SWITCH_OFF,0);

  
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////                                                the main code of nathan and daniel                                          /////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void loop() {

    
    current_meter_bat=getcurrent_bat();
    Serial.print("current baterie ");
    Serial.println(current_meter_bat);

    current_meter_cap=getcurrent_cap();
    Serial.print("current capacitor ");
    Serial.println(current_meter_cap);

    voltage_bat = getvoltage_bat();                 // Binary read voltage on the battery
    Serial.print("voltage baterie ");
    Serial.println(voltage_bat);

    Serial.print("\n");
    delay(1000);

    digitalWrite(BAT_SWITCH_ON, 1);    
    delay(200);                              //bat on
    digitalWrite(BAT_SWITCH_ON, 0); 
    delay(200);

    digitalWrite(CAP_SWITCH_ON, 1);    
    delay(200);                              //cap on 
    digitalWrite(CAP_SWITCH_ON, 0);

    /*digitalWrite(BAT_SWITCH_OFF, 1);    
    delay(200);                              //bat off
    digitalWrite(BAT_SWITCH_OFF, 0);  
    delay(1500);
    digitalWrite(CAP_SWITCH_OFF, 1);    
    delay(10);                              //cap off
    digitalWrite(CAP_SWITCH_OFF, 0); 
    delay(1500);
*/


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////                                        the 1st statemant                                 ////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
    if (voltage_bat <= 13.0 && current_meter_bat < 3.0  && off_to_on) {  // Car is off, check the minimum current off to on init to high and will reset to higfh every igniotion
        Serial.println("Car is off");  // indicator 
        digitalWrite(BAT_SWITCH, HIGH);    //!!!!!
        delay(1000);

        while(getcurrent_bat() <= 1)                 // equalibrium current between the cap and bat
            {
                Serial.println("discharge or charge the cap");
                Serial.print(getcurrent_cap());
                Serial.print(" ");                                                          
                digitalWrite(CAP_SWITCH, HIGH);                       // Close the switch to compare voltage      

                if(getcurrent_cap() <= max_vec(vec_current_cap))        // the voltage of the bat and cap is the same 
                {
                  Serial.println("enter to the break");
                  digitalWrite(CAP_SWITCH, LOW);                   
                  break;
                }
            }
            
            
           // vactor check to find the max current when car is off (the leakage current)
          Serial.print("enter veck check\n");        
          for (int i=0;i<100;i++)
          {
              vec_current_bat[i] = getcurrent_bat();
              vec_current_cap[i] = getcurrent_cap();

          }
          Serial.print(current_meter_bat);
          Serial.print("out veck check\n");
          Serial.println(max_vec(vec_current_bat));
          Serial.println(max_vec(vec_current_cap));
          counter++;   
          cap_full = LOW; 
          off_to_on = LOW;                                               // Bool indicates the status of the capacitor
     
    }

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////                                        the 2nd statemant                                 ////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    if (voltage_bat <= 13.0 && current_meter_bat > max_vec(vec_current_bat)*3) {   // Car switch is open, "someone will start the car soon"

        cap_full = LOW;
        digitalWrite(CAP_SWITCH, HIGH); 

        if(counter == 10)
        {
          Serial.println("the counter is 10 only cap will ignite the car");
          digitalWrite(BAT_SWITCH, LOW); 
          counter = 0;        // init the cap for next time 
        }

        // Close the switch to help jump start the car
    }
    
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////                                        the 3rd statemant                                 ////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    if (voltage_bat >= 13.5 && cap_full == LOW) {

        Serial.println("the engain is on and alternator is pumping current ");
        digitalWrite(BAT_SWITCH, HIGH); 
        digitalWrite(CAP_SWITCH, HIGH);

    
        if (current_meter_cap <max_vec(vec_current_cap)) {
            digitalWrite(CAP_SWITCH, LOW);
            cap_full = HIGH;
            Serial.println("the capacitor is full");
        }

        off_to_on = HIGH;   // re init the 1st statment 
   
    }
    */
}



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////                                   the current function and the init function                          ///////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////                                   the vector creating to find the  max value                         ///////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

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


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////                      function to init the zero current of the sensor offset  (bat)                    ///////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



bool setZeroCurent_bat()
{   
    float total_offset = 0;
    for (int i = 0; i < MEASUREMENT_ITERATION; i++) {
      offset_bat = ((float)analogRead(CURRENT_METER_BAT_SENSOR)/1024)*vRef;

      if(offset_bat <= 1 || offset_bat >= 4.9)
      {
          Serial.println("found one_in_offset ");
          i--;
          continue;
      }


      total_offset += offset_bat;
    }
    offset_bat =total_offset/MEASUREMENT_ITERATION;
    ZeroCurrentvalue_bat=getcurrent_bat();
    Serial.println();
    Serial.println("bat sensor init:\n");
    Serial.print("the offset in the bat sensor is :");
    Serial.println(offset_bat,4);
    Serial.print("the zerro current offset is:");
    Serial.println(ZeroCurrentvalue_bat);
    return HIGH;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////                      function to init the zero current of the sensor offset  (cap)                    ///////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool setZeroCurent_cap()
{   
    float total_offset = 0;
    for (int i = 0; i < MEASUREMENT_ITERATION; i++) {
      offset_cap = ((float)analogRead(CURRENT_METER_BAT_SENSOR)/1024)*vRef;

      if(offset_cap <= 1 || offset_cap >= 4.9)
      {
          Serial.println("found one_in_offset ");
          i--;
          continue;
      }

      total_offset += offset_cap;
    }

    offset_cap =total_offset/MEASUREMENT_ITERATION;
    offset_cap = ((float)analogRead(CURRENT_METER_CAP_SENSOR)/1024)*vRef;
    ZeroCurrentvalue_cap=getcurrent_cap();
    Serial.println();
    Serial.println("cap value init:\n");
    Serial.print("the offset in the cap sensor is :");
    Serial.println(offset_cap,4);
    Serial.print("the zerro current offset is:");
    Serial.println(ZeroCurrentvalue_cap);
    return HIGH;
}



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////                                   pull the current value from the sensor (bat)                         ///////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

float getcurrent_bat()
{
       float totalCurrent = 0;
  
    for (int i = 0; i < MEASUREMENT_ITERATION; i++) {
        int rawValue = (float)analogRead(CURRENT_METER_BAT_SENSOR);
        float voltage = (((rawValue / 1024.0) * vRef));
        if(voltage <= 1 || voltage >= 4.9)
        {
          Serial.println("found one ");
          i--;
          continue;
        }

        float current1 = (voltage - offset_bat) / sensitivity_bat;
        totalCurrent += current1;
        delay(1);
         /* Serial.print(voltage);
        Serial.print(" ");*/
    }
    Serial.println();
  
    float averageCurrent = totalCurrent / (MEASUREMENT_ITERATION);
    /*Serial.print("average Vout ");
    Serial.println((averageCurrent * sensitivity_bat) + offset_bat);*/
    if(set_zero_current_value_bat)
    {
        averageCurrent -= ZeroCurrentvalue_bat;
    }
    return averageCurrent;
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////                                   pull the current value from the sensor (cap)                         ///////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

float getcurrent_cap()
{
       float totalCurrent = 0;
  
    for (int i = 0; i < MEASUREMENT_ITERATION; i++) {
        int rawValue = (float)analogRead(CURRENT_METER_CAP_SENSOR);
        float voltage = ((rawValue / 1024.0) * vRef);
         if(voltage <= 1 || voltage >= 4.9)
         {
            Serial.println("found one ");
            i--;
            continue;
        }
        float current1 = (voltage - offset_cap) / sensitivity_cap;
        totalCurrent += current1;
        delay(1);

       /* Serial.print(voltage);
        Serial.print(" ");*/
    
    }
    float averageCurrent = totalCurrent / MEASUREMENT_ITERATION;
    /*Serial.print("average Vout ");
    Serial.println((averageCurrent * sensitivity_cap) + offset_cap);*/
    if(set_zero_current_value_cap)
    {
        averageCurrent -= ZeroCurrentvalue_cap;
    }
    return averageCurrent;
}





float getvoltage_bat(){
  float total_voltage = 0;

  for(int i = 0; i < MEASUREMENT_ITERATION; i++){

        int rawValue = (float)analogRead(VOLTAGE_METER_BAT_SENSOR);
        float voltage = (((rawValue / 1024.0) * vRef) * ((R1 + R2) / R2));
        total_voltage += voltage;
        delayMicroseconds(100); 
  }

  return (total_voltage/MEASUREMENT_ITERATION) ;


}


  
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////                              end of the code for now                                            /////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////