// include the library code:
#include "pitches.h"
#include "SuperMario.h"
#include "PiratesCaribbean.h"

// Turn on debug statements to the serial output
#define  DEBUG_ENABLE 0

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
  
// Speaker setting. Connect to 100 holm resistor
const int _pinSpeaker = 7;    // pins to determine song and speaker used by tone()

//Sound Sensor module
const byte _pinSoundAnalog = A5; //, _pinSoundDigital = 2, _pinSoundDigitalInterrupt = 0;//digital is an interrupt PIN 2
int _soundValueAnalog;

//Song to play
bool _isSongPlayStatus = 0;
typedef void (*songArray[])(); //object type to store song void functions

//RGB LEDs
int _pinLedRgb[] = {5, 6, 9};      // LED pins
unsigned int _pinCountLedRgb = ARRAY_SIZE(_pinLedRgb);
const int _maxBrightnessLedRgb = 220; //max brightness of each RGB color
const unsigned long _rgbRandomDelayTime = 1000;
unsigned long _rgbRandomLastUpdate;
const unsigned int _rgb_index_count_change = 10;

// the following variables are long's because the time, measured in miliseconds,
// will quickly become a bigger number than can be stored in an int.

void setup() {
  #if DEBUG_ENABLE
    Serial.begin(115200);
    DEBUGS("\n[Father's Day test]");
  #endif
  
    //Sound sensor input pins
  pinMode(_pinSoundAnalog, INPUT);
  
  for(int i=0; i< _pinCountLedRgb; i++){
    pinMode(_pinLedRgb[i], OUTPUT);
  }
  
  // testing RGB colors during setup
  setLedRgbSetup();
  
  //play initial ABCD notes sounds, set the speaker
  playTonesSetup();
  
  //read interrupt pin 2
  attachInterrupt(0, songPlayOff, RISING);
  
  #if DEBUG_ENABLE
    DEBUGS("\n[Setup complete!]");
  #endif
}

void loop() {
  if ((millis() - _rgbRandomLastUpdate) > _rgbRandomDelayTime || _rgbRandomLastUpdate > millis()) {
    setLedRgbRandom();
    _rgbRandomLastUpdate = millis();
  }
  
  readSound();
}

void readSound(){
  unsigned long startMillis = millis();
  unsigned int peakToPeak = 0;
  
  unsigned int signalMax = 0, signalMin = 1024;
  int sampleWindow = 50;
  
  while(millis() - startMillis < sampleWindow){
    _soundValueAnalog = analogRead(_pinSoundAnalog);
    
    if(_soundValueAnalog > signalMax){
      signalMax = _soundValueAnalog;
    }else if(_soundValueAnalog < signalMin){
      signalMin = _soundValueAnalog;
    }
  }
  
  peakToPeak = signalMax - signalMin;
  int displayPeak = 0;
  
  if(peakToPeak > 10){ //remove noise
    displayPeak = peakToPeak; // map(peakToPeak, 11, 30, 0, _pinDigitalLedsCount);
    
    #if DEBUG_ENABLE
      DEBUG("\n displayPeak sound: ", displayPeak);
      delay(100);
    #endif
  }
    
  onSoundDetected(&displayPeak);
}

void onSoundDetected(int * peakValue){
  int rgbPurple[] = {148, 0, _maxBrightnessLedRgb};
  int rgbYellow[] = {150, 200, 0};
  int rgbOrange[] = {200, 153, 50};
  int rgbPeru[] = {205,133,63};
  
  if(*peakValue > 250){
    setLedRgbColors(rgbPeru);
    playSong(3);
    
  }else if(*peakValue > 100){
    setLedRgbColors(rgbOrange);
    playSong(2);
    
  }else if(*peakValue > 60){
    setLedRgbColors(rgbPurple);
    playSong(1);
    
  }else if(*peakValue > 15){
    setLedRgbColors(rgbYellow);
    playSong(0);
    
  }
}

void playSong(int songSet){ 
    _isSongPlayStatus = 1;
    
    songArray songs1 = {playFibonacci, playPiratesCaribbean, playSuperMario, playRandomSound}; 
    int songsCount = ARRAY_SIZE(songs1);
    
    if(songSet > songsCount - 1){
      songSet = songsCount - 1;
    }
    
    songs1[songSet]();
    noTone(_pinSpeaker);
    
    _isSongPlayStatus = 0;
}

void setLedRgbRandom()
{
  #if DEBUG_ENABLE
      DEBUGS("\n Random RGB");
  #endif
  
  int rgb[] = {random(0, _maxBrightnessLedRgb), 
                  random(0, _maxBrightnessLedRgb), 
                  random(0, _maxBrightnessLedRgb) };
    
  for(int i=0; i<_pinCountLedRgb; i++){
    setLedRgbColor(&_pinLedRgb[i], &rgb[i]);
  }
}

void setLedRgbRandomNew()
{
  #if DEBUG_ENABLE
      DEBUGS("\n Random RGB");
  #endif

  static int count_rgb_change = 1;
  static int rgb_index = random(0, 2);
  
  static int rgb[] = {random(0, _maxBrightnessLedRgb), 
                  random(0, _maxBrightnessLedRgb), 
                  random(0, _maxBrightnessLedRgb) };

  if(count_rgb_change > _rgb_index_count_change){
      rgb_index = random(0, 2);
    }

  rgb[rgb_index] = rgb[rgb_index] + 20;

  if(rgb[rgb_index] > _maxBrightnessLedRgb){ rgb[rgb_index] = 0; }
  
  for(int i=0; i<_pinCountLedRgb; i++){
    setLedRgbColor(&_pinLedRgb[i], &rgb[i]);
  }

  count_rgb_change++;
}

void playRandomSound(){
  #if DEBUG_ENABLE
      DEBUGS("\n Random Sounds");
  #endif

  int maximum = 100, randomNoteDuration;
  
  for(int i = 0; i < 50; i++){
    randomNoteDuration = random(maximum, 10*maximum);
    beep(randomNoteDuration, randomNoteDuration);
    
    if(!isSongPlayOn()){ break; }
  }
}

void playFibonacci(){
  #if DEBUG_ENABLE
      DEBUGS("\n Fibanocci");
  #endif

  int randomLength = random(20, 100);
  unsigned int randomDelay = random(100, 300);
  unsigned long fib = 1, fib1 = 1, fib2 = 2;
  
  for(int i = 0; i < randomLength; i++){
    fib = fib1 + fib2;
    fib1 = fib2; fib2 = fib;
    
    tone(_pinSpeaker, fib, randomDelay);
    delay(randomDelay);
    
    if(!isSongPlayOn()){ break; }
  }
}

void playPiratesCaribbean(){
  #if DEBUG_ENABLE
      DEBUGS("\n Pirates Caribbean");
  #endif
  
  int duration = 0;

  for(int i = 0; i<ARRAY_SIZE(piratesCaribbeanNotes1); i++){
    duration = piratesCaribbeanDuration1[i] * piratesCaribbeanTempo;
    beep(piratesCaribbeanNotes1[i], duration);
    if(!isSongPlayOn()){ return; }
  }

  for(int i = 0; i<ARRAY_SIZE(piratesCaribbeanNotes1); i++){
    duration = piratesCaribbeanDuration1[i] * piratesCaribbeanTempo;
    beep(piratesCaribbeanNotes1[i], duration);
    if(!isSongPlayOn()){ return; }
  }

  for(int i = 0; i<ARRAY_SIZE(piratesCaribbeanNotes2); i++){
    duration = piratesCaribbeanDuration2[i] * piratesCaribbeanTempo;
    beep(piratesCaribbeanNotes2[i], duration);
    if(!isSongPlayOn()){ return; }
  }
}

void playSuperMario(){
  #if DEBUG_ENABLE
      DEBUGS("\n Super Mario");
  #endif
  
    playSuperMarioTheme(1);
    playSuperMarioTheme(1);
    playSuperMarioTheme(2);
  }


void playSuperMarioTheme(int theme) {
  int noteDuration, pauseBetweenNotes;
  // iterate over the notes of the melody:
  //song = s;
  if (theme == 2) { 
    //int melodySizeUnderworld = ARRAY_SIZE(superMarioUnderworldMelody);
    
    for (int thisNote = 0; thisNote < ARRAY_SIZE(superMarioUnderworldMelody); thisNote++) {

      // to calculate the note duration, take one second
      // divided by the note type.
      //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
      noteDuration = 1000 / superMarioUnderworldTempo[thisNote];

      beepDigitalWriteSuperMario(_pinSpeaker, superMarioUnderworldMelody[thisNote], noteDuration);
      //beep(superMarioUnderworldMelody[thisNote], noteDuration);
      
      // to distinguish the notes, set a minimum time between them.
      // the note's duration + 30% seems to work well:
      pauseBetweenNotes = noteDuration * 1.30;
      delay(pauseBetweenNotes);

      // stop the tone playing:
      beepDigitalWriteSuperMario(_pinSpeaker, 0, noteDuration);
      
      if(!isSongPlayOn()){ break; }
    }

  } else {
    
    for (int thisNote = 0; thisNote < ARRAY_SIZE(superMarioMelody); thisNote++) {

      // to calculate the note duration, take one second
      // divided by the note type.
      //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
      noteDuration = 1000 / superMarioTempo[thisNote];

      beepDigitalWriteSuperMario(_pinSpeaker, superMarioMelody[thisNote], noteDuration);

      // to distinguish the notes, set a minimum time between them.
      // the note's duration + 30% seems to work well:
      pauseBetweenNotes = noteDuration * 1.30;
      delay(pauseBetweenNotes);

      // stop the tone playing:
      beepDigitalWriteSuperMario(_pinSpeaker, 0, noteDuration);
      
      if(!isSongPlayOn()){ break; }
    }
  }
}

void playTonesSetup()
{
  int notesDelay = 300;
  
  //tone(_pinSpeaker, NOTE_A2, 10); noTone(_pinSpeaker);//sets the speaker
  beep(NOTE_A4, notesDelay);
  beep(NOTE_B4, notesDelay);
  beep(NOTE_C4, notesDelay);
  beep(NOTE_D4, notesDelay);
}

void setLedRgbSetup(){
    int rgb[] = {_maxBrightnessLedRgb, 0, 0}; //red
    setLedRgbColors(rgb); delay(500);
    
    rgb[0] = 0; rgb[1] = _maxBrightnessLedRgb; //green
    setLedRgbColors(rgb); delay(500);
    
    rgb[1] = 0; rgb[2] = _maxBrightnessLedRgb; //blue
    setLedRgbColors(rgb); delay(500);
}

void beep(int note, int duration)
{
  //Play tone on buzzerPin
  tone(_pinSpeaker, note, duration);
  delay(duration);
  noTone(_pinSpeaker);
}


void beepDigitalWriteSuperMario(int targetPin, long frequency, long length) {
  //digitalWrite(13, HIGH);
  long delayValue = 1000000 / frequency / 2; // calculate the delay value between transitions

  //// 1 second's worth of microseconds, divided by the frequency, then split in half since
  //// there are two phases to each cycle
  long numCycles = frequency * length / 1000; // calculate the number of cycles for proper timing
  
  //// multiply frequency, which is really cycles per second, by the number of seconds to
  //// get the total number of cycles to produce
  for (long i = 0; i < numCycles; i++) { // for the calculated length of time...
    digitalWrite(targetPin, HIGH); // write the buzzer pin high to push out the diaphram
    delayMicroseconds(delayValue); // wait for the calculated delay value
    digitalWrite(targetPin, LOW); // write the buzzer pin low to pull back the diaphram
    delayMicroseconds(delayValue); // wait again or the calculated delay value
    
    if(!isSongPlayOn()){ break; }
  }
}

//turn off the songPlay variable. It is turned on when a song starts to play. Button will stop the song;
void songPlayOff(){
  _isSongPlayStatus = 0;
}

byte isSongPlayOn(){
  #if DEBUG_ENABLE //SuperMario does not play properly when this is enabled
    DEBUG("\n_isSongPlayStatus: ", _isSongPlayStatus);
    delay(100);
  #endif
  
  return _isSongPlayStatus;
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
