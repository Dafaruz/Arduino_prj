#define RedOn A1
#define VmeterBattery A2
#define VmeterAlternator A3
#define Swich_chargeing 8
#define Swich_starter 9
bool  Engin_ON=LOW;
bool  Swich_chargeing_state ;
void setup() 
{

  pinMode(RedOn,INPUT);  //define red cable is input
  pinMode(VmeterBattery,INPUT);    // define volt meter of the batterty is input
  pinMode(VmeterAlternator,INPUT);  //define volt meter of the alternator  is input
  pinMode(Swich_chargeing,OUTPUT); 
  pinMode(Swich_starter,OUTPUT); 
  Serial.begin(115200);
}

void loop() {



  Serial.print(analogRead(RedOn));
  Serial.print("   ");
  Serial.print(analogRead(VmeterAlternator));
  Serial.print("\n");
  delay(100);




if (analogRead(RedOn)<100  )
  {

    Car_is_off();
  } 
else if (analogRead(RedOn)>800 && analogRead(VmeterAlternator)<300)
  {
    digitalWrite(Swich_chargeing , LOW);
    Car_is_on();
  }
else if (analogRead(RedOn)>800 && analogRead(VmeterAlternator)>800)
  {
    The_engin_is_on();
  }
}
/////////////////////////////////////////////////////////////////////////////////////////
/// FUNCTION BEACH//////////////////////////////////////////////////////////////////////

void Car_is_off()
{
  digitalWrite(Swich_chargeing , LOW);
  digitalWrite(Swich_starter , LOW);
}




void Car_is_on()
{
  Swich_chargeing_state=Check_Engine_status();                       // check if thge engin is on
  digitalWrite(Swich_starter , LOW);        //// --> disconect the battary 
  delay(1000);
  digitalWrite(Swich_chargeing , Swich_chargeing_state);  // if the car is on will start to charge if not   stay ythe same 

}


bool Check_Engine_status()  // function that check if the engint is  on  and return boolean value
{ 
  digitalWrite(Swich_starter , HIGH);        //// --> try to boost the battary 
  for(int i=0;i<5000;i++)                      // 20 sec semple
  {
    if (analogRead(VmeterAlternator)>350)     
    {
      delay(1);
      Engin_ON=HIGH;
      
    }
    if (analogRead(VmeterAlternator)<350)
    { 
      delay(1);
      Engin_ON=LOW;
    }
  }
  return Engin_ON;
}


void The_engin_is_on()
{
  digitalWrite(Swich_chargeing , HIGH);
  digitalWrite(Swich_starter , LOW);
  // sleep mode 
}
