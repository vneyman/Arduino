#include <FastLED.h>

#define NUM_LEDS 141
#define DATA_PIN 6

#define CHIPSET     WS2811
#define COLOR_ORDER RGB
#define BRIGHTNESS  10
#define UPDATES_PER_SECOND 100

CRGB leds[NUM_LEDS];

CRGBPalette16 currentPalette;
TBlendType    currentBlending;

//Run test
const unsigned int tmpUpdateDelay = 300000;
unsigned long tmpLastUpdate;

void setup() {
  FastLED.addLeds<CHIPSET, DATA_PIN>(leds, NUM_LEDS);
  FastLED.setBrightness( BRIGHTNESS );
  
  currentPalette = RainbowStripeColors_p;
  currentBlending = NOBLEND;
  
  fill_solid( leds, NUM_LEDS, CRGB(0, 200, 0));
  FastLED.show();
  delay(1000);
  fill_solid( leds, NUM_LEDS, CRGB(0,0,200));
  FastLED.show();
  delay(1000);
  fill_solid( leds, NUM_LEDS, CRGB(200, 0, 0));
  FastLED.show();
  delay(1000);
  FastLED.clear();
  delay(1000);
  
  testOneByOne(0);
  testOneByOne(1);
}

void loop(){
  
  if(millis() - tmpLastUpdate > tmpUpdateDelay || tmpLastUpdate > millis()){
    FastLED.clear();
    testOneByOne(0);
    testOneByOne(1);
    tmpLastUpdate = millis();
  }
  
  /*leds[0] = CRGB::Red;
  FastLED.show();
  delay(500);
  
  leds[0] = CRGB::Black;
  FastLED.show();
  delay(500);
  
  FastLED.clear();
  */
  //FastLED.clear();
  //delay(1000);
    
  static uint8_t startIndex = 0;
  startIndex = startIndex + 1;
  
  FillLEDsFromPaletteColors(startIndex);
  FastLED.show();
  
  delay(1000 / UPDATES_PER_SECOND);
}

void FillLEDsFromPaletteColors(uint8_t colorIndex)
{
    uint8_t brightness = 255;
    
    for( int i = 0; i < NUM_LEDS; i++) {
        leds[i] = ColorFromPalette( currentPalette, colorIndex, brightness, currentBlending);
        colorIndex += 3;
    }
}

void testOneByOne(int showAll){ 
  int color = 225;
  
  for(int i=0; i< NUM_LEDS; i++){
    leds[i] = CRGB(color, color, color);
    FastLED.show();
    
    delay(250);
    
    if(showAll==0){
      leds[i] = CRGB::Black;
    }
  }
  
  delay(1000);
  FastLED.clear();
}
