#include <MD_Parola.h>
#include <MD_MAX72xx.h>
#include <SPI.h>

// include the library code:
#include "pitches.h"
#include "StarWarsPitches.h"
#include "HappyBirthday.h"
#include "HarryPotter.h"
#include "PiratesCaribbean.h"

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

// Define the number of devices we have in the chain and the hardware interface
// NOTE: These pin numbers will probably not work with your hardware and may
// need to be adapted
#define HARDWARE_TYPE MD_MAX72XX::FC16_HW
#define MAX_DEVICES 4
#define CLK_PIN   13
#define DATA_PIN  11
#define CS_PIN    10

// HARDWARE SPI
MD_Parola P = MD_Parola(HARDWARE_TYPE, CS_PIN, MAX_DEVICES);

// constants won't change. They're used here to 
// set pin numbers:
const int _pinSongButton = A0, _pinSpeaker = 7;    // pins to determine song and speaker used by tone()
const unsigned long _btnDebounceDelay = 100;    // the debounce time; increase if the output flickers
unsigned long _btnLastDebounceTime = 0;  // the last time the output pin was toggled

unsigned int _pinSongValue;
typedef void (*songArray[])(); //object type to store song void functions

//Temperature: TMP 36 sensor/transistor
const int _pinTmp = A3;
const unsigned long _tmpUpdateDelay = 10000; //1000ms* 60 = 60000 for 1 minute
unsigned long _tmpLastUpdate;
float _tempC, _tempF, _tempCadjust = 0.0;
const float _tempF_perfect = 70., _tempF_max = 80., _tempF_min = 60.;

//RGB LEDs
int _pinLedRgb[] = {5, 6, 9};      // the number of the LED pin
unsigned int _pinCountLedRgb = ARRAY_SIZE(_pinLedRgb);
const int _maxBrightnessLedRgb = 220; //max brightness of each RGB color

// the following variables are long's because the time, measured in miliseconds,
// will quickly become a bigger number than can be stored in an int.

void setup() {
  #if DEBUG_ENABLE
    Serial.begin(115200);
    DEBUGS("\n[Birthday Box test]");
  #endif
  
  pinMode(_pinSongButton, INPUT);
  pinMode(_pinTmp, INPUT);
  
  for(int i=0; i< _pinCountLedRgb; i++){
    pinMode(_pinLedRgb[i], OUTPUT);
  }
  
  // set initial LED state
  setLedRgbOff();
  
  //play initial ABCD notes sounds, set the speaker
  playStartupTones();

  #if DEBUG_ENABLE
    int rgb[] = {_maxBrightnessLedRgb, 0, 0}; //red
    
    setLedRgbColors(rgb); delay(500);
    rgb[0] = 0; rgb[1] = _maxBrightnessLedRgb;
    setLedRgbColors(rgb); delay(500);
    rgb[1] = 0; rgb[2] = _maxBrightnessLedRgb;
    setLedRgbColors(rgb); delay(500);
  #endif
}

void loop() {
  if ((millis() - _btnLastDebounceTime) > _btnDebounceDelay || _btnLastDebounceTime > millis()) {
    readTuneButton();
    _btnLastDebounceTime = millis();
  }
  
  if ((millis() - _tmpLastUpdate) > _tmpUpdateDelay || _tmpLastUpdate > millis()) {
    setTempColors();
    _tmpLastUpdate = millis();
  }
}

void readTuneButton(){
  _pinSongValue = analogRead(_pinSongButton);
  
  #if DEBUG_ENABLE
    DEBUG("\n Pin Song Value: ", _pinSongValue); 
    //delay(500);
  #endif
  
  int rgbPurple[] = {148, 0, _maxBrightnessLedRgb};
  int rgbYellow[] = {150, 200, 0};
  int rgbOrange[] = {200, 153, 50};

  if(_pinSongValue > 900){
    setLedRgbColors(rgbPurple);
    playRandomSong(0);
    
  }else if(_pinSongValue > 300){
    setLedRgbColors(rgbOrange);
    playRandomSong(1);

  }else{
    noTone(_pinSpeaker);
  }
}

void playRandomSong(int songSet){  
    static int randomSongPrevious;

    //separate songs because uses too much dynamic memory when part of a single array
    songArray songs1 = {playStarWars, playPiratesCaribbean, playHarryPotter, 
      playHappyBirthday, playSpaceGun, playFibonacci, playRandomSound}; //playPiratesCaribbean, playSuperMario
      
    int songsCount = ARRAY_SIZE(songs1);
    int startIndex, endIndex;
    
    if(songSet > 0){
      startIndex = 3; endIndex = songsCount;
      }else{
        startIndex = 0; endIndex = 3;
        }
    
    int randomSong = random(startIndex, endIndex);

    //DEBUG("\n songsCount: ", songsCount);
    //DEBUG("\t startIndex: ", startIndex); DEBUG("\t endIndex: ", endIndex);
    
    while(randomSong == randomSongPrevious){
      randomSong = random(startIndex, endIndex);
      }

    //DEBUG("\n randomSong: ", randomSong); DEBUG("\t randomSongPrevious: ", randomSongPrevious); 
    //delay(1000);
    
    songs1[randomSong]();
    randomSongPrevious = randomSong;
  }



void setLedRgbColors(int colorValues[])
{   
  for(int i=0; i<_pinCountLedRgb; i++){ 
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
    
  for(int i=0; i<_pinCountLedRgb; i++){    
    setLedRgbColor(&_pinLedRgb[i], &rgbOff[i]);
  }
}



void playRandomSound(){
  #if DEBUG_ENABLE
      DEBUGS("\n Random Sounds");
  #endif

  int maximum = 100, randomNoteDuration;
  
  for(int i = 0; i < 50; i++){
    randomNoteDuration = random(maximum, 10*maximum);
    beep(randomNoteDuration, randomNoteDuration);
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

void soundSpaceGun(int maximum){  
  for(int i = 0; i < maximum; i++){
    digitalWrite(_pinSpeaker, HIGH);
    delayMicroseconds(i);
    digitalWrite(_pinSpeaker, LOW);
    delayMicroseconds(i);
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

void playHappyBirthday(){
  #if DEBUG_ENABLE
      DEBUGS("\n Happy Birthday");
  #endif
  
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
  #if DEBUG_ENABLE
      DEBUGS("\n Star Wars");
  #endif
  
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

void playHarryPotter(){
  #if DEBUG_ENABLE
      DEBUGS("\n Harry Potter");
  #endif
  
  unsigned int playCount = 3;
  
  for(int i = 0; i<3; i++){
    playHarryPotterMain();
    delay(2000);
  }
}

void playHarryPotterMain(){  
  beep(harryPotterNote.B, harryPotterTempo * harryPotterDuration.one);
  beep(harryPotterNote.E, harryPotterTempo * harryPotterDuration.one_half);
  beep(harryPotterNote.G, harryPotterTempo * harryPotterDuration.half);
  beep(harryPotterNote.F, harryPotterTempo * harryPotterDuration.one);
  beep(harryPotterNote.E, harryPotterTempo * harryPotterDuration.two);
  beep(harryPotterNote.B2, harryPotterTempo * harryPotterDuration.one);
  beep(harryPotterNote.A2, harryPotterTempo * harryPotterDuration.two_half);
  beep(harryPotterNote.Fs, harryPotterTempo * harryPotterDuration.two_half);
  
  beep(harryPotterNote.E, harryPotterTempo * harryPotterDuration.one_half);
  beep(harryPotterNote.G, harryPotterTempo * harryPotterDuration.half);
  beep(harryPotterNote.F, harryPotterTempo * harryPotterDuration.one);
  beep(harryPotterNote.Ds, harryPotterTempo * harryPotterDuration.two);
  beep(harryPotterNote.F, harryPotterTempo * harryPotterDuration.one);
  beep(harryPotterNote.B, harryPotterTempo * harryPotterDuration.two_half);
  }


void playPiratesCaribbean(){
  #if DEBUG_ENABLE
      DEBUGS("\n Pirates Caribbean");
  #endif
  
  int duration = 0;

  for(int i = 0; i<ARRAY_SIZE(piratesCaribbeanNotes1); i++){
    duration = piratesCaribbeanDuration1[i] * piratesCaribbeanTempo;
    beep(piratesCaribbeanNotes1[i], duration);
  }

  for(int i = 0; i<ARRAY_SIZE(piratesCaribbeanNotes1); i++){
    duration = piratesCaribbeanDuration1[i] * piratesCaribbeanTempo;
    beep(piratesCaribbeanNotes1[i], duration);
  }

  for(int i = 0; i<ARRAY_SIZE(piratesCaribbeanNotes2); i++){
    duration = piratesCaribbeanDuration2[i] * piratesCaribbeanTempo;
    beep(piratesCaribbeanNotes2[i], duration);
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

void beepDigitalWrite(int tone1, int duration) {
  for(long i = 0; i < duration * 1000L; i += tone1 * 2) {
     digitalWrite(_pinSpeaker, HIGH);
     delayMicroseconds(tone1);
     digitalWrite(_pinSpeaker, LOW);
     delayMicroseconds(tone1);
  }
}

void tmp36Sensor(){
  int tmp36Reading = analogRead(_pinTmp);
  float tmp36Voltage = tmp36Reading * 5.0;
  tmp36Voltage /= 1024.0;
  
  _tempC = (tmp36Voltage - .5)*100.0 + _tempCadjust;
  _tempF = (_tempC * 9.0/5.0) + 32.;
  
  #if DEBUG_ENABLE    
    //DEBUG("\nTemp F: ", _tempF); delay(250);
  #endif
}

void setTempColors()
{  
  tmp36Sensor(); //reads tmp36 and sets _tempF
  
  static int rgbTempColors[] = {0, _maxBrightnessLedRgb, 0};
  
  float diff = _tempF - _tempF_perfect;
  float adjust;
  
  if(diff > .5){
    adjust = min((diff/(_tempF_max - _tempF_perfect)) * _maxBrightnessLedRgb, _maxBrightnessLedRgb);
    rgbTempColors[0] = adjust; //red
    rgbTempColors[1] = max(_maxBrightnessLedRgb - adjust, 0); //green
    rgbTempColors[2] = 0.; //blue
  }else if(diff < -.5){
    adjust = min(diff/(_tempF_min - _tempF_perfect)*_maxBrightnessLedRgb, _maxBrightnessLedRgb);
    rgbTempColors[2] = adjust; //blue
    rgbTempColors[1] = max(_maxBrightnessLedRgb - adjust, 0); //green
    rgbTempColors[0] = 0.; //red
  }else{
    rgbTempColors[0] = 0.; //red
    rgbTempColors[1] = _maxBrightnessLedRgb; //green
    rgbTempColors[2] = 0.; //blue
  }

  #if DEBUG_ENABLE
    //DEBUG("\nTemp F: ", _tempF); 
    //DEBUG("\t rgbTempColors[0]: ", rgbTempColors[0]); 
    //DEBUG("\t rgbTempColors[1]: ", rgbTempColors[1]);
    //delay(1000);
  #endif

  setLedRgbColors(rgbTempColors);
}
