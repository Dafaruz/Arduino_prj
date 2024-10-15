#define Cap 5
#define Bat 6

void setup() {
  pinMode(Cap , OUTPUT); 
  pinMode(Bat , OUTPUT); 
}
int  x=500;

void loop() {
    digitalWrite(Cap,HIGH);  // put your main code here, to run repeatedly:
    delay(x);
    digitalWrite(Cap,LOW);
    digitalWrite(Bat,HIGH);  // put your main code here, to run repeatedly:
    delay(x);
    digitalWrite(Bat,LOW);
   
}
