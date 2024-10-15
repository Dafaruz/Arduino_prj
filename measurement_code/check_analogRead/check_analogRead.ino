
#define boutonPin  A0




void setup() {
  pinMode(boutonPin, INPUT);
  Serial.begin(9600);
}
// Variables pour suivre l'état du bouton
int etatBouton;      // Variable pour stocker l'état actuel du bouton

void loop() {
  etatBouton = analogRead(boutonPin);
  Serial.println(etatBouton);

}
