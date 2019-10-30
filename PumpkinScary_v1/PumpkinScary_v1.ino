#include "pitches.h"
#include "spookyMusic.h"
// include the library code:

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

// Speaker setting. Connect to 100 holm resistor
const int _pinSpeaker = 6;    // pins to determine song and speaker used by tone()
byte _isSongPlayStatus = 0;

//PIR Sensor
const int _pinPir = 2;
const int _pinPirInterrupt = 0; //Interrupt pin conversion: pin 2 = 0; pin 3 = 1
volatile byte _pirState = LOW;

//RGB LEDs
int _pinled[] = {8, 9, 10};      // the number of the LED pin
unsigned int _ledPinCount = ARRAY_SIZE(_pinled);
const int _ledBrightnessMax = 220; //max brightness of each RGB color
unsigned int _ledBrightness = _ledBrightnessMax;
const unsigned int _ledIndexCountChange = 10;

//Runtime variables
unsigned long _runTimeLastUpdate;
const unsigned int _runTimeUpdateDelay = 15000;

// the following variables are long's because the time, measured in miliseconds,
// will quickly become a bigger number than can be stored in an int.


void setup() {
  #if DEBUG_ENABLE
    Serial.begin(115200);
    DEBUGS("\n[Pumpkin Scary test]");
  #endif

  pinMode(_pinPir, INPUT); //digital
  
  for(int i=0; i< _ledPinCount; i++){
    pinMode(_pinled[i], OUTPUT);
  }
  
  #if DEBUG_ENABLE
    for(int i=0; i< _ledPinCount; i++){
      digitalWrite(_pinled[i], HIGH);
      delay(250);
      digitalWrite(_pinled[i], LOW);
      delay(250);
    }
  #endif

  attachInterrupt(_pinPirInterrupt, readPirSensor, CHANGE);
}

void loop() {
  if ((millis() - _runTimeLastUpdate) > _runTimeUpdateDelay || _runTimeLastUpdate > millis()) {
    //readPirSensorTemp();
    _runTimeLastUpdate = millis();
  }

  #if DEBUG_ENABLE
    DEBUGS("\nPIR State");
    DEBUG("\tON or OFF: ", _pirState);
  #endif

  ledOnOff();

  if(_pirState){    
    //playRandomSound();
    //playSpookyMusic();
    //playSpaceGun();
    playSound();    
    }else{
      noTone(_pinSpeaker);
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

void playSound(){
  ///beep(2978,150);
 /// beep(3478,200);
  ///beep(3978,400);
  beep(4978,500);
  beep(4878,1000);
  //delay(1000)
}


 void playSpookyMusic()
{
  if ((millis() - _runTimeLastUpdate) < _runTimeUpdateDelay || _runTimeLastUpdate > millis()) {
  
   current_song_time = millis();
    if(spooky_music_on_off == 0)
    {
      if (current_song_time - prev_song_time >= 20){
        if(note_count+1 == spooky_music_num_tones){
          wait = 10000;
          spooky_music_on_off++;
          spooky_music_on_off %= 2;
        } else{
          prev_song_time = current_song_time;
          tone(_pinSpeaker, spookyMusicTones[note_count]);
          spooky_music_on_off++;
          spooky_music_on_off %= 2;
        }
      }
    } else {
      if (current_song_time - prev_song_time >= wait){//delays[note_count]
        prev_song_time = current_song_time;
        noTone(_pinSpeaker);
        note_count++;
        note_count %= spooky_music_num_tones;
        spooky_music_on_off++;
        spooky_music_on_off %= 2;
        wait = 200;
      }
    }

  _runTimeLastUpdate = millis();
  }
}

 
void playSpaceGun(){  
  #if DEBUG_ENABLE
      DEBUGS("\n Space Gun");
  #endif

  unsigned int shotsFired = random(5, 20);
  unsigned int maximum = 0;
  
  for(int i = 0; i < shotsFired; i++){
    maximum = random(300, 800);
    
    for(int i = 0; i < maximum; i++){
      digitalWrite(_pinSpeaker, HIGH);
      delayMicroseconds(i);
      digitalWrite(_pinSpeaker, LOW);
      delayMicroseconds(i);
    }
  }
}

void ledOnOff(){  
  for(int i=0; i< _ledPinCount; i++){
      digitalWrite(_pinled[i], _pirState);
    }  
 }

void readPirSensor(){
  _pirState = digitalRead(_pinPir);
  }

 void beep(int note, int duration)
{
  //Play tone on buzzerPin
  tone(_pinSpeaker, note, duration);
  delay(duration);
  noTone(_pinSpeaker);
}
