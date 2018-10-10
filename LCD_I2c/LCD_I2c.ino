#include <Time.h>
#include <TimeLib.h>
#include <DS1307RTC.h>

/* YourDuino.com Example Software Sketch
 20 character 4 line I2C Display
 Backpack Interface labelled "YwRobot Arduino LCM1602 IIC V1"
 Connect Vcc and Ground, SDA to A4, SCL to A5 on Arduino
 terry@yourduino.com */

/*-----( Import needed libraries )-----*/
#include <Wire.h>  // Comes with Arduino IDE
// Get the LCD I2C Library here: 
// https://bitbucket.org/fmalpartida/new-liquidcrystal/downloads
// Move any other LCD libraries to another folder or delete them
// See Library "Docs" folder for possible commands etc.
#include <LiquidCrystal_I2C.h>

/*-----( Declare Constants )-----*/
/*-----( Declare objects )-----*/
// set the LCD address to 0x27 for a 20 chars 4 line display
// Set the pins on the I2C chip used for LCD connections:
//                    addr, en,rw,rs,d4,d5,d6,d7,bl,blpol
LiquidCrystal_I2C lcd(0x3F, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);  // Set the LCD I2C address


/*-----( Declare Variables )-----*/
char readChar = 0;
String readStr = "";
int potSensorPin = 0;
int potSensorValue = 0;

int potValueColumnStart = 13;
int potValueRow = 3;

String temp;
const char* message[] = {"Ready", " Set ", "GO!! "};

void readPot(){
    
    potSensorValue = analogRead(potSensorPin);
    lcd.setCursor(potValueColumnStart,potValueRow);
    lcd.print("    ");
    lcd.setCursor(potValueColumnStart,potValueRow);
    lcd.print(potSensorValue);
  }

void setup()   /*----( SETUP: RUNS ONCE )----*/
{
  Serial.begin(9600);  // Used to type in characters

  lcd.begin(20,4);         // initialize the lcd for 20 chars 4 lines, turn on backlight

// ------- Quick 3 blinks of backlight  -------------


  for(int i = 0; i< 3; i++)
  {
    lcd.backlight();
    lcd.setCursor(7,2);
    temp = message[i];
    lcd.print(message[i]);
    delay(1000);
    lcd.noBacklight();
    delay(250);
  }
  
  lcd.clear();
  lcd.backlight(); // finish with backlight on  

//-------- Write characters on the display ------------------
  // NOTE: Cursor Position: Lines and Characters start at 0  
  lcd.setCursor(1,0); //Start at character 4 on line 0
  lcd.print("Val's Arduino LCD");
  delay(1000);
  lcd.setCursor(2,1);
  lcd.print("Potentiometer");
  lcd.setCursor(6,2);
  lcd.print("Reader");
  delay(1000);

  lcd.setCursor(potValueColumnStart - 8,potValueRow);
  lcd.print("Value:"); 

//  lcd.clear();
//  lcd.setCursor(20,0);
//  lcd.autoscroll();

//  for (int thisChar = 0; thisChar < 10; thisChar++) {
//    lcd.print(thisChar);
//    delay(250);
//  }
//  // turn off automatic scrolling
//  lcd.noAutoscroll();
  
}/*--(end setup )---*/


void loop()   /*----( LOOP: RUNS CONSTANTLY )----*/
{ 
  while(!Serial){}
  
  delay(1000);
  readPot();
}/* --(end main loop )-- */

void loopOld()   /*----( LOOP: RUNS CONSTANTLY )----*/
{ 
    // when characters arrive over the serial port...
    if (Serial.available()) {
      
      // wait a bit for the entire message to arrive
      delay(100);

      readPot();
      
      // clear the screen
      lcd.clear();
      // read all the available characters
      while (Serial.available() > 0) {
        // display each character to the LCD
        readChar = Serial.read();
        
        if(readChar != '\n'){
          readStr += readChar;
          //Serial.println(readChar);
          //lcd.write(readChar);
          }
          
        //Serial.println(readStr);
        //lcd.write(readChar);
      }

      readStr.trim();
      
      if(readStr.length()>0){
        lcd.setCursor(0,2);
        lcd.print(readStr);
      }
    }

}/* --(end main loop )-- */


/* ( THE END ) */

