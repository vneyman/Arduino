/*
// Get the LCD I2C Library here: 
// https://bitbucket.org/fmalpartida/new-liquidcrystal/downloads
// Move any other LCD libraries to another folder or delete them
// See Library "Docs" folder for possible commands etc.
*/

// include the library code:
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include "pitches.h"
#include "StarWarsPitches.h"

// initialize the library with the numbers of the interface pins
LiquidCrystal_I2C lcd(0x3F, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);
const int lcdRowLength = 16;

//Startup message variables
const char* message[] = {"Happy","Birthday","Adriano"};
const int sizeOfMessage = sizeof(message)/sizeof(char *);

//LED variables
const int pinLedRed=10, pinLedYellow=11, pinLedGrd=12;
const unsigned int ledBlinkDelay = 5000;
unsigned long ledLastUpdate;
unsigned int ledState = 0;

//Read Motor Driver Board variables
const int pinMotor1 = A1, pinMotor2 = A2;
int motorValuePin1, motorValuePin2, motorValuePin1Last=1, motorValuePin2Last;
unsigned long motorLastUpdate;
const unsigned int motorUpdateDelay = 500;

//Thermistor variables
const int pinTherm = A6;
const float thermR1static = 10000.;
float A, B, C, thermR2, thermR2log, tempC, tempF;
int thermVout;
unsigned long thermLastUpdate;
const unsigned int thermUpdateDelay = 2000;

//8-ohm speaker variables
const int pinSpeaker = 2;
int melody[] = {
  NOTE_C4, NOTE_G3,NOTE_G3, NOTE_A3, NOTE_G3,0, NOTE_B3, NOTE_C4};
// note durations: 4 = quarter note, 8 = eighth note, etc.:
int noteDurations[] = {4, 8, 8, 4,4,4,4,4 };

//Happy Birthday notes
const int length = 28, tempo = 150; // the number of notes
char notes[] = "GGAGcB GGAGdc GGxecBA yyecdc";
int beats[] = { 2, 2, 8, 8, 8, 16, 1, 2, 2, 8, 8,8, 16, 1, 2,2,8,8,8,8,16, 1,2,2,8,8,8,16 };

//Infrared sensor module (obstacle avoidance)
const int pinObstacleGrd = 5, pinObstacleVcc = 4;
const int pinObstacle = 6;
int isObstacle = HIGH, obstacleLast=1;
unsigned long obstacleLastUpdate;
const unsigned int obstacleUpdateDelay = 500;

//Runtime variables
unsigned long runTimeLastUpdate;
const unsigned int runTimeUpdateDelay = 1000;


void setup() {
  Serial.begin(9600);
  
  //Serial.println((String)pinMotor1 + " set to INPUT");
  pinMode(pinMotor1, INPUT);
  pinMode(pinMotor2, INPUT);
  
  //LED pin settings
  pinMode(pinLedRed, OUTPUT);
  pinMode(pinLedYellow, OUTPUT);
  pinMode(pinLedGrd, OUTPUT);
  digitalWrite(pinLedGrd, LOW); //ground pin
  
  //Obstacle pin settings
  pinMode(pinObstacle, INPUT);
  pinMode(pinObstacleVcc, OUTPUT);
  pinMode(pinObstacleGrd, OUTPUT);
  digitalWrite(pinObstacleVcc, HIGH); //power pin
  digitalWrite(pinObstacleGrd, LOW); //ground pin
  
  // set up the LCD's number of columns and rows: 
  lcd.begin(lcdRowLength, 2);
  startUpMessage();
  
  tone(pinSpeaker,NOTE_A2,10); noTone(pinSpeaker);//sets the speaker
  playHappyBirthday();
}

void loop() {
  readMotorDriverBoard();
  updateTemperature();
  ledBlink();
  obstacleDetect();
  runTime();
}

void readMotorDriverBoard(){
  if((millis() - motorLastUpdate) > motorUpdateDelay){
    motorValuePin1 = digitalRead(pinMotor1);
    motorValuePin2 = digitalRead(pinMotor2);
    
    if(motorValuePin1==1 && motorValuePin1Last == 0){
      lcdUpdate(0,0,"OYE!");
      motorValuePin1Last = 1;
    }else if(motorValuePin1Last == 1){
      lcdUpdate(0,0," OK ");
      motorValuePin1Last = 0;
    }
    
    if(motorValuePin2==1 && motorValuePin2Last == 0){
      playMelody();
      motorValuePin2Last = 1;
    }else if(motorValuePin2Last == 1){
      motorValuePin2Last = 0;
    }
    
    //String strValuePins = String(valuePin1, DEC) + "  " + String(valuePin2, DEC);
    //Serial.println(strValuePins);
  }
}

void runTime(){
  if((millis() - runTimeLastUpdate) > runTimeUpdateDelay){
    
    unsigned long runTimeSeconds = 0;
    int hours, minutes, seconds;
    runTimeSeconds = millis()/1000;
    
    if(runTimeSeconds >= 3600){
      hours = (int)(runTimeSeconds/3600);
      seconds = runTimeSeconds - (int)(hours*3600); //remaining seconds
      minutes = (int)(seconds/60);
      seconds = seconds - (int)(minutes*60);
    }else if(runTimeSeconds >= 60){
      hours = 0;
      seconds = runTimeSeconds - (int)(hours*3600);
      minutes = (int)(seconds/60);
      seconds = seconds - minutes*60;
    }else{
      hours = 0;
      seconds = runTimeSeconds - (int)(hours*3600);
      minutes = 0;
      seconds = seconds - minutes*60;
    }
    
    String strHours = String(hours,DEC);
    String strMinutes = String(minutes,DEC);
    String strSeconds = String(seconds,DEC);
   
    if(hours<10){ strHours = String("0" + strHours); }
    if(minutes<10){ strMinutes = String("0" + strMinutes); }
    if(seconds<10){ strSeconds = String("0" + strSeconds); }
    
    lcdUpdate(0,1,strHours + ":" + strMinutes + ":" + strSeconds);
    
    runTimeLastUpdate = millis();
  }
}

void obstacleDetect(){
  if((millis() - obstacleLastUpdate) > obstacleUpdateDelay){
    int isObstacle = digitalRead(pinObstacle);
    
    //Serial.println(isObstacle);
    
    if(isObstacle == LOW){
      if(obstacleLastUpdate % 2 == 0){
        lcdUpdate(14, 0, "SW");
        playStarWars();
      }else{
        lcdUpdate(14, 0, "MY"); //MY = Melody
        playMelody();
      }
      obstacleLast = 1;
    }else if(obstacleLast == 1){
      lcdUpdate(14, 0, "NO");
      obstacleLast = 0;
    }
  
    obstacleLastUpdate = millis();
  }
}

void ledBlink(){
  if((millis() - ledLastUpdate) > ledBlinkDelay){
    ledState = !ledState;
    
    digitalWrite(pinLedYellow, ledState);
    digitalWrite(pinLedRed, !ledState);
    
    ledLastUpdate = millis();
  }
}



void updateTemperature(){
  if(millis() - thermLastUpdate > thermUpdateDelay){
    tempF = thermistorCelcius() * 9./5. + 32.;
    tempF -=1.5; //adjustment

    char tempBuffer[9], tempSuffix[3] = "F";
    dtostrf(tempF,3,1,tempBuffer);
    strcat(tempBuffer, tempSuffix);
    
    lcdUpdate(10, 2, tempBuffer);
    
    thermLastUpdate = millis();
  }
}

float thermistorCelcius() {
 thermVout = analogRead(pinTherm);

 A = 1.129148e-03, B = 2.34125e-04, C = .876741e-07;
 thermR2 = thermR1static * (1023.0 / thermVout - 1.0);
 thermR2log = log(thermR2);
 tempC = (1.0 / (A + B*thermR2log + C*thermR2log*thermR2log*thermR2log)) - 273.15;  
 return tempC;
}

void startUpMessage(){
  int startCursor = 0;
  String blank = repeatString(" ", lcdRowLength);
  
  for(int i=0; i<sizeOfMessage; i++){
    lcd.setCursor(0,0);
    lcd.print(blank);
    
    startCursor = lcdRowLength/2 - (int)(String(message[i]).length()/2);

    lcd.setCursor(startCursor,0);
    lcd.print(message[i]);
    delay(1000);
  }
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

void playMelody(){
  for (int thisNote = 0; thisNote < 8; thisNote++) {

    // to calculate the note duration, take one second 
    // divided by the note type.
    //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
    int noteDuration = 1000/noteDurations[thisNote];
    tone(pinSpeaker, melody[thisNote],noteDuration);

    // to distinguish the notes, set a minimum time between them.
    // the note's duration + 30% seems to work well:
    int pauseBetweenNotes = noteDuration * 1.30;
    delay(pauseBetweenNotes);
    // stop the tone playing:
    noTone(pinSpeaker);
  }
}

void playHappyBirthday(){
  for (int i = 0; i < length; i++) {
   if (notes[i] == ' ') {
     delay(beats[i] * tempo); // rest
   } else {
     playNote(notes[i], beats[i] * tempo);
   }

   // pause between notes
   delay(tempo);
  }
}

void playNote(char note, int duration) {
  char names[] = {'C','D','E', 'F', 'G', 'A', 'B','c', 'd', 
                  'e', 'f', 'g', 'a', 'b','x', 'y' };

  int tones[] = { 1915, 1700, 1519, 1432, 1275, 1136, 1014,
                 956,  834,  765,  593,  468,  346,  224, 655, 715 };

  int SPEE = 5;

// play the tone corresponding to the note name

for (int i = 0; i < 17; i++) {

   if (names[i] == note) {
    int newduration = duration/SPEE;
     playTone(tones[i], newduration);
   }
  }
}

void playTone(int tone, int duration) {
  for(long i = 0; i < duration * 1000L; i += tone * 2) {
     digitalWrite(pinSpeaker, HIGH);
     delayMicroseconds(tone);
     digitalWrite(pinSpeaker, LOW);
     delayMicroseconds(tone);
  }
}

void playStarWars(){
  StarWarsFirstSection();
  StarWarsSecondSection();
}

void StarWarsFirstSection()
{
    int notes1[] = {a,a,a, f, cH, a, f,cH, a };
    int durations1[] = {500, 500,500, 350,150,500,350,150,650};
    
    for(int i; i<9;i++){
      tone(pinSpeaker, notes1[i], durations1[i]);
      delay(durations1[i]);
      noTone(pinSpeaker);
      delay(50);
    }

  delay(500);

    int notes2[] = {eH,eH,eH, fH, cH, gS, f, cH, a };
    int durations2[] = {500, 500,500, 350,150,500,350,150,650};
    
    for(int i; i<9;i++){
      tone(pinSpeaker, notes2[i], durations2[i]);
      delay(durations2[i]);
      noTone(pinSpeaker);
      delay(50);
    }

  delay(500);
}

void StarWarsSecondSection()
{
  beep(aH, 500);
  beep(a, 300);
  beep(a, 150);
  beep(aH, 500);
  beep(gSH, 325);
  beep(gH, 175);
  beep(fSH, 125);
  beep(fH, 125);    
  beep(fSH, 250);

  delay(325);

  beep(aS, 250);
  beep(dSH, 500);
  beep(dH, 325);  
  beep(cSH, 175);  
  beep(cH, 125);  
  beep(b, 125);  
  beep(cH, 250);  

  delay(350);
}

void beep(int note, int duration)
{
  //Play tone on buzzerPin
  tone(pinSpeaker, note, duration);
  delay(duration);
  noTone(pinSpeaker);
  delay(50);
}
