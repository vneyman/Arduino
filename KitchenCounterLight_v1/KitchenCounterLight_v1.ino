//#include "LowPower.h"
#include <FastLED.h>

//#define IS_TEST
#define LED_PIN_TEST 13

//PIR Sensor
const int _pinPir = 2;
byte _pirState = LOW;

//Infrared sensor module (obstacle avoidance)
const int _pinIrInterrupt = 1; //Interrupt pin conversion: pin 2 = 0; pin 3 = 1
const int _pinIrLed = 12;
byte _irState = HIGH;
unsigned long _irSensorUpdateDelay = 3000;
volatile unsigned long _irSensorLastUpdate;

//WS5801 LED RGB Strip
#define LED_PIN     10
#define NUM_LEDS    140
unsigned int NUM_LED_TO_START=15; //cannot be defined wheb used in a for loop
#define LED_TYPE    WS2811
#define COLOR_ORDER GRB

#define MAX_BRIGHTNESS 200      // Full power is 255, watch the power!
#define MIN_BRIGHTNESS 0       //set to a minimum
#define BRIGHTNESS 90
unsigned int _BRIGHTNESS = BRIGHTNESS; //Needed for match operations. Define does not allow math operations.

CRGB _leds[NUM_LEDS];
CRGB _black = CRGB::Black;
//CRGB _white = CRGB::White;

//Potentiometers for LED RGB colors
int _pinPots[] = {A4, A2, A0};
unsigned int _potValues[] = { _BRIGHTNESS, _BRIGHTNESS, _BRIGHTNESS };

//LED strip on/off settings
const unsigned long _ledSameColorOnTime = 60000, _ledPotCheckIncrement = 5000, _ledPatternOnTime = 20000; // 1 minute = 60000
byte _ledIsSameColor = LOW;
volatile byte _ledIsPattern = LOW;

// List of patterns to cycle through.  Each is defined as a separate function below.
//Based on the FastLed library example: DemoReell00
typedef void (*SimplePatternList[])();
SimplePatternList _gPatterns = { confetti, sinelon, juggle, bpm };

uint8_t _gCurrentPatternNumber = 0; // Index number of which pattern is current
uint8_t _gHue = 0; // rotating "base color" used by many of the patterns

#define ARRAY_SIZE(A) (sizeof(A) / sizeof((A)[0]))
#define FRAMES_PER_SECOND  120

void setup() {
  // put your setup code here, to run once:
  //randomSeed(analogRead(5));
  delay(3000); // sanity delay
  
  pinMode(_pinPir, INPUT);
  pinMode(_pinIrLed, OUTPUT);

  FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(_leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.setBrightness( BRIGHTNESS );

  #if defined(IS_TEST)
    #warning "TESTING MODE"
    Serial.begin(115200);
    pinMode(LED_PIN_TEST, OUTPUT);
    digitalWrite(LED_PIN_TEST, LOW);
  #endif
  
  turnOffAllLeds();
  
  attachInterrupt(_pinIrInterrupt, readIsPatternIrSensor, RISING);
}

void loop(){  
  // MAIN CODE WRAPPER  
  if(digitalRead(_pinPir) == HIGH){
    _pirState = HIGH;
    turnOnLeds();
  }else{
    _pirState = LOW;
    turnOffAllLeds();
  }
}


void turnOnLeds(){
  unsigned long startTime = millis(), potLastCheckTime;
  potLastCheckTime = startTime;
  
  while(millis() - startTime <= _ledSameColorOnTime && millis() > startTime){    
    if(_ledIsSameColor == HIGH){
        if(millis() - potLastCheckTime > _ledPotCheckIncrement || potLastCheckTime > millis()){
          _ledIsSameColor = LOW;
          potLastCheckTime = millis();
        }
    }
    
    if(_ledIsSameColor == LOW && _ledIsPattern == LOW){
      _ledIsSameColor = HIGH;
      ledSameColor();
    }
    
    if(_ledIsPattern == HIGH){ 
      nextPattern(); _ledIsSameColor = LOW; }
    }

  _ledIsSameColor = LOW;
}


void ledSameColor(){
  readColorPotentiometer();
 
 CRGB color = CRGB(_potValues[0], _potValues[1], _potValues[2]);
 
 #if defined(IS_TEST)
   //String crgb = String("Red\t") + String(_potValues[0]) + String("\tGreen\t") + String(_potValues[1]) + String("\tBlue\t") + String(_potValues[2]);
   //Serial.println(crgb);
   //delay(200);
 #endif
 
  if(NUM_LED_TO_START > 0){
    for(int i=0; i< NUM_LED_TO_START; i++){
      _leds[i] = _black;
    }
  }
 
  for(int i=NUM_LED_TO_START; i< NUM_LEDS; i++){
    _leds[i] = color;
  }
  
  FastLED.show();
}

void readColorPotentiometer(){
  unsigned int potValue = 0;
  
  for(int i=0; i<ARRAY_SIZE(_pinPots); i++){
    potValue = analogRead(_pinPots[i]); delay(5);
    _potValues[i] = map(potValue, 0, 1024, MIN_BRIGHTNESS, MAX_BRIGHTNESS);
  }
  
  checkBrightnessExceedTwoColors();
  checkBrightnessMinExceed();
}

//decrease POT values by BRIGHTNESS/max when 2 or more POT values exceed BRIGHTNESS
void checkBrightnessExceedTwoColors(){
  int countExceedBrightness = 0;
  unsigned int maxValue = 0;
  
  for(int i=0; i<ARRAY_SIZE(_potValues); i++){
    if(_potValues[i] > _BRIGHTNESS){
      countExceedBrightness++;
    }
    
    if(_potValues[i] > maxValue){
      maxValue = _potValues[i];
    }
  }
 
  if(countExceedBrightness >= 2){
    for(int i=0; i<ARRAY_SIZE(_potValues); i++){
      _potValues[i] = (int)(_potValues[i] * _BRIGHTNESS / maxValue);
    }
  } 
}

//decrease POT values by 50% when minimum exceeds BRIGHTNESS
void checkBrightnessMinExceed(){
  unsigned int minValue = 9999;
  
  for(int i=0; i<ARRAY_SIZE(_potValues); i++){    
    if(_potValues[i] < minValue || minValue == 9999){
      minValue = _potValues[i];
    }
  }
  
  if(minValue > _BRIGHTNESS){
    for(int i=0; i<ARRAY_SIZE(_potValues); i++){
      _potValues[i] = (int)(_potValues[i] / 2);
    }
  }
  
}

void readIsPatternIrSensor(){
  
  if(millis() - _irSensorLastUpdate >= _irSensorUpdateDelay || _irSensorLastUpdate > millis()){
    _ledIsPattern = !_ledIsPattern;
    digitalWrite(_pinIrLed, _ledIsPattern);
    _irSensorLastUpdate = millis();
  }
}


void turnOffAllLeds(){ 
  fadeToBlackBy( _leds, NUM_LEDS, 50 );
  FastLED.show();
}


void nextPattern(){
  unsigned long startSeconds = millis();
  
  FastLED.setBrightness( 120 );
  
  while(millis() - startSeconds <= _ledPatternOnTime && _ledIsPattern == HIGH){
    _gPatterns[_gCurrentPatternNumber]();
    
      // send the 'leds' array out to the actual LED strip
    FastLED.show();  
    // insert a delay to keep the framerate modest
    FastLED.delay(1000/FRAMES_PER_SECOND);
    
    // do some periodic updates
    EVERY_N_MILLISECONDS( 20 ) { _gHue++; } // slowly cycle the "base color" through the rainbow
  }
  
  _gCurrentPatternNumber = (_gCurrentPatternNumber + 1) % ARRAY_SIZE( _gPatterns);
  FastLED.setBrightness( BRIGHTNESS );
}

void confetti() 
{
  // random colored speckles that blink in and fade smoothly
  fadeToBlackBy( _leds, NUM_LEDS, 10);
  int pos = random16(NUM_LEDS);
  _leds[pos] += CHSV( _gHue + random8(64), 200, 255);
}

void sinelon()
{
  // a colored dot sweeping back and forth, with fading trails
  fadeToBlackBy( _leds, NUM_LEDS, 20);
  int pos = beatsin16( 13, 0, NUM_LEDS-1 );
  _leds[pos] += CHSV( _gHue, 255, 192);
}

void bpm()
{
  // colored stripes pulsing at a defined Beats-Per-Minute (BPM)
  uint8_t BeatsPerMinute = 62;
  CRGBPalette16 palette = PartyColors_p;
  uint8_t beat = beatsin8( BeatsPerMinute, 64, 255);
  for( int i = 0; i < NUM_LEDS; i++) { //9948
    _leds[i] = ColorFromPalette(palette, _gHue+(i*2), beat-_gHue+(i*10));
  }
}

void juggle() {
  // eight colored dots, weaving in and out of sync with each other
  fadeToBlackBy( _leds, NUM_LEDS, 20);
  byte dothue = 0;
  for( int i = 0; i < 8; i++) {
    _leds[beatsin16( i+7, 0, NUM_LEDS-1 )] |= CHSV(dothue, 200, 255);
    dothue += 32;
  }
}

void wakeUp(){
    // Just a handler for the pin interrupt.
}
