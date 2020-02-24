// include the library code:
#include "pitches.h"

// Turn on debug statements to the serial output
#define  DEBUG_ENABLE  1

#if  DEBUG_ENABLE
#define DEBUG(s, x) { Serial.print(F(s)); Serial.print(x); }
#define DEBUGS(x) Serial.print(F(x))
#define DEBUGX(x) Serial.println(x, HEX)
#else
#define DEBUG(s, x)
#define DEBUGS(x)
#define DEBUGX(x)
#endif

//SET PINS----------------------------------------------------
const int _pinLdr=A5;
const int _pinSpeaker=7;
const int _pinLedRed=6, _pinLedGreen=5;
const int _pinButton=2;

//GLOBAL VARIABLES----------------------------------------------------
byte _alarmStatus = 0; //0 - detecting; 1 - breach; 2 - sound alarm
int _alarmDelay = 2500;
int _alarmLdrTrigger = 300; //change to 700
int _alarmLength=1500;

void setup()
{
  #if DEBUG_ENABLE
    Serial.begin(115200);
    DEBUGS("\n[Laser Alarm test]");
  #endif
  
  pinMode(_pinLdr, INPUT);
  pinMode(_pinSpeaker, OUTPUT);
  pinMode(_pinLedRed, OUTPUT); pinMode(_pinLedGreen, OUTPUT);
  pinMode(_pinButton, INPUT);
  
  //TEST
  playStartupTones();
  analogWrite(_pinLedRed, 255); delay(750); analogWrite(_pinLedRed, 0); 
  analogWrite(_pinLedGreen, 255); delay(750); analogWrite(_pinLedGreen, 0);
}

void loop()
{
  //detectLDR();
  //soundAlarm();
  detectButton();
}

void detectButton()
{
  #if DEBUG_ENABLE
    DEBUGS("\nDetect Button");
  #endif
  
  int buttonValue = digitalRead(_pinButton);

  #if DEBUG_ENABLE
    DEBUG("\nButton sensor: ", buttonValue);
    delay(200);
  #endif

DEBUG("\nAlarm Status 1: ", _alarmStatus);

if(buttonValue == 1)
{
  _alarmStatus++;
    
    if(_alarmStatus > 2)
    {
      _alarmStatus=0;
      }
  
}

DEBUG("\nAlarm Status 2: ", _alarmStatus);

  alarmAction();

  delay(500);
  
}

void alarmAction(){
  #if DEBUG_ENABLE
    DEBUG("\nAlarm Status: ", _alarmStatus);
  #endif
  
  switch (_alarmStatus) {
    case 0:
      analogWrite(_pinLedRed,0);
      analogWrite(_pinLedGreen, 255);
      break;
    case 1:
      analogWrite(_pinLedGreen,0);
      analogWrite(_pinLedRed,255);
      break;
    default:
      soundAlarm();
      break;
}
  
  }

void detectLDR()
{
  #if DEBUG_ENABLE
    DEBUGS("\nDetect LDR");
  #endif
  
  int ldrValue=analogRead(_pinLdr);

  #if DEBUG_ENABLE
    DEBUG("\nLDR sensor: ", ldrValue);
    delay(200);
  #endif
  
  if(ldrValue <= _alarmLdrTrigger)
  {
    _alarmStatus=1;
  }else
  {
    _alarmStatus=0;
  }
    
}

void soundAlarm()
{
  if(_alarmStatus == 2)
    {
      delay(_alarmDelay);
      playSpaceGun();
      //playFibonacci();
      //tone(_pinSpeaker, 500); delay(_alarmLength);
    }else
    {
      digitalWrite(_pinSpeaker, LOW);
      noTone(_pinSpeaker);
    }
}

void playSpaceGun(){  
  #if DEBUG_ENABLE
      DEBUGS("\n Space Gun");
  #endif

  unsigned int shotsFired = random(5, 20);
  
  for(int i = 0; i < shotsFired; i++){
    unsigned int maximum = random(300, 800);
    
    for(int i = 0; i < maximum; i++){
      digitalWrite(_pinSpeaker, HIGH);
      delayMicroseconds(i);
      digitalWrite(_pinSpeaker, LOW);
      delayMicroseconds(i);
    }
  }
}

void playFibonacci(){
  #if DEBUG_ENABLE
      DEBUGS("\n Fibanocci");
  #endif

  int randomLength = random(20, 30);
  unsigned int fibDelay = 200;
  unsigned long fib = 1, fib1 = 1, fib2 =2;
  
  for(int i = 0; i < randomLength; i++){
    fib = fib1 + fib2;
    fib1 = fib2; fib2 = fib;
    
    tone(_pinSpeaker, fib, fibDelay);
    delay(fibDelay);
  }
}

void playStartupTones()
{
  int notesDelay = 300;
  
  //tone(_pinSpeaker, NOTE_A2, 10); noTone(_pinSpeaker);//sets the speaker
  beep(NOTE_A4, notesDelay);
  beep(NOTE_B4, notesDelay);
  beep(NOTE_C4, notesDelay);
  beep(NOTE_D4, notesDelay);
}

void beep(int note, int duration)
{
  //Play tone on buzzerPin
  tone(_pinSpeaker, note, duration);
  delay(duration);
  noTone(_pinSpeaker);
}
