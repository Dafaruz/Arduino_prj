#define Bat_HIGH 2
#define Bat_LOW 3
#define Cap_HIGH 4
#define Cap_LOW 5


void setup() {
  pinMode(Bat_HIGH , OUTPUT); 
  pinMode(Bat_LOW, OUTPUT); 
  pinMode(Cap_HIGH , OUTPUT); 
  pinMode(Cap_LOW, OUTPUT); 
}

int  x=100;

void button (int switch1){
    digitalWrite(switch1,HIGH);  // put your main code here, to run repeatedly:
    delay(x);
    digitalWrite(switch1,LOW);
    delay(x);
}

void loop() {
    button(Cap_HIGH);
    delay(1000);
    button(Cap_LOW);
    delay(1000);
    button(Bat_HIGH);
    delay(1000);
    button(Bat_LOW);
    delay(1000);
   
}
