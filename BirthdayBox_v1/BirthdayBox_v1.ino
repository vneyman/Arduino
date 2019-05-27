// include the library code:
#include "pitches.h"
#include "StarWarsPitches.h"
#include "HappyBirthday.h"


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

#define ARRAY_SIZE(A) (sizeof(A) / sizeof((A)[0]))

// constants won't change. They're used here to 
// set pin numbers:
unsigned int _pinCount;
const int _pinSongButton = A0, _pinSpeaker = 7;    // pins to determine song and speaker used by tone()
unsigned int _pinSongValue;

//Temperature: TMP 36 sensor/transistor
const int _pinTmp=A3;
const unsigned long _tmpUpdateDelay = 10000; //1000ms* 60 = 60000 for 1 minute
unsigned long _tmpLastUpdate;
float _tempC, _tempF, _tempCadjust= 0.0;
const float _tempPerfect = 70., _tempMax = 80., _tempMin = 60.;
int _pinLedRgb[] = {5, 6, 9};      // the number of the LED pin
const float _maxBrightness = 220; //max brightness of each RGB color

typedef void (*songArray[])();
//songArray songs = {playStarWars, playHappyBirthday, playSpaceGun};
songArray songs = {};
unsigned int songsCount = ARRAY_SIZE(songs);

// the following variables are long's because the time, measured in miliseconds,
// will quickly become a bigger number than can be stored in an int.
unsigned long lastDebounceTime = 0;  // the last time the output pin was toggled
unsigned long debounceDelay = 50;    // the debounce time; increase if the output flickers

void setup() {
  songs[0] = playStarWars; songs[1] = playHappyBirthday; songs[2] = playSpaceGun;
  
  #if DEBUG_ENABLE
    Serial.begin(115200);
    DEBUGS("[Birthay Box test]");
  #endif
  
  pinMode(_pinSongButton, INPUT);
  pinMode(_pinTmp, INPUT);
  
  //Set PIN modes of the PWM pins
  _pinCount = sizeof(_pinLedRgb)/sizeof(int);

  for(int i=0; i< _pinCount; i++){
    pinMode(_pinLedRgb[i], OUTPUT);
  }
  
  // set initial LED state
  setLedRgbOff();
  
  //play initial ABC notes sounds, set the speaker
  playStartupTones();

  #if DEBUG_ENABLE
    int rgb[] = {_maxBrightness, 0, 0};
    
    setLedRgbColors(rgb); delay(500);
    rgb[0] = 0; rgb[1] = _maxBrightness;
    setLedRgbColors(rgb); delay(500);
    rgb[1] = 0; rgb[2] = _maxBrightness;
    setLedRgbColors(rgb); delay(500);
  #endif
}

void loop() {
  if ((millis() - lastDebounceTime) > debounceDelay || lastDebounceTime > millis()) {
    readTuneButton();
    lastDebounceTime = millis();
  }
  
  if ((millis() - _tmpLastUpdate) > _tmpUpdateDelay || _tmpLastUpdate > millis()) {
    setTempColors();
    _tmpLastUpdate = millis();
  }
}

void readTuneButton(){
  _pinSongValue = analogRead(_pinSongButton);
  
  #if DEBUG_ENABLE
    DEBUG("\nSong PIN value: ", _pinSongValue);
  #endif
  
  int rgbPurple[] = {148, 0, _maxBrightness};
  int rgbYellow[] = {150, 200, 0};
  int rgbOrange[] = {200, 153, 50};
  
  if(_pinSongValue > 900){
    setLedRgbColors(rgbPurple);
    soundRandom(400);
    
  }else if(_pinSongValue > 400){
    setLedRgbColors(rgbYellow);
    soundSpaceGun(random(300, 800));
    //soundFibonacci(random(30, 50));
    
  }else if(_pinSongValue > 100){
    setLedRgbColors(rgbOrange);
    
    //playHappyBirthday();
    songs[random(0, songsCount)]();
  }else{
    //setLedRgbOff();
    noTone(_pinSpeaker);
  }
}

void tmp36Sensor(){
  unsigned int tmpReading = analogRead(_pinTmp);
  float tmp36Voltage = tmpReading * 5.0;
  tmp36Voltage /= 1024.0;
  
  _tempC = (tmp36Voltage - .5)*100.0 + _tempCadjust;
  _tempF = (_tempC * 9.0/5.0) + 32.;
  
  #if DEBUG_ENABLE
    //DEBUG("\nTemp F: ", _tempF);
    //delay(500);
  #endif
}

void setTempColors()
{  
  tmp36Sensor(); //reads tmp36 and sets _tempF
  
  static int rgbTempColors[] = {0,(int)_maxBrightness,0};
  
  float diff = _tempF - _tempPerfect;
  float adjust;
  
  if(diff > .5){
    adjust = (diff/(_tempMax - _tempPerfect)) * _maxBrightness;
    
    rgbTempColors[0] = adjust; //red
    rgbTempColors[1] = _maxBrightness - adjust; //green
    rgbTempColors[2] = 0.; //blue
  }else if(diff < -.5){
    adjust = diff/(_tempMin - _tempPerfect)*_maxBrightness;
    rgbTempColors[2] = adjust; //blue
    rgbTempColors[1] = _maxBrightness - adjust; //green
    rgbTempColors[0] = 0.; //red
  }else{
    rgbTempColors[0] = 0.; //red
    rgbTempColors[1] = (int)_maxBrightness; //green
    rgbTempColors[2] = 0.; //blue
  }
  
  setLedRgbColors(rgbTempColors);
}


void setLedRgbColors(int colorValues[])
{  
  _pinCount = sizeof(_pinLedRgb)/sizeof(int);
  
  for(int i=0; i<_pinCount; i++){ 
    setLedRgbColor(&_pinLedRgb[i], &colorValues[i]);
  }

}

void setLedRgbColor(int * pin, int * colorValue)
{
  analogWrite(*pin, *colorValue);
  delay(2);
}

void setLedRgbOff()
{
  int rgbOff[] = {0, 0, 0};
  
  _pinCount = sizeof(_pinLedRgb)/sizeof(int);
  
  for(int i=0; i<_pinCount; i++){    
    setLedRgbColor(&_pinLedRgb[i], &rgbOff[i]);
  }
}

void soundRandom(int maximum){
  tone(_pinSpeaker, random(maximum, 10*maximum));
  delay(maximum);
}

void soundSpaceGun(int maximum){  
  for(int i = 0; i < maximum; i++){
    digitalWrite(_pinSpeaker, HIGH);
    delayMicroseconds(i);
    digitalWrite(_pinSpeaker, LOW);
    delayMicroseconds(i);
  }
}

void playSpaceGun(){  
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

void soundFibonacci(int maximum){
  unsigned int fibDelay = 200;
  unsigned long fib = 1, fib1 = 1, fib2 =2;
  
  for(int i = 0; i < maximum; i++){
    fib = fib1 + fib2;
    fib1 = fib2; fib2 = fib;
    
    tone(_pinSpeaker, fib, fibDelay);
    delay(fibDelay);
  }
}

void playHappyBirthday(){  
  for (int i = 0; i < ARRAY_SIZE(happyBirthdayBeats); i++) {
   if (happyBirthdayNotes[i] == ' ') {
     delay(happyBirthdayBeats[i] * happyBirthdayTempo); // rest
   } else {
     HappyBirthdayTones(happyBirthdayNotes[i], happyBirthdayBeats[i] * happyBirthdayTempo);
   }

   // pause between happyBirthdayNotes
   delay(happyBirthdayTempo);
  }
}

void HappyBirthdayTones(char note, int duration) {
  int SPEE = 5;

  // play the tone corresponding to the note name
  for (int i = 0; i < ARRAY_SIZE(happyBirthdayNoteNames); i++) {
     if (happyBirthdayNoteNames[i] == note) {
      int newduration = duration/SPEE;
       beepDigitalWrite(happyBirthdayNoteTones[i], newduration);
     }
    }
}

void playStarWars(){
  playStarWarsFirstSection();
  playStarWarsSecondSection();
}

void playStarWarsFirstSection()
{
    for(int i; i < ARRAY_SIZE(starWarsNotesS1_1);i++){
      beep(starWarsNotesS1_1[i], starWarsDurationsS1_1[i]);
    }

  delay(500);
  
    for(int i; i < ARRAY_SIZE(starWarsNotesS1_2);i++){
      beep(starWarsNotesS1_2[i], starWarsDurationsS1_2[i]);
    }

  delay(500);
}

void playStarWarsSecondSection()
{    
    for(int i; i < ARRAY_SIZE(starWarsNotesS2_1);i++){
      beep(starWarsNotesS2_1[i], starWarsDurationsS2_1[i]);
    }

  delay(325);

    for(int i; i< ARRAY_SIZE(starWarsNotesS2_2); i++){
      beep(starWarsNotesS2_2[i], starWarsDurationsS2_2[i]);
    }

  delay(350);
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
  delay(50);
}

void beepDigitalWrite(int tone1, int duration) {
  for(long i = 0; i < duration * 1000L; i += tone1 * 2) {
     digitalWrite(_pinSpeaker, HIGH);
     delayMicroseconds(tone1);
     digitalWrite(_pinSpeaker, LOW);
     delayMicroseconds(tone1);
  }
}
