#include "LowPower.h"

const int _pinLed = 9;
const unsigned long _minLedOnTime = 1000; // 1 minute = 60000

const int _pinPir = 2;
int _pirValue = 0, _pirState = LOW;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(_pinLed, OUTPUT);
  //pinMode(_pinPir, INPUT);

  digitalWrite(_pinLed, HIGH);
  delay(1000);
  digitalWrite(_pinLed, LOW);
  delay(1000);
}

void loop(){
  // Allow wake up pin to trigger interrupt on low.
    attachInterrupt(0, wakeUp, HIGH);
    
    // Enter power down state with ADC and BOD module disabled.
    // Wake up when wake up pin is low.
    LowPower.powerDown(SLEEP_FOREVER, ADC_OFF, BOD_OFF); 
    
    // Disable external pin interrupt on wake up pin.
    detachInterrupt(0);
    turnOnLed();
  }

void turnOnLed(){
  unsigned long startTime = millis(), startMinLedOnTime;
  unsigned int minTimeBeforeAction = 8000;
  
  while(digitalRead(_pinPir) == HIGH){
    //Serial.println(_pirState);
    if(_pirState == LOW){
      digitalWrite(_pinLed, HIGH);
      delay(1);
      _pirState = HIGH;
      }
    
    if(millis() - startTime > minTimeBeforeAction){
      ledAction();
      }
    }
  
  startMinLedOnTime = millis();
  
  while(millis() - startMinLedOnTime <= _minLedOnTime){
    _pirState = HIGH; // placeholder; replaces sleep
    }
  
  digitalWrite(_pinLed, LOW);
  _pirState = LOW;
}

void ledAction(){
  digitalWrite(_pinLed, LOW);
  delay(500);
  digitalWrite(_pinLed, HIGH);
  delay(500);
  }

void wakeUp(){
    // Just a handler for the pin interrupt.
}

void turnOnLedOLD(){
  _pirValue = digitalRead(_pinPir);  // read input value
  if (_pirValue == HIGH) {            // check if the input is HIGH
    digitalWrite(_pinLed, HIGH);  // turn LED ON
    if (_pirState == LOW) {
      // we have just turned on
      Serial.println("Motion detected!");
      // We only want to print on the output change, not state
      _pirState = HIGH;
    }
  } else {
    digitalWrite(_pinLed, LOW); // turn LED OFF
    if (_pirState == HIGH){
      // we have just turned of
      Serial.println("Motion ended!");
      // We only want to print on the output change, not state
      _pirState = LOW;
    }
  }
}
