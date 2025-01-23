#include <Servo.h>

#define x_knob A0
#define y_knob A1
int pos_xy[2];      // init arr for the x and y position of the knob
Servo myservo1;
Servo myservo2;
const float Vref = 4.90;
const int num_of_samples = 50;
void setup() {
  Serial.begin(9600);
  myservo1.attach(9);
  myservo2.attach(10);
  pinMode(x_knob,INPUT);
  pinMode(y_knob,INPUT);     

}

void loop() {
  
  float x_cord_raw = (float(analogRead(x_knob))/1023.0)*Vref;
  float y_cord_raw = (float(analogRead(y_knob))/1023.0)*Vref;
  int x_cord = round((180/Vref)*x_cord_raw);
  int y_cord = round((180/Vref)*y_cord_raw);
  get_pos();
  Serial.print("X:  ");
  Serial.print(pos_xy[0]);
  Serial.print("     ");
  Serial.print("Y:");
  Serial.println(pos_xy[1]);


  myservo1.write(pos_xy[0]);
  myservo2.write(pos_xy[1]);
}

void get_pos(){
  int sum_y = 0;
  int sum_x = 0;
  for(int i = 0 ; i<num_of_samples; i++){
    float x_cord_raw = (float(analogRead(x_knob))/1023.0)*Vref;
    float y_cord_raw = (float(analogRead(y_knob))/1023.0)*Vref;
    int x_cord = round((80/Vref)*x_cord_raw);
    int y_cord = round((180/Vref)*y_cord_raw);
    //Serial.print("X:-");
    //Serial.println(x_cord);
    //Serial.print("Y:-");
    //Serial.println(y_cord);
   if (x_cord >= 0 && x_cord <= 180 && y_cord >= 0 && y_cord <= 180) {
      sum_y += y_cord;
      sum_x += x_cord;
    }
    else {
      i-=1;
    }

  }
  pos_xy[0]= round(sum_x/num_of_samples);
  pos_xy[1]= round(sum_y/num_of_samples);

}