



#define BAT_SWITCH_ON  2
#define BAT_SWITCH_OFF 3
#define CAP_SWITCH_ON  4
#define CAP_SWITCH_OFF 5
#define BAT_SWITCH_OUT 6
#define CAP_SWITCH_OUT 7


// In this project, we encountered noise issues caused by one Arduino. 
// The problem occurred when the Arduino performed an analog read and a digital write simultaneously. 
// This likely resulted in electrical noise, possibly caused by signal interference or feedback along the lines, leading to inaccurate readings.


void setup() {
  Serial.begin(9600);
  pinMode(BAT_SWITCH_ON,INPUT);
  pinMode(BAT_SWITCH_OFF,INPUT);
  pinMode(CAP_SWITCH_ON,INPUT);
  pinMode(CAP_SWITCH_OFF,INPUT);
  pinMode(BAT_SWITCH_OUT,OUTPUT);
  pinMode(CAP_SWITCH_OUT,OUTPUT);

  

}


bool check =LOW;
bool check_L =LOW;
bool check_C = LOW;
bool check_LC = LOW;

void SWITCHH_ON(int SWITCH_ON,int SWITCH_OUT)
{
    if(digitalRead(SWITCH_ON) == HIGH && check == LOW)
  {
    check =HIGH;
  }
   if(digitalRead(SWITCH_ON) == LOW && check == HIGH)
  {
    check = LOW;
    digitalWrite(SWITCH_OUT,HIGH);
    //delay(300);

  }

}

void SWITCHH_OFF(int SWITCH_OFF,int SWITCH_OUT)
{
    if(digitalRead(SWITCH_OFF) == HIGH && check == LOW)
  {
    check_L =HIGH;
  }
   if(digitalRead(SWITCH_OFF) == LOW && check == HIGH)
  {
    check_L = LOW;
    digitalWrite(SWITCH_OUT,LOW);
    //delay(300);

  }

}


void loop() {

  /*SWITCHH_ON(BAT_SWITCH_ON,BAT_SWITCH_OUT);
  SWITCHH_OFF(BAT_SWITCH_OFF,BAT_SWITCH_OUT);

  SWITCHH_ON(CAP_SWITCH_ON,CAP_SWITCH_OUT);
  SWITCHH_OFF(CAP_SWITCH_OFF,CAP_SWITCH_OUT);*/

   if(digitalRead(BAT_SWITCH_ON) == HIGH && check == LOW)
  {
    check =HIGH;
  }
   if(digitalRead(BAT_SWITCH_ON) == LOW && check == HIGH)
  {
    check = LOW;
    digitalWrite(BAT_SWITCH_OUT,HIGH);
    //delay(300);

  }
  
  if(digitalRead(BAT_SWITCH_OFF) == HIGH && check_L == LOW)
  {
    check_L =HIGH;
  }
   if(digitalRead(BAT_SWITCH_OFF) == LOW && check_L == HIGH)
  {
    check_L = LOW;
    digitalWrite(BAT_SWITCH_OUT,LOW);
    //delay(300);

  }

  if(digitalRead(CAP_SWITCH_ON) == HIGH && check_C == LOW)
  {
    check_C =HIGH;
  }
   if(digitalRead(CAP_SWITCH_ON) == LOW && check_C == HIGH)
  {
    check_C = LOW;
    digitalWrite(CAP_SWITCH_OUT,HIGH);
    //delay(300);

  }
  
  if(digitalRead(CAP_SWITCH_OFF) == HIGH && check_LC == LOW)
  {
    check_LC =HIGH;
  }
   if(digitalRead(CAP_SWITCH_OFF) == LOW && check_LC == HIGH)
  {
    check_LC = LOW;
    digitalWrite(CAP_SWITCH_OUT,LOW);
    //delay(300);

  }

  
}