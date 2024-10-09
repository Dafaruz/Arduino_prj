
#define CURRENT_METER_BAT_SENSOR A5
#define CURRENT_METER_CAP_SENSOR A0

float vRef = 5.0;  // référence Tension of Arduino (5V in general)
#define MEASUREMENT_ITERATION 1000

const float sensitivity_bat = 0.013333;  // Sensitivity of the sensor in V/A (to adjust per sensor)
//float correction_value_bat = 0.146; // correction_value in mV to sdjust the tension for the output sensor 
float ZeroCurrentvalue_bat; // the first current output when the sensor is not connected to the system
float offset_bat;
bool set_zero_current_value_bat = LOW;

const float sensitivity_cap = 0.01;  // Sensitivity of the sensor in V/A (to adjust per sensor)
float correction_value_cap = 0.146; // correction_value in mV to sdjust the tension for the output sensor 
float ZeroCurrentvalue_cap; // the first current output when the sensor is not connected to the system
float offset_cap;
bool set_zero_current_value_cap = LOW;


float current_meter_bat;
float current_meter_cap;


void setup() 
{

  Serial.begin(9600);
  analogReference(EXTERNAL);
   Serial.println("hello");
 
  set_zero_current_value_cap = setZeroCurent_cap();
  set_zero_current_value_bat = setZeroCurent_bat();
}


void loop(){
  
   current_meter_bat=getcurrent_bat();
    Serial.print("current baterie ");
    Serial.println(current_meter_bat);


   current_meter_cap=getcurrent_cap();
    Serial.print("current capacitor ");
    Serial.println(current_meter_cap);

    Serial.print("\n");
    delay(1000);
}


bool setZeroCurent_bat()
{   
    float total_offset = 0;
    for (int i = 0; i < MEASUREMENT_ITERATION; i++) {
        offset_bat = ((float)analogRead(CURRENT_METER_BAT_SENSOR)/1024)*vRef;
        if(offset_bat <= 1.0 || offset_bat >= 4.9)
        {
          Serial.println("found one offset_bat ");
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
    Serial.print("the zero current offset is:");
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
        if(offset_cap <= 1.0 || offset_cap >= 4.9)
        {
            Serial.println("found one offset_cap ");
              i--;
              continue;
        }
        total_offset += offset_cap;
    }
    

    offset_cap =total_offset/MEASUREMENT_ITERATION;
    ZeroCurrentvalue_cap=getcurrent_cap();
    Serial.println();
    Serial.println("cap value init:\n");
    Serial.print("the offset in the cap sensor is :");
    Serial.println(offset_cap,4);
    Serial.print("the zero current offset is:");
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
        /* Serial.print(voltage,3);
        Serial.print(" ");*/
    }
    Serial.println();
  
    float averageCurrent = totalCurrent / (MEASUREMENT_ITERATION);
    Serial.print("average Vout ");
    Serial.println((averageCurrent * sensitivity_bat) + offset_bat);
    if(set_zero_current_value_bat)
    {
        averageCurrent -= ZeroCurrentvalue_bat;
    }
    return averageCurrent;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////                                   pull the curent value from the sensor (cap)                         ///////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

float getcurrent_cap()
{
       float totalCurrent = 0;
  
    for (int i = 0; i < MEASUREMENT_ITERATION; i++) {
        int rawValue = (float)analogRead(CURRENT_METER_CAP_SENSOR);
        float voltage = ((rawValue / 1024.0) * vRef);
         if(voltage <= 1.0 || voltage >= 4.9)
         {
            Serial.println("found one ");
            i--;
            continue;
         }
        float current1 = (voltage - offset_cap) / sensitivity_cap;
        totalCurrent += current1;
        delay(1);

       /* Serial.print(voltage,3);
        Serial.print(" ");*/
    
    }
    float averageCurrent = totalCurrent / MEASUREMENT_ITERATION;
    Serial.print("average Vout ");
    Serial.println((averageCurrent * sensitivity_cap) + offset_cap);
    if(set_zero_current_value_cap)
    {
        averageCurrent -= ZeroCurrentvalue_cap;
    }
    return averageCurrent;
}