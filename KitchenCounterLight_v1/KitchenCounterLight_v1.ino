#include "LowPower.h"
#include <FastLED.h>

#define isTest

//PIR Sensor
const int _pinPir = 2;
int _pirValue = 0, _pirState = LOW;

//Potentiometers for LED RGB colors
const int _potPinRed = 4, _potPinGreen = 2, _potPinBlue = 0;
unsigned int _potRedValue, _potGreenValue, _potBlueValue;

//WS5801 LED RGB Strip
#define LED_PIN     10
#define NUM_LEDS    15
#define LED_TYPE    WS2811
#define COLOR_ORDER RGB //GRB

#define MAX_BRIGHTNESS 250      // Full power is 255, watch the power!
#define MIN_BRIGHTNESS 0       //set to a minimum
int BRIGHTNESS = 200;

CRGB _leds[NUM_LEDS];
CRGB _black = CRGB::Black;

const unsigned long _ledMinOnTime = 1000; // 1 minute = 60000
unsigned int _ledIsPattern = 0;


void setup() {
  // put your setup code here, to run once:
  randomSeed(analogRead(5));
  Serial.begin(115200);
  
  //pinMode(_pinLed, OUTPUT);
  pinMode(_pinPir, INPUT);

  FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(_leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
  FastLED.setBrightness( BRIGHTNESS );

  #if defined(isTest)
    testOneByOne(1);
  #endif
}

void loop(){
  // Allow wake up pin to trigger interrupt on low.
    attachInterrupt(0, wakeUp, HIGH);
    
    // Enter power down state with ADC and BOD module disabled.
    // Wake up when wake up pin is low.
    LowPower.powerDown(SLEEP_FOREVER, ADC_OFF, BOD_OFF); 
    
    // Disable external pin interrupt on wake up pin.
    detachInterrupt(0);
    turnOnLeds();
  }

void turnOnLeds(){
  unsigned long startTime = millis(), startMinLedOnTime;
  unsigned int ledMinTimeBeforeAction = 8000;
  
  while(digitalRead(_pinPir) == HIGH){
    //Serial.println(_pirState);
    if(_pirState == LOW){
      ledAllWhite();
      _pirState = HIGH;
      }
    
    if(millis() - startTime > ledMinTimeBeforeAction){
      ledPattern1();
      _ledIsPattern = 1;
      }

    delay(5);
    }
  
  startMinLedOnTime = millis();
  
  if(_pirState == HIGH) {
    while(millis() - startMinLedOnTime <= _ledMinOnTime){
      if(_ledIsPattern == 1) { ledPattern1(); } // placeholder; replaces sleep
    }
    
    FastLED.clear();
    _ledIsPattern = 0;
    _pirState = LOW;
  }
}

void ledPattern1(){
  MatrixColorTemperature();
}


void ledAllWhite(){
  int color = constrain(225, MIN_BRIGHTNESS, MAX_BRIGHTNESS);
  
  for(int i=0; i< NUM_LEDS; i++){
    _leds[i] = CRGB(color, color, color);
  }

  FastLED.show();
}

void wakeUp(){
    // Just a handler for the pin interrupt.
}

void MatrixColorTemperature(){
  // FastLED provides these pre-conigured incandescent color profiles:
//     Candle, Tungsten40W, Tungsten100W, Halogen, CarbonArc,
//     HighNoonSun, DirectSunlight, OvercastSky, ClearBlueSky,
// FastLED provides these pre-configured gaseous-light color profiles:
//     WarmFluorescent, StandardFluorescent, CoolWhiteFluorescent,
//     FullSpectrumFluorescent, GrowLightFluorescent, BlackLightFluorescent,
//     MercuryVapor, SodiumVapor, MetalHalide, HighPressureSodium,
// FastLED also provides an "Uncorrected temperature" profile
//    UncorrectedTemperature;

const int numbColorProfiles = 9;
int colorProfiles[] = {Candle, Tungsten40W, Tungsten100W, Halogen, CarbonArc, 
  HighNoonSun, DirectSunlight, OvercastSky, ClearBlueSky};
FastLED.setTemperature( colorProfiles[random(numbColorProfiles)] );

// How many seconds to show each temperature before switching
  int DISPLAYTIME=20;
  
  // draw a generic, no-name rainbow
  static uint8_t starthue = 100;
  
  for(int i=0; i<DISPLAYTIME*100; i++)
  {
    fill_rainbow( _leds, NUM_LEDS, --starthue, 20);
    
    FastLED.show();
    FastLED.delay(8);
  }
}

void testOneByOne(int showAll){ 
  int color = constrain(225, MIN_BRIGHTNESS, MAX_BRIGHTNESS);
  
  for(int i=0; i< NUM_LEDS; i++){
    _leds[i] = CRGB(color, color, color);
    FastLED.show();
    
    delay(250);
    
    if(showAll==0){
      _leds[i] = CRGB::Black;
    }
  }
  
  delay(1000);
  FastLED.clear();
}


void turnOnLedOLD(){
  _pirValue = digitalRead(_pinPir);  // read input value
  if (_pirValue == HIGH) {            // check if the input is HIGH
    //digitalWrite(_pinLed, HIGH);  // turn LED ON
    if (_pirState == LOW) {
      // we have just turned on
      Serial.println("Motion detected!");
      // We only want to print on the output change, not state
      _pirState = HIGH;
    }
  } else {
    //digitalWrite(_pinLed, LOW); // turn LED OFF
    if (_pirState == HIGH){
      // we have just turned of
      Serial.println("Motion ended!");
      // We only want to print on the output change, not state
      _pirState = LOW;
    }
  }
}
