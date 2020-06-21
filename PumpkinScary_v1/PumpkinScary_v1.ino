#include "pitches.h"
#include "spookyMusic.h"
// include the library code:

// Turn on debug statements to the serial output
#define  DEBUG_ENABLE  0

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
typedef void (*_songArray[])(); //object type to store song void functions
unsigned int _songsCount;
unsigned int _songsIndexCurrent, _songsIndexPrior;

//PIR Sensor
const int _pinPir = 2;
const int _pinPirInterrupt = 0; //Interrupt pin conversion: pin 2 = 0; pin 3 = 1
volatile byte _pirState = LOW;

//LEDs
//int _pinled[] = {8, 9, 10};      // the number of the LED pin
const int _pinled[] = {13};      // the number of the LED pin
unsigned int _ledPinCount = ARRAY_SIZE(_pinled);

//Runtime variables
unsigned long _runTimeLastUpdate;
const unsigned int _runTimeUpdateDelay = 5000;

// the following variables are long's because the time, measured in miliseconds,
// will quickly become a bigger number than can be stored in an int.


void setup() {
  #if DEBUG_ENABLE
    Serial.begin(115200);
    DEBUGS("\n[Pumpkin Scary test]");
  #endif

  pinMode(_pinPir, INPUT); //digital
  pinMode(_pinSpeaker, OUTPUT); //digital
  
  for(int i=0; i< _ledPinCount; i++){
    pinMode(_pinled[i], OUTPUT);
  }
  
    for(int i=0; i< _ledPinCount; i++){
      digitalWrite(_pinled[i], HIGH);
      delay(500);
      digitalWrite(_pinled[i], LOW);
      delay(250);
    }

  attachInterrupt(_pinPirInterrupt, readPirSensor, CHANGE);
}

void loop() {
  if ((millis() - _runTimeLastUpdate) > _runTimeUpdateDelay || _runTimeLastUpdate > millis()) {
    _runTimeLastUpdate = millis();
  }

  #if DEBUG_ENABLE
    DEBUGS("\nPIR State");
    DEBUG("\tON or OFF: ", _pirState);
  #endif

  if(_pirState){ playSong(); }
}

void playSong(){ 
    ledOnOff(1);
    
    _isSongPlayStatus = 1;
    
    static _songArray songs = {playSpookyMusic, playSampsonSound, chirp, meow2};
    _songsCount = ARRAY_SIZE(songs);

    while(_songsIndexCurrent == _songsIndexPrior){
      _songsIndexCurrent = random(0, _songsCount);
    }

  #if DEBUG_ENABLE
      DEBUGS("\n playSong function");
      DEBUG("\t_songsCount: ", _songsCount);
      DEBUG("\t_songsIndexCurrent: ", _songsIndexCurrent);
  #endif
      
      songs[_songsIndexCurrent]();
      _songsIndexPrior = _songsIndexCurrent;
      
      noTone(_pinSpeaker);
    
    _isSongPlayStatus = 0;
    //delay(1000);

    ledOnOff(0);
}

void playSampsonSound(){
  #if DEBUG_ENABLE
      DEBUGS("\n Sampson Sounds");
  #endif

  unsigned long startTime = millis();
  const unsigned int runSeconds = random(4, 12) * 1000L; //1000 is 1 second

  while((millis() - startTime) < runSeconds){
      beep(4978,500);
      beep(4878,1000);
    }
}

void playRandomSound(){
  #if DEBUG_ENABLE
      DEBUGS("\n Random Sounds");
  #endif

  int maximum = 50, randomNoteDuration;
  
  for(int i = 0; i < 50; i++){
    randomNoteDuration = random(maximum, 10*maximum);
    beep(randomNoteDuration, randomNoteDuration);
  }
}

void playSpookyMusic()
{
  #if DEBUG_ENABLE
      DEBUGS("\n Spooky Music");
  #endif
  
  unsigned long startTime = millis();
  const unsigned int runSeconds = random(4, 12) * 1000L; //1000 is 1 second

  while((millis() - startTime) < runSeconds){
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
   }
}

void chirp() {  // Bird chirp
  
  unsigned long startTime = millis();
  const unsigned int runSeconds = random(4, 12) * 1000L; //1000 is 1 second

  while((millis() - startTime) < runSeconds){
    for(uint8_t i=200; i>180; i--) { playTone(i,9); }
  }
}
 
 
void meow2() {  // cat meow (emphasis on "ow")
  unsigned long startTime = millis();
  const unsigned int runSeconds = random(4, 12) * 1000L; //1000 is 1 second

  while((millis() - startTime) < runSeconds){
      playTone(5100,55);       // "m" (short)
      playTone(394,170);       // "eee" (long)
      delay(30);               // wait a tiny bit
      
      for(uint16_t i=330; i<360; i+=2)  // vary "ooo" down
         playTone(i,10);
         
      playTone(5100,40);       // "w" (short)
  }

}

void ledOnOff(byte isOn){  
  #if DEBUG_ENABLE
    DEBUGS("\nledOnOff");
    DEBUG("\tON or OFF: ", isOn);
  #endif
  
  for(int i=0; i< _ledPinCount; i++){
      digitalWrite(_pinled[i], isOn);
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

void playTone(uint16_t tone1, uint16_t duration) {
  if(tone1 < 50 || tone1 > 15000) return;  // these do not play on a piezo
  for (long i = 0; i < duration * 1000L; i += tone1 * 2) {
     digitalWrite(_pinSpeaker, HIGH);
     delayMicroseconds(tone1);
     digitalWrite(_pinSpeaker, LOW);
     delayMicroseconds(tone1);
  }     
}
