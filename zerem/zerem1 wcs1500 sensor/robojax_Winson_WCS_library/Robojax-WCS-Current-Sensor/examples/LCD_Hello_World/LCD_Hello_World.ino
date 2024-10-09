#include <Wire.h> 
#include <Robojax_WCS.h>

// Set the LCD address to 0x27 for a 16 chars and 2 line display
Robojax_WCS_LCD lcd(0x3f, 16, 2);

void setup()
{
  Serial.begin(9600);
  Serial.print("Robojax WCS Library");
	// initialize the LCD
	lcd.begin();

	// Turn on the blacklight and print a message.
	lcd.backlight();
	lcd.print("Robojax WCS");
  lcd.setCursor(0,1);
  lcd.print("Current Sensor");
}

void loop()
{
	lcd.printDebug();
}