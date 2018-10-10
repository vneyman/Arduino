#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <TimerOne.h>

// initialize the library with the numbers of the interface pins
LiquidCrystal_I2C lcd(0x3F, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);

int redLedPin = 7;
const int yellowLedPin = 2; //software interrupt does not need to use special Pins for interrupts

//LCD variables
const int lcdRowLength = 16;

//Global Variables
int ledStatus = 0;
volatile unsigned long interruptCount = 0; // use volatile for shared variables
String interruptMsg = "Yel Blinks: ";
const int interruptMsgLen = interruptMsg.length();
  
void setup() {  
  // put your setup code here, to run once:
  lcd.begin(lcdRowLength, 2);
  
  pinMode(redLedPin, OUTPUT);
  pinMode(yellowLedPin, OUTPUT);

  Timer1.initialize(2000000); //delay between each interrupt in microseconds (1 sec = 10^6 microsec)
  Timer1.attachInterrupt(BlinkInterruptLed);
  lcdUpdate(0,0,interruptMsg);
}

void loop() {
  // put your main code here, to run repeatedly:
  
   unsigned long interruptCountCopy;  // holds a copy of the blinkCount

  // to read a variable which the interrupt code writes, we
  // must temporarily disable interrupts, to be sure it will
  // not change while we are reading.  To minimize the time
  // with interrupts off, just quickly make a copy, and then
  // use the copy while allowing the interrupt to keep working.
  noInterrupts();
  interruptCountCopy = interruptCount;
  interrupts();
  
  lcdUpdate(interruptMsgLen, 0, (String)interruptCountCopy);
  
  BlinkLed(&redLedPin);  
}

void BlinkLed(int *pin){
  int thisPin = *pin;
  
    digitalWrite(thisPin, HIGH);
    delay(500);
    digitalWrite(thisPin, LOW);
    delay(500);
  }

void BlinkInterruptLed(){  
  if(ledStatus == 0){
      ledStatus = 1;
      interruptCount++;
      }else{      
        ledStatus = 0;
      }
      
   digitalWrite(yellowLedPin, ledStatus);   
}
 
void lcdUpdate(uint8_t charStart, uint8_t line, String value){
  int valueLength = value.length();
  //int count = 0;
  String blank = repeatString(" ", valueLength);
  
  lcd.setCursor(charStart, line);
  lcd.print(blank);
  lcd.setCursor(charStart, line);
  lcd.print(value);
}

String repeatString(String charToRepeat, int repeat){
  String stringValue = "";
  int count = 1;
  
  do{
    stringValue += charToRepeat;
    count++;
  }while(count<repeat);
  
  return stringValue;
}
