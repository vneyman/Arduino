#include <TimerOne.h>
#include "pitches.h"

const int pinPiezo = A0;
const int thresholdLow = 100, thresholdHigh = 500;
int valuePiezo;
const int secretKnockLockLength = 3;
const int secretKnock[secretKnockLockLength] = {0,1,0};
int secretKnockCount = 0;

//Buzzer variables
const int pinBuzzer = 9;
int melody[] = {
  NOTE_C4, NOTE_G3, NOTE_G3, NOTE_A3, NOTE_G3, 0, NOTE_B3, NOTE_C4
};

// note durations: 4 = quarter note, 8 = eighth note, etc.:
int noteDurations[] = { 4, 8, 8, 4, 4, 4, 4, 4 };

//LED variables
const int pinLed = 10;
int statusLed = 0;
const int pinLedBW[] = {3,2};
volatile unsigned int statusLedBW = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(pinLed, OUTPUT);
  
  for(int i=0; i<sizeof(pinLedBW); i++){
    pinMode(pinLedBW[i], OUTPUT);
  }
  
  Timer1.initialize(2000000); //delay between each interrupt in microseconds (1 sec = 10^6 microsec)
  Timer1.attachInterrupt(ledBWStatus);
}

void loop() {
  // put your main code here, to run repeatedly:
  piezoSensor();
}

void piezoSensor(){
  valuePiezo = analogRead(pinPiezo);  
  
  checkKnock(valuePiezo);

  if(statusLed == 1){
    led(255);
    }else{ led(0); }
  
  }

void checkKnock(int piezoReading){

//Serial.println(piezoReading);
//delay(100);

  if(piezoReading >= thresholdHigh){
    playBuzzerTone(1);
    
    if(secretKnock[secretKnockCount] == 1){
      secretKnockCount++;      
      }
      else{
        secretKnockCount = 0;
        }
    }else if(piezoReading >= thresholdLow){
      playBuzzerTone(0);
      
      if(secretKnock[secretKnockCount] == 0){
      secretKnockCount++;      
      }
      else{
        secretKnockCount = 0;
        }
      }else if(piezoReading > 0){
        secretKnockCount = 0;
        }

  if(secretKnockCount == secretKnockLockLength){
    statusLed = 1;
    secretKnockCount = 0;
    }
 }

void playBuzzerTone(int toneId){
  tone(pinBuzzer, melody[toneId], 1000/4);
  delay(300);
  noTone(pinBuzzer);
  }

void led(int valueLed){
  analogWrite(pinLed, valueLed);
  if(valueLed > 0) { delay(3000); }
  statusLed = 0;
  }
  
void ledBWStatus(){
  statusLedBW = !statusLedBW;
  toggleLedBW();
}

void toggleLedBW(){
  int ledHigh, ledLow;
  if(statusLedBW == 0){
    ledHigh = pinLedBW[0];
    ledLow = pinLedBW[1];
  }else{
    ledHigh = pinLedBW[1];
    ledLow = pinLedBW[0];
  }
  
  digitalWrite(ledHigh, HIGH);
  digitalWrite(ledLow, LOW);
}
