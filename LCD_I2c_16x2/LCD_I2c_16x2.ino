/*
  LiquidCrystal Library - Hello World
 
 Demonstrates the use a 16x2 LCD display.  The LiquidCrystal
 library works with all LCD displays that are compatible with the 
 Hitachi HD44780 driver. There are many of them out there, and you
 can usually tell them by the 16-pin interface.
 
 This sketch prints "Hello World!" to the LCD
 and shows the time.
 
  The circuit:
 * LCD RS pin to digital pin 12
 * LCD Enable pin to digital pin 11
 * LCD D4 pin to digital pin 5
 * LCD D5 pin to digital pin 4
 * LCD D6 pin to digital pin 3
 * LCD D7 pin to digital pin 2
 * LCD R/W pin to ground
 * 10K resistor:
 * ends to +5V and ground
 * wiper to LCD VO pin (pin 3)
 
 Library originally added 18 Apr 2008
 by David A. Mellis
 library modified 5 Jul 2009
 by Limor Fried (http://www.ladyada.net)
 example added 9 Jul 2009
 by Tom Igoe
 modified 22 Nov 2010
 by Tom Igoe
 
 This example code is in the public domain.

 http://www.arduino.cc/en/Tutorial/LiquidCrystal
 */

// include the library code:
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// initialize the library with the numbers of the interface pins
LiquidCrystal_I2C lcd(0x3F, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);

const int _lcdColumns = 16, _lcdRows = 2;
String _blankChar = " ";

const char* message[] = {"Ready", "Set", "GO!!"};
const int sizeOfMessage = sizeof(message)/sizeof(char *);

void setup() {
  Serial.begin(9600);
  
  // set up the LCD's number of columns and rows: 
  lcd.begin(_lcdColumns, _lcdRows);
  // Print a message to the LCD.
  lcd.print("hello, world!");
  delay(2000);
  
  int startCursor = 0;
  String blank = repeatString(&_blankChar, &_lcdColumns);
  
  for(int i=0; i<sizeOfMessage; i++){    
    lcd.setCursor(0,0);
    lcd.print(blank);
    
    startCursor = _lcdColumns/2 - (int)(String(message[i]).length()/2)-1;

    lcd.setCursor(startCursor,0);
    lcd.print(message[i]);
    delay(1000);
  }
}

void loop() {
  // set the cursor to column 0, line 1
  // (note: line 1 is the second row, since counting begins with 0):
  lcd.setCursor(0, 1);
  // print the number of seconds since reset:
  
  lcdUpdate(0,1,"reset: " + (String)(millis()/1000) + " s");
  delay(1000);
}

void lcdUpdate(uint8_t charStart, uint8_t line, String value){
  int valueLength = value.length();
  //int count = 0;
  String blank = repeatString(&_blankChar, &valueLength);
  
  lcd.setCursor(charStart, line);
  lcd.print(blank);
  lcd.setCursor(charStart, line);
  lcd.print(value);
}

String repeatString(String * charToRepeat, int * repeat){
  String stringValue = "";
  int count = 1;
  
  do{
    stringValue += *charToRepeat;
    count++;
  }while(count < *repeat);
  
  return stringValue;
}
