/**
 * File: Robojax_WCS.cpp
Written by Ahmad Shamshiri (Robojax.com) on July 25, 2020

This librar includes the LCD librarey with I2C taken from

https://github.com/fdebrabander/Arduino-LiquidCrystal-I2C-library


 * Description: This is for Winson WCS Hall Effect Sensor.
 Watch example on how to use it https://youtu.be/z-s8UvCWGxY
 for wiring and other resources for this libray visit http://robojax.com/L/?id=231

 * Author: Ahmad Shamshiri
 * Contact: robojax.com
 * Copyright: 2020 Ahmad Shamshiri



    Robojax_WCS.cpp is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
	
Update logs	

2020-July-25 Initial wiring of this libarary	
*/


#include "Robojax_WCS.h"
#include <inttypes.h>
#include <Arduino.h>
#include <Wire.h>

char *Robojax_WCS::_version = "1.0 2020-July-29";

char *Robojax_WCS::_modelNames[] = {
          //dirct series wiring
		  "WCS38A25",//0
		  "WCS37A50",//1
		  "WCS2801",//2
		  "WCS2702",//3
		  "WCS2705",//4
		  "WCS2810",//5
		  "WCS2720",//6
		  "WCS2750",//7
		  "WCS3740",//8
		  
		  //through hole sensor
		  "WCS1500",//9
		  "WCS1600",//10
		  "WCS1700",//11
		  "WCS1800",//12
		  "WCS2800",//13
		  "WCS6800",//14
		  
		  //AC to DC Current sensor
		  "WCS2202",//15

      };
float Robojax_WCS::_sensitivity[] = {
			7.0,//WCS38A25
			3.5,//WCS37A50
			2.0,//WCS2801
			1.0,//WCS2702
			260.0,//WCS2705
			135.0,//WCS2810
			65.0,//WCS2720
			32.0,//WCS2750
			32.0,//WCS3740
			
			//through hole sensor
			11.0,//WCS1500
			22.0,//WCS1600
			33.0,//WCS1700
			66.0,//WCS1800
			70.0,//WCS2800
			65.0,//WCS6800
			
			//AC to DC Current Sensors
			525.0,//WCS2202
		
	};
	
float Robojax_WCS::_quiescent_Output_voltage [] ={
			0.5,//WCS38A25
			0.5,//WCS37A50
			0.5,//WCS2801
			0.5,//WCS2702
			0.5,//WCS2705
			0.5,//WCS2810
			0.5,//WCS2720
			0.5,//WCS2750
			0.5,//WCS3740
			
			//through hole sensor
			0.5,//WCS1500
			0.5,//WCS1600
			0.5,//WCS1700
			0.5,//WCS1800
			0.5,//WCS2800
			0.5,//WCS6800

          };
	  
// When the display powers up, it is configured as follows:
//
// 1. Display clear
// 2. Function set:
//    DL = 1; 8-bit interface data
//    N = 0; 1-line display
//    F = 0; 5x8 dot character font
// 3. Display on/off control:
//    D = 0; Display off
//    C = 0; Cursor off
//    B = 0; Blinking off
// 4. Entry mode set:
//    I/D = 1; Increment by 1
//    S = 0; No shift
//
// Note, however, that resetting the Arduino doesn't reset the LCD, so we
// can't assume that its in that state when a sketch starts (and the
// LiquidCrystal constructor is called).

Robojax_WCS::Robojax_WCS(uint8_t lcd_addr, uint8_t lcd_cols, uint8_t lcd_rows, uint8_t charsize)
{
	_addr = lcd_addr;
	_cols = lcd_cols;
	_rows = lcd_rows;
	_charsize = charsize;
	_backlightval = LCD_BACKLIGHT;
}

//Constructor for basic current dispaly on serial monitor
Robojax_WCS::Robojax_WCS(uint8_t model, uint8_t inPin, uint8_t vccPin, 
					long zeroCurrentWaitTime, uint8_t zeroCurrentLEDPin,
					uint8_t correctionValue, uint16_t iteration, float voltageReference,
					uint8_t bitResolution, bool debugOnce
					
					)
	{
		_vin = inPin;
		_model = model; 
		_vccPin = vccPin;
		_bitResolution = bitResolution;
		_voltageReference = voltageReference;
		_zeroCurrentSet =false;		
		_zeroCurrentWaitTime =zeroCurrentWaitTime;
		_zeroCurrentLEDPin = zeroCurrentLEDPin;
		_correctionValue = correctionValue;
		_iteration = iteration;
		_debugOnce = debugOnce;
		pinMode(_vccPin, OUTPUT);
		digitalWrite(_vccPin, HIGH);
		
		pinMode(_zeroCurrentLEDPin, OUTPUT);
	

						
	}
	

	
	//Constructor for LCD 
Robojax_WCS::Robojax_WCS(uint8_t lcd_addr, uint8_t lcd_cols, uint8_t lcd_rows, 
	                uint8_t model, uint8_t inPin, uint8_t vccPin, 
					long zeroCurrentWaitTime, bool showZeroCurrent,
					uint8_t correctionValue, uint16_t iteration, float voltageReference,					
					uint8_t bitResolution, bool debugOnce, char *currentTitle, char *zeroCurrentTitle,										
				    uint8_t charsize = LCD_5x8DOTS)
	{
	//WCS sensor settings
		_model = model;	
		_vin = inPin;
		_vccPin = vccPin;
		_zeroCurrentWaitTime =zeroCurrentWaitTime;		
		_showZeroCurrent = showZeroCurrent;
		_correctionValue = correctionValue;
		_iteration = iteration;
		_voltageReference = voltageReference;			
		_bitResolution = bitResolution;
		_debugOnce = debugOnce;		
		_zeroCurrentSet =false;		
		_currentTitle = currentTitle;
		_zeroCurrentTitle = zeroCurrentTitle;

		pinMode(_vccPin, OUTPUT);
		digitalWrite(_vccPin, HIGH);
		_isLCD = true;
		
		
	//LCD settings	
	_addr = lcd_addr;
	_cols = lcd_cols;
	_rows = lcd_rows;
	_charsize = charsize;
	_backlightval = LCD_BACKLIGHT;					
	}				

	
	//Constructor for LCD with VCC Pin 
Robojax_WCS::Robojax_WCS(uint8_t lcd_addr, uint8_t lcd_cols, uint8_t lcd_rows, 
	                uint8_t model, uint8_t inPin, uint8_t vccPin, 
					long zeroCurrentWaitTime, bool showZeroCurrent,
					uint8_t correctionValue, uint16_t iteration, float voltageReference,					
					uint8_t bitResolution, bool debugOnce, char *currentTitle, char *zeroCurrentTitle,	
					int lcdVccPin,
				    uint8_t charsize = LCD_5x8DOTS)
	{
	//WCS sensor settings
		_model = model;	
		_vin = inPin;
		_vccPin = vccPin;		
		_zeroCurrentWaitTime =zeroCurrentWaitTime;		
		_showZeroCurrent = showZeroCurrent;
		_correctionValue = correctionValue;
		_iteration = iteration;
		_voltageReference = voltageReference;			
		_bitResolution = bitResolution;
		_debugOnce = debugOnce;		
		_zeroCurrentSet =false;		
		_currentTitle = currentTitle;
		_zeroCurrentTitle = zeroCurrentTitle;

		pinMode(_vccPin, OUTPUT);
		digitalWrite(_vccPin, HIGH);

		pinMode(lcdVccPin, OUTPUT);
		digitalWrite(lcdVccPin, HIGH);		
		
		_isLCD = true;
		
		
	//LCD settings	
	_addr = lcd_addr;
	_cols = lcd_cols;
	_rows = lcd_rows;
	_charsize = charsize;
	_backlightval = LCD_BACKLIGHT;					
	}

	
/*
 returns the version of library
 
*/		
char *Robojax_WCS::version()
{
	return _version;
}	
/*
 starts setting the library and pins and values
 
*/	
void Robojax_WCS::start()
{

		if(_isLCD)
		{
		   setCursor(0,0);	
		   print("Robojax");
		   setCursor(8,0);	   
		   print(_modelNames[_model]);	   
		   setCursor(0,1);	
		   print("Current Sensor");	  
		   delay(_zeroCurrentWaitTime);	
		   setZeroCurrent();
	  //delay(2000);
       clearLine(0);
	   setCursor(0,0);	
		   if(_rows >2)
		   {
			   print("Robojax");
			   setCursor(8,0);	   
			   print(_modelNames[_model]);	
			   clearLine(1);	   
			   setCursor(0,1);	
			   print(_currentTitle); 
				if(_showZeroCurrent)
				{
				   clearLine(2);	   
				   setCursor(0,2);	
				   print(_zeroCurrentTitle);
				   setCursor((unsigned)strlen(_zeroCurrentTitle)-1,2);
				   print(_zeroCurrentValue);
				   print("A"); 					
				}

		   
		   }else{   
			   print(_modelNames[_model]);	
				if(_showZeroCurrent)
				{
				   print(" ZC:");			   
				   setCursor((unsigned)strlen(_modelNames[_model])+4,0);
				   print(_zeroCurrentValue);
				   print("A");						
				}
				
		   
			   clearLine(1);	   
			   setCursor(0,1);	
			   print(_currentTitle);
	   
		   
		   }
   
  
			
		}else
		{
		   delay(_zeroCurrentWaitTime);	
		   setZeroCurrent();			
		}
		

}//start()	



/*
 * readCurrent()
 * @brief reads the current and updates the variabel _current
 * @param none
 * @return none
 * Written by Ahmad Shamshiri for robojax.com
 * on July 26, 2020  in Ajax, Ontario, Canada
 */
void Robojax_WCS::readCurrent()
{
   _current=getCurrent();		
   if(_isLCD)
   {
		   clearCharacters(1,(unsigned)strlen(_currentTitle),_cols);   
		   setCursor((unsigned)strlen(_currentTitle),1);	
		   print(_current);
		   print("A"); 		   
			   
   }

}//readCurrent()

/*
 * printCurrent()
 * @brief prints  the current on serial monitor
 * @param none
 * @return none
 * Written by Ahmad Shamshiri for robojax.com
 * on May 08, 2020 at 02:45 in Ajax, Ontario, Canada
 */
void Robojax_WCS::printCurrent()
{
   Serial.print("Current:");
   Serial.print(getCurrent(),2);
   Serial.println("A");
   Serial.println();
   
 
}//printCurrent()

/*
*
* Returns the model of the sensor
*/

char *Robojax_WCS::getModel()
{
	return _modelNames[_model];
}//getModel()

/*
*
* Returns the zero current value
*/
float Robojax_WCS::getZeroCurrent()
{
	return _zeroCurrentValue;
}//getZeroCurrent()

/*
 * getCurrent()
 * @brief gets current
 * @param none

 * @return returns one of the values above
 * Written by Ahmad Shamshiri for robojax.com
 * on May 10, 2020 at 03:05 in Ajax, Ontario, Canada
 */
float Robojax_WCS::getCurrent()
{
 //robojax.com WCS Current Measurement for Arduino

    float averageSensorValue =0;
    int sensorValue ;
    float voltage, current;

    for(int i=0; i< _iteration; i++)
    {   
      sensorValue = analogRead(_vin);
      //Serial.print("S:");Serial.println(sensorValue);      
      delay(2);

        voltage = (sensorValue) * (_voltageReference /  (pow(2,_bitResolution)-1)) -( _quiescent_Output_voltage[_model] * _voltageReference) + _correctionValue ; 
       //Serial.print("V:");Serial.println(voltage);
        current  = voltage / _sensitivity[_model]  ;
 
          averageSensorValue += current;
 

    }    

    averageSensorValue /=_iteration;
    if(_zeroCurrentSet)
    {
     averageSensorValue -=_zeroCurrentValue;//subtract the zero current value
    }
    
    return   averageSensorValue;
}//getCurrent()
/*
 * setZeroCurrent()
 * measure the current before the load is connected
 * and saves it as Zero current so that value is 
 * not included in the calculation
 * Written by Ahmad Shamshiri on July 22, 2020 
 * www.Robojax.com
 */
void Robojax_WCS::setZeroCurrent()
{
  _zeroCurrentValue=getCurrent();
  _zeroCurrentSet =true;


  		if(_isLCD)
		{
		   setCursor(0,1);	
		   print("Zero Current");	 
		}else{
          digitalWrite(_zeroCurrentLEDPin, HIGH);			
		  Serial.print("Zero Value Set to: ");
		  Serial.print(_zeroCurrentValue);
		  Serial.println("A");
		  Serial.println("*********************");			
		}	   
  
  
}//setZeroCurrent()

/*
 * printDebug()
 * @brief prints  the full details of current measurement
 * @param none
 * @return none
 * Written by Ahmad Shamshiri for robojax.com
 * on July 26, 2020  in Ajax, Ontario, Canada
 */
void Robojax_WCS::printDebug()
{
  //robojax.com WCS1800 Current Measurement for Arduino
    Serial.println("Debug Details Measureing ");

	
    Serial.print("Sensor Model: \t\t");
    Serial.println(_modelNames[_model]);
 
    Serial.print("Debug Details Measureing ");
    Serial.print(_iteration);
    Serial.println(" Times");
    Serial.print("Reading from pin: \t");
    Serial.println(_vin); 
    Serial.print("Sensor VCC Pin: \t");
    Serial.println(_vccPin); 	
	
    Serial.print("Sensor Sensitivity: \t");
    Serial.print(_sensitivity[_model]);Serial.println("mV/A");     
     
    Serial.print("Zero Current Voltage: \t");
    Serial.print(_quiescent_Output_voltage[_model]* _sensitivity[_model]);Serial.println("mV");   


    Serial.print("Voltage Reference: \t");
    Serial.print(_voltageReference);Serial.println("mV"); 
	
    Serial.print("Correction Value: \t");
    Serial.print(_correctionValue);Serial.println("mV"); 	
	
    Serial.print("Bits Resolution: \t");
    Serial.print(_bitResolution);Serial.println(" bits"); 

    Serial.print("Wait for Zero Current: \t");
    Serial.print(_zeroCurrentWaitTime);Serial.println("mS"); 	
	
    Serial.println();        
 
 
    Serial.print("Average Current: ");   
    Serial.print(getCurrent());            
    Serial.println("A");  
    Serial.println();   
    while(_debugOnce);
}//printDebug



/*
   printModels()
 * @brief prints all Winson WCS Current Sensor models supported by this libary
 * @param none
 * @return does not return anything
 * Written by Ahmad Shamshiri
 * www.Robojax.com code July 29, 2020 in Ajax, Ontario, Canada
 */
void Robojax_WCS::printModels()
{
	Serial.println("Robojax Winson WCS Arduino Library");
	Serial.print("Library Version:"); Serial.println(version());
	Serial.println("List supported Winson WCS Current Sensor");
    Serial.println("#\tModel\t\tSensitivity mv/A");
		for (byte i = 0; i < (sizeof(_modelNames) / sizeof(_modelNames[0])); i++) {
			Serial.print(i+1);Serial.print(" ");
			Serial.print(_modelNames[i]);
			Serial.print("\t\t");
			Serial.println(_sensitivity[i]);

		}
	
}//printModels() end

/*
   clearCharacters(uint8_t row,uint8_t start, uint8_t stop)
 * @brief clears a line of display (erases all characters)
 * @param none
 * @return does not return anything
 * Written by Ahmad Shamshiri
 * www.Robojax.com code May 28, 2020 at 16:21 in Ajax, Ontario, Canada
 */
void Robojax_WCS::clearCharacters(uint8_t row,uint8_t start, uint8_t stop )
{
    for (int i=start; i<=stop; i++)
    {
    setCursor (i,row); //  
    write(254);
    } 

}//clearCharacters


/*
   clearLine(uint8_t line)
 * @brief clears a line of display (erases all characters)
 * @param none
 * @return does not return anything
 * Written by Ahmad Shamshiri
 * www.Robojax.com code May 28, 2020 at 16:21 in Ajax, Ontario, Canada
 */
void Robojax_WCS::clearLine(uint8_t line)
{
	  for (int i=0; i<_cols; i++)
	  {
		setCursor (i,line); //	
		write(254);
	  }	

}//clearLine(uint8_t line)


///////////////////////// start of LCD functions
void Robojax_WCS::begin() {
	Wire.begin();
	_displayfunction = LCD_4BITMODE | LCD_1LINE | LCD_5x8DOTS;

	if (_rows > 1) {
		_displayfunction |= LCD_2LINE;
	}

	// for some 1 line displays you can select a 10 pixel high font
	if ((_charsize != 0) && (_rows == 1)) {
		_displayfunction |= LCD_5x10DOTS;
	}

	// SEE PAGE 45/46 FOR INITIALIZATION SPECIFICATION!
	// according to datasheet, we need at least 40ms after power rises above 2.7V
	// before sending commands. Arduino can turn on way befer 4.5V so we'll wait 50
	delay(50);

	// Now we pull both RS and R/W low to begin commands
	expanderWrite(_backlightval);	// reset expanderand turn backlight off (Bit 8 =1)
	delay(1000);

	//put the LCD into 4 bit mode
	// this is according to the hitachi HD44780 datasheet
	// figure 24, pg 46

	// we start in 8bit mode, try to set 4 bit mode
	write4bits(0x03 << 4);
	delayMicroseconds(4500); // wait min 4.1ms

	// second try
	write4bits(0x03 << 4);
	delayMicroseconds(4500); // wait min 4.1ms

	// third go!
	write4bits(0x03 << 4);
	delayMicroseconds(150);

	// finally, set to 4-bit interface
	write4bits(0x02 << 4);

	// set # lines, font size, etc.
	command(LCD_FUNCTIONSET | _displayfunction);

	// turn the display on with no cursor or blinking default
	_displaycontrol = LCD_DISPLAYON | LCD_CURSOROFF | LCD_BLINKOFF;
	display();

	// clear it off
	clear();

	// Initialize to default text direction (for roman languages)
	_displaymode = LCD_ENTRYLEFT | LCD_ENTRYSHIFTDECREMENT;

	// set the entry mode
	command(LCD_ENTRYMODESET | _displaymode);

	home();
	start();
}

/********** high level commands, for the user! */
void Robojax_WCS::clear(){
	command(LCD_CLEARDISPLAY);// clear display, set cursor position to zero
	delayMicroseconds(2000);  // this command takes a long time!
}

void Robojax_WCS::home(){
	command(LCD_RETURNHOME);  // set cursor position to zero
	delayMicroseconds(2000);  // this command takes a long time!
}

void Robojax_WCS::setCursor(uint8_t col, uint8_t row){
	int row_offsets[] = { 0x00, 0x40, 0x14, 0x54 };
	if (row > _rows) {
		row = _rows-1;    // we count rows starting w/0
	}
	command(LCD_SETDDRAMADDR | (col + row_offsets[row]));
}

// Turn the display on/off (quickly)
void Robojax_WCS::noDisplay() {
	_displaycontrol &= ~LCD_DISPLAYON;
	command(LCD_DISPLAYCONTROL | _displaycontrol);
}
void Robojax_WCS::display() {
	_displaycontrol |= LCD_DISPLAYON;
	command(LCD_DISPLAYCONTROL | _displaycontrol);
}

// Turns the underline cursor on/off
void Robojax_WCS::noCursor() {
	_displaycontrol &= ~LCD_CURSORON;
	command(LCD_DISPLAYCONTROL | _displaycontrol);
}
void Robojax_WCS::cursor() {
	_displaycontrol |= LCD_CURSORON;
	command(LCD_DISPLAYCONTROL | _displaycontrol);
}

// Turn on and off the blinking cursor
void Robojax_WCS::noBlink() {
	_displaycontrol &= ~LCD_BLINKON;
	command(LCD_DISPLAYCONTROL | _displaycontrol);
}
void Robojax_WCS::blink() {
	_displaycontrol |= LCD_BLINKON;
	command(LCD_DISPLAYCONTROL | _displaycontrol);
}

// These commands scroll the display without changing the RAM
void Robojax_WCS::scrollDisplayLeft(void) {
	command(LCD_CURSORSHIFT | LCD_DISPLAYMOVE | LCD_MOVELEFT);
}
void Robojax_WCS::scrollDisplayRight(void) {
	command(LCD_CURSORSHIFT | LCD_DISPLAYMOVE | LCD_MOVERIGHT);
}

// This is for text that flows Left to Right
void Robojax_WCS::leftToRight(void) {
	_displaymode |= LCD_ENTRYLEFT;
	command(LCD_ENTRYMODESET | _displaymode);
}

// This is for text that flows Right to Left
void Robojax_WCS::rightToLeft(void) {
	_displaymode &= ~LCD_ENTRYLEFT;
	command(LCD_ENTRYMODESET | _displaymode);
}

// This will 'right justify' text from the cursor
void Robojax_WCS::autoscroll(void) {
	_displaymode |= LCD_ENTRYSHIFTINCREMENT;
	command(LCD_ENTRYMODESET | _displaymode);
}

// This will 'left justify' text from the cursor
void Robojax_WCS::noAutoscroll(void) {
	_displaymode &= ~LCD_ENTRYSHIFTINCREMENT;
	command(LCD_ENTRYMODESET | _displaymode);
}

// Allows us to fill the first 8 CGRAM locations
// with custom characters
void Robojax_WCS::createChar(uint8_t location, uint8_t charmap[]) {
	location &= 0x7; // we only have 8 locations 0-7
	command(LCD_SETCGRAMADDR | (location << 3));
	for (int i=0; i<8; i++) {
		write(charmap[i]);
	}
}

// Turn the (optional) backlight off/on
void Robojax_WCS::noBacklight(void) {
	_backlightval=LCD_NOBACKLIGHT;
	expanderWrite(0);
}

void Robojax_WCS::backlight(void) {
	_backlightval=LCD_BACKLIGHT;
	expanderWrite(0);
}
bool Robojax_WCS::getBacklight() {
  return _backlightval == LCD_BACKLIGHT;
}


/*********** mid level commands, for sending data/cmds */

inline void Robojax_WCS::command(uint8_t value) {
	send(value, 0);
}

inline size_t Robojax_WCS::write(uint8_t value) {
	send(value, Rs);
	return 1;
}


/************ low level data pushing commands **********/

// write either command or data
void Robojax_WCS::send(uint8_t value, uint8_t mode) {
	uint8_t highnib=value&0xf0;
	uint8_t lownib=(value<<4)&0xf0;
	write4bits((highnib)|mode);
	write4bits((lownib)|mode);
}

void Robojax_WCS::write4bits(uint8_t value) {
	expanderWrite(value);
	pulseEnable(value);
}

void Robojax_WCS::expanderWrite(uint8_t _data){
	Wire.beginTransmission(_addr);
	Wire.write((int)(_data) | _backlightval);
	Wire.endTransmission();
}

void Robojax_WCS::pulseEnable(uint8_t _data){
	expanderWrite(_data | En);	// En high
	delayMicroseconds(1);		// enable pulse must be >450ns

	expanderWrite(_data & ~En);	// En low
	delayMicroseconds(50);		// commands need > 37us to settle
}

void Robojax_WCS::load_custom_character(uint8_t char_num, uint8_t *rows){
	createChar(char_num, rows);
}

void Robojax_WCS::setBacklight(uint8_t new_val){
	if (new_val) {
		backlight();		// turn backlight on
	} else {
		noBacklight();		// turn backlight off
	}
}

void Robojax_WCS::printstr(const char c[]){
	//This function is not identical to the function used for "real" I2C displays
	//it's here so the user sketch doesn't have to be changed
	print(c);
}
