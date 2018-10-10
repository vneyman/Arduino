#include "numbers.h"
#include "letters.h"
#include <FastLED.h>

//LED Strip
#define LED_PIN     2
#define NUM_LEDS    25

#define MAX_BRIGHTNESS 250      // Full power is 255, watch the power!
#define MIN_BRIGHTNESS 100       //set to a minimum
int BRIGHTNESS = 150;

#define LED_TYPE    WS2811
#define COLOR_ORDER GRB

CRGB leds[NUM_LEDS];
CRGB _black = CRGB::Black;

uint8_t _updatesPerSecond=2, _delayDefault=100;

//Temperature TMP 36
const int pinTmp=A5, pinTmpLow=A4;
const unsigned long tmpUpdateDelay = 60000;
unsigned long tmpLastUpdate;
float tempC, tempF, tempCadjust= 0.0;
const int tempShowCount = 3;

//Light sensor with LDR
const int pinLdr=A0, pinLdrLed=12;
const unsigned int ldrUpdateDelay = 500, ldrThreshold = 250;
unsigned long ldrLastUpdate;
int ldrValue, ledValue;

void setup() {
  //Serial.begin(9600);
    delay( 2000 ); // power-up safety delay
    
    randomSeed(analogRead(7));
    
    pinMode(pinTmp, INPUT);
    //Set pins Gnd
    pinMode(pinTmpLow, OUTPUT);
    digitalWrite(pinTmpLow, LOW);
    
    pinMode(pinLdr, INPUT);
    pinMode(pinLdrLed, OUTPUT);
    
    FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
    FastLED.setBrightness(  BRIGHTNESS );
    
    ldrSensor();
}


void loop()
{
  if(millis() - ldrLastUpdate > ldrUpdateDelay || ldrLastUpdate > millis()){
      ldrSensor();
      ldrLastUpdate = millis();
      /*
      Serial.print("millis: "); Serial.println(ldrLastUpdate);
      Serial.print("LDR: "); Serial.println(ldrValue);
      Serial.print("BRIGHTNESS: "); Serial.println(BRIGHTNESS);
      */
  }

  if(millis() - tmpLastUpdate > tmpUpdateDelay || tmpLastUpdate > millis()){      
    displayTemperature();
    tmpLastUpdate = millis();
  }

  SelectMatrixPattern();
}


void SelectMatrixPattern()
{
    uint8_t secondHand = (millis() / 1000) % 60;
    static uint8_t lastSecond = 99; //prevents repeat of same pattern of all 60 seconds are assigned a pattern
    static uint8_t lastPattern = 99; //last pattern to prevent repeat of pattern
    
    if(lastSecond != secondHand) {
      lastSecond = secondHand;
        
      if(secondHand <= 10 & lastPattern != 0) {
        lastPattern = 0; MatrixCross(); 
      }else if(secondHand <= 15 & lastPattern != 1) {
        //Serial.println("MatrixOneByOne: ");
        lastPattern = 1; MatrixOneByOne();
      }else if(secondHand <= 25 & lastPattern != 2){
        //Serial.println("MatrixSnake: "); delay(50);
        lastPattern = 2; MatrixSnake();
      }else if(secondHand <= 35 & lastPattern != 3){
        //Serial.println("MatrixSquareInOut: ");
        lastPattern = 3;
        for(int i=0; i<random(1,5); i++){ MatrixSquareInOut(); } //run pattern 1 to 4 times
      }else if(secondHand <= 45 & lastPattern != 4) {
        //Serial.println("MatrixRandomFill: ");
        lastPattern = 4; MatrixRandomFill();
      }else{
        MatrixColorTemperature();
      }
    }
}

void MatrixCross(){
  FastLED.clear(); FastLED.delay(_delayDefault);
  
  CRGB color1 = CRGB(random8(), random(MIN_BRIGHTNESS), constrain(random8(), MIN_BRIGHTNESS, BRIGHTNESS)); //X
  CRGB color2 = CRGB(constrain(random8(), MIN_BRIGHTNESS, BRIGHTNESS), random8(), constrain(random8(), MIN_BRIGHTNESS, BRIGHTNESS)); //left
  CRGB color3 = CRGB(random8(), constrain(random8(), MIN_BRIGHTNESS, BRIGHTNESS), constrain(random8(), MIN_BRIGHTNESS, BRIGHTNESS)); //right
  CRGB color4 = CRGB(constrain(random8(), MIN_BRIGHTNESS, BRIGHTNESS), constrain(random8(), MIN_BRIGHTNESS, BRIGHTNESS), random8()); //bottom
  CRGB color5 = CRGB(constrain(random8(), MIN_BRIGHTNESS, BRIGHTNESS), constrain(random8(), MIN_BRIGHTNESS, BRIGHTNESS), constrain(random8(), MIN_BRIGHTNESS, BRIGHTNESS)); //top

  int numLeds = 12;   
  int leds10_1[] = {0, 4, 20, 24, 6, 8, 16, 18, 12, 12, 12, 12};
  
  for(int i=0; i<numLeds; i+=4){
    leds[leds10_1[i]] = color1;
    leds[leds10_1[i+1]] = color1;
    leds[leds10_1[i+2]] = color1;
    leds[leds10_1[i+3]] = color1;
    
    FastLED.show(); FastLED.delay(_delayDefault*2);
  }
  
  FastLED.delay(_delayDefault);
  
  for(int i=0; i<random(1,5); i++){
    leds[1] = color4;
    leds[2] = color4;
    leds[3] = color4;
    leds[5] = color3;
    leds[14] = color3;
    leds[15] = color3;
    leds[21] = color5;
    leds[22] = color5;
    leds[23] = color5;
    leds[9] = color2;
    leds[10] = color2;
    leds[19] = color2;
    
    FastLED.show(); FastLED.delay(_delayDefault*2);
    
    leds[7] = color4;
    leds[13] = color3;
    leds[17] = color5;
    leds[11] = color2;
    
    FastLED.show(); FastLED.delay(_delayDefault*2);
    
    for(int i=numLeds-1; i>0; i-=4){
      leds[leds10_1[i]] = _black;
      leds[leds10_1[i-1]] = _black;
      leds[leds10_1[i-2]] = _black;
      leds[leds10_1[i-3]] = _black;
      
      FastLED.show();
      FastLED.delay(_delayDefault*2);
    }
    
    FastLED.delay(_delayDefault);
    
    for(int i=0; i<numLeds; i+=4){
      leds[leds10_1[i]] = color1;
      leds[leds10_1[i+1]] = color1;
      leds[leds10_1[i+2]] = color1;
      leds[leds10_1[i+3]] = color1;
      
      FastLED.show(); FastLED.delay(_delayDefault*2);
    }
    
    FastLED.delay(_delayDefault);
    
    leds[7] = _black;
    leds[13] = _black;
    leds[17] = _black;
    leds[11] = _black;
    
    FastLED.show(); FastLED.delay(_delayDefault*2);
    
    leds[1] = _black;
    leds[2] = _black;
    leds[3] = _black;
    leds[5] = _black;
    leds[14] = _black;
    leds[15] = _black;
    leds[21] = _black;
    leds[22] = _black;
    leds[23] = _black;
    leds[9] = _black;
    leds[10] = _black;
    leds[19] = _black;
    
    FastLED.show(); FastLED.delay(_delayDefault*2);
  }
  
}

void MatrixSquareInOut(){

  CRGB color1 = CRGB(random8(), random(MIN_BRIGHTNESS), constrain(random8(), MIN_BRIGHTNESS, BRIGHTNESS)); //outside
  CRGB color2 = CRGB(random8(), constrain(random8(), MIN_BRIGHTNESS, BRIGHTNESS), random(BRIGHTNESS)); //middle
  CRGB color3 = CRGB(random8(), random8(), random8()); //center

  FastLED.clear();
  FastLED.delay(_delayDefault*2);

  int numLeds = 8;   
  int leds8_1[] = {0, 2, 4, 10, 14, 20, 22, 24};
  CRGB colors8_1[] = {color1, color1, color1, color1, color1, color1, color1, color1};
  updateLedsArray(numLeds, leds8_1, colors8_1, _delayDefault);
  
  int leds8_2[] = {1, 3, 5, 9, 15, 19, 21, 23};
  CRGB colors8_2[] = {color1, color1, color1, color1, color1, color1, color1, color1};
  updateLedsArray(numLeds, leds8_2, colors8_2, _delayDefault);
  
  numLeds = 4;
  int leds4_1[] = {8, 6, 16, 18};
  CRGB colors4_1[] = {color2, color2, color2, color2};
  updateLedsArray(numLeds, leds4_1, colors4_1, _delayDefault);
  
  int leds4_2[] = {7, 11, 13, 17};
  CRGB colors4_2[] = {color2, color2, color2, color2};
  updateLedsArray(numLeds, leds4_2, colors4_2, _delayDefault);
  
  numLeds = 1;
  int leds1_1[] = {12};
  CRGB colors1_1[] = {color3};
  updateLedsArray(numLeds, leds1_1, colors1_1, _delayDefault*2);
  
  int leds1_2[] = {12};
  CRGB colors1_2[] = {_black};
  updateLedsArray(numLeds, leds1_2, colors1_2, _delayDefault);
  
  numLeds = 4;
  int leds4_3[] = {7, 11, 13, 17};
  CRGB colors4_3[] = {_black, _black, _black, _black};
  updateLedsArray(numLeds, leds4_3, colors4_3, _delayDefault);

  int leds4_4[] = {8, 6, 16, 18};
  CRGB colors4_4[] = {_black, _black, _black, _black};
  updateLedsArray(numLeds, leds4_4, colors4_4, _delayDefault);
  
  numLeds = 8;
  int leds8_3[] = {1, 3, 5, 9, 15, 19, 21, 23};
  CRGB colors8_3[] = {_black, _black, _black, _black, _black, _black, _black, _black};
  updateLedsArray(numLeds, leds8_3, colors8_3, _delayDefault);
  
  int leds8_4[] = {0, 2, 4, 10, 14, 20, 22, 24};
  CRGB colors8_4[] = {_black, _black, _black, _black, _black, _black, _black, _black};
  updateLedsArray(numLeds, leds8_4, colors8_4, _delayDefault);
}

  // This function fills the palette with totally random colors.
void MatrixRandomFill()
{
  static int randomLed = random(NUM_LEDS);
  
  fill_solid( leds, NUM_LEDS, CRGB(MIN_BRIGHTNESS, MIN_BRIGHTNESS, MIN_BRIGHTNESS));
  FastLED.show();
  FastLED.delay(_delayDefault);
  
    for( int i = 0; i < NUM_LEDS*2; i++) {
        randomLed = random(NUM_LEDS);
        leds[randomLed] = CHSV( random(MIN_BRIGHTNESS, MAX_BRIGHTNESS), random(MIN_BRIGHTNESS, MAX_BRIGHTNESS), BRIGHTNESS);
        
        FastLED.show();
        
        ldrSensor();        
        FastLED.delay(_delayDefault);
    }
}

void MatrixOneByOne(){  
  CRGB color = CRGB(constrain(random(BRIGHTNESS), MIN_BRIGHTNESS, MAX_BRIGHTNESS),
    constrain(random(BRIGHTNESS), MIN_BRIGHTNESS, MAX_BRIGHTNESS),
    constrain(random(BRIGHTNESS), MIN_BRIGHTNESS, MAX_BRIGHTNESS));
  
  for(int i=0; i< NUM_LEDS; i++){
    leds[i] = color;
    FastLED.show();
    FastLED.delay(_delayDefault);
    
    if(i>0){ leds[i-1] = _black; }
  }
  
  FastLED.clear();
  FastLED.delay(_delayDefault);
}

void MatrixSnake(){  
  fill_solid( leds, NUM_LEDS, CRGB(MIN_BRIGHTNESS, MIN_BRIGHTNESS, MIN_BRIGHTNESS));
  FastLED.show();
  FastLED.delay(_delayDefault*2);
  
  CRGB color1 = CRGB(255, 0, 0); //red
  CRGB color2 = CRGB(255, 130, 0); //orange
  CRGB color3 = CRGB(MAX_BRIGHTNESS, MAX_BRIGHTNESS, 0); //yellow
  CRGB color4 = CRGB(170, 255, 50); //lime
  CRGB color5 = CRGB(0, 255, 0); //green
  CRGB color6 = CRGB(0, MAX_BRIGHTNESS, MAX_BRIGHTNESS); //aqua
  CRGB color7 = CRGB(0, 125, 255); //light blue
  CRGB color8 = CRGB(0, 0, 255); //blue
  CRGB color9 = CRGB(50, 0, 255); //violet
  CRGB color10 = CRGB(130, 0, 255); //purple
  CRGB color11 = CRGB(130, 0, 130); //pinkish purple
  CRGB color12 = CRGB(250, 0, 250); //pink
  
  int numLeds = 1;   
  int leds1_1[] = {24};
  CRGB colors1_1[] = {color1};  
  updateLedsArray(numLeds, leds1_1, colors1_1, _delayDefault);
  
  numLeds = 2;   
  int leds2_1[] = {23, 24};
  CRGB colors2_1[] = {color1, color2};
  updateLedsArray(numLeds, leds2_1, colors2_1, _delayDefault);
  
  numLeds = 3;   
  int leds3_1[] = {22, 23, 24};
  CRGB colors3_1[] = {color1, color2, color3};
  updateLedsArray(numLeds, leds3_1, colors3_1, _delayDefault);
  
  numLeds = 4;   
  int leds4_1[] = {21, 22, 23, 24};
  CRGB colors4_1[] = {color1, color2, color3, color4};
  updateLedsArray(numLeds, leds4_1, colors4_1, _delayDefault);
  
  numLeds = 5;
  int leds5_1[] = {20, 21, 22, 23, 24};
  CRGB colors5_1[] = {color1, color2, color3, color4, color5};
  updateLedsArray(numLeds, leds5_1, colors5_1, _delayDefault);
  
  numLeds = 6;   
  int leds6_1[] = {19, 20, 21, 22, 23, 24};
  CRGB colors6_1[] = {color1, color2, color3, color4, color5, color6};
  updateLedsArray(numLeds, leds6_1, colors6_1, _delayDefault);
  
  numLeds = 7;   
  int leds7_1[] = {10, 19, 20, 21, 22, 23, 24};
  CRGB colors7_1[] = {color1, color2, color3, color4, color5, color6, color7};
  updateLedsArray(numLeds, leds7_1, colors7_1, _delayDefault);
  
  numLeds = 8;   
  int leds8_1[] = {9, 10, 19, 20, 21, 22, 23, 24};
  CRGB colors8_1[] = {color1, color2, color3, color4, color5, color6, color7, color8};
  updateLedsArray(numLeds, leds8_1, colors8_1, _delayDefault);
  
  numLeds = 9;   
  int leds9_1[] = {0, 9, 10, 19, 20, 21, 22, 23, 24};
  CRGB colors9_1[] = {color1, color2, color3, color4, color5, color6, color7, color8, color9};
  updateLedsArray(numLeds, leds9_1, colors9_1, _delayDefault);
  
  numLeds = 10;   
  int leds10_1[] = {1, 0, 9, 10, 19, 20, 21, 22, 23, 24};
  CRGB colors10_1[] = {color1, color2, color3, color4, color5, color6, color7, color8, color9, color10};
  updateLedsArray(numLeds, leds10_1, colors10_1, _delayDefault);
  
  numLeds = 11;   
  int leds11_1[] = {2, 1, 0, 9, 10, 19, 20, 21, 22, 23, 24};
  CRGB colors11_1[] = {color1, color2, color3, color4, color5, color6, color7, color8, color9, color10, color11};
  updateLedsArray(numLeds, leds11_1, colors11_1, _delayDefault);
  
  numLeds = 12;   
  int leds12_1[] = {3, 2, 1, 0, 9, 10, 19, 20, 21, 22, 23, 24};
  CRGB colors12_1[] = {color1, color2, color3, color4, color5, color6, color7, color8, color9, color10, color11, color12};
  updateLedsArray(numLeds, leds12_1, colors12_1, _delayDefault);
  
  numLeds = 13;
  int leds13_1[] = {4, 3, 2, 1, 0, 9, 10, 19, 20, 21, 22, 23, 24};
  CRGB colors13_1[] = {color1, color2, color3, color4, color5, color6, color7, color8, color9, color10, color11, color12, _black};
  updateLedsArray(numLeds, leds13_1, colors13_1, _delayDefault);
 
  int leds13_2[] = {5, 4, 3, 2, 1, 0, 9, 10, 19, 20, 21, 22, 23};
  CRGB colors13_2[] = {color1, color2, color3, color4, color5, color6, color7, color8, color9, color10, color11, color12, _black};
  updateLedsArray(numLeds, leds13_2, colors13_2, _delayDefault);
  
  int leds13_3[] = {14, 5, 4, 3, 2, 1, 0, 9, 10, 19, 20, 21, 22};
  CRGB colors13_3[] = {color1, color2, color3, color4, color5, color6, color7, color8, color9, color10, color11, color12, _black};
  updateLedsArray(numLeds, leds13_3, colors13_3, _delayDefault);
  
  int leds13_4[] = {15, 14, 5, 4, 3, 2, 1, 0, 9, 10, 19, 20, 21};
  CRGB colors13_4[] = {color1, color2, color3, color4, color5, color6, color7, color8, color9, color10, color11, color12, _black};
  updateLedsArray(numLeds, leds13_4, colors13_4, _delayDefault);
  
  int leds13_5[] = {16, 15, 14, 5, 4, 3, 2, 1, 0, 9, 10, 19, 20};
  CRGB colors13_5[] = {color1, color2, color3, color4, color5, color6, color7, color8, color9, color10, color11, color12, _black};
  updateLedsArray(numLeds, leds13_5, colors13_5, _delayDefault);
  
  int leds13_6[] = {17, 16, 15, 14, 5, 4, 3, 2, 1, 0, 9, 10, 19};
  CRGB colors13_6[] = {color1, color2, color3, color4, color5, color6, color7, color8, color9, color10, color11, color12, _black};
  updateLedsArray(numLeds, leds13_6, colors13_6, _delayDefault);
  
  leds[18] = color1;
  leds[17] = color2;
  leds[16] = color3;
  leds[15] = color4;
  leds[14] = color5;
  leds[5] = color6;
  leds[4] = color7;
  leds[3] = color8;
  leds[2] = color9;
  leds[1] = color10;
  leds[0] = color11;
  leds[9] = color12;
  leds[10] = _black;
  FastLED.show();
  FastLED.delay(_delayDefault);
  
  leds[11] = color1;
  leds[18] = color2; 
  leds[17] = color3; 
  leds[16] = color4;
  leds[15] = color5;
  leds[14] = color6;
  leds[5] = color7;
  leds[4] = color8;
  leds[3] = color9;
  leds[2] = color10;
  leds[1] = color11;
  leds[0] = color12;
  leds[9] = _black;
  FastLED.show();
  FastLED.delay(_delayDefault);
  
  leds[8] = color1; leds[11] = color2; leds[18] = color3; leds[17] = color4; leds[16] = color5;
  leds[15] = color6;
  leds[14] = color7;
  leds[5] = color8;
  leds[4] = color9;
  leds[3] = color10;
  leds[2] = color11;
  leds[1] = color12;
  leds[0] = _black;
  FastLED.show();
  FastLED.delay(_delayDefault);
  
  leds[7] = color1;
  leds[8] = color2; 
  leds[11] = color3; 
  leds[18] = color4; 
  leds[17] = color5; 
  leds[16] = color6;
  leds[15] = color7;
  leds[14] = color8;
  leds[5] = color9;
  leds[4] = color10;
  leds[3] = color11;
  leds[2] = color12;
  leds[1] = _black;
  FastLED.show();
  FastLED.delay(_delayDefault);
  
  leds[6] = color1;
  leds[7] = color2;
  leds[8] = color3; 
  leds[11] = color4;
  leds[18] = color5; 
  leds[17] = color6; 
  leds[16] = color7;
  leds[15] = color8;
  leds[14] = color9;
  leds[5] = color10;
  leds[4] = color11;
  leds[3] = color12;
  leds[2] = _black;
  FastLED.show();
  FastLED.delay(_delayDefault);
  
  leds[13] = color1;
  leds[6] = color2;
  leds[7] = color3;
  leds[8] = color4; 
  leds[11] = color5;
  leds[18] = color6; 
  leds[17] = color7; 
  leds[16] = color8;
  leds[15] = color9;
  leds[14] = color10;
  leds[5] = color11;
  leds[4] = color12;
  leds[3] = _black;
  FastLED.show();
  FastLED.delay(_delayDefault);
  
  leds[12] = color1;
  leds[13] = color2;
  leds[6] = color3;
  leds[7] = color4;
  leds[8] = color5; 
  leds[11] = color6;
  leds[18] = color7; 
  leds[17] = color8; 
  leds[16] = color9;
  leds[15] = color10;
  leds[14] = color11;
  leds[5] = color12;
  leds[4] = _black;
  FastLED.show();
  FastLED.delay(_delayDefault);
  
  leds[12] = color2;
  leds[13] = color3;
  leds[6] = color4;
  leds[7] = color5;
  leds[8] = color6; 
  leds[11] = color7;
  leds[18] = color8; 
  leds[17] = color9; 
  leds[16] = color10;
  leds[15] = color11;
  leds[14] = color12;
  leds[5] = _black;
  FastLED.show();
  FastLED.delay(_delayDefault);
  
  leds[12] = color3;
  leds[13] = color4;
  leds[6] = color5;
  leds[7] = color6;
  leds[8] = color7; 
  leds[11] = color8;
  leds[18] = color9; 
  leds[17] = color10; 
  leds[16] = color11;
  leds[15] = color12;
  leds[14] = _black;
  FastLED.show();
  FastLED.delay(_delayDefault);
  
  leds[12] = color4;
  leds[13] = color5;
  leds[6] = color6;
  leds[7] = color7;
  leds[8] = color8; 
  leds[11] = color9;
  leds[18] = color10; 
  leds[17] = color11; 
  leds[16] = color12;
  leds[15] = _black;
  FastLED.show(); FastLED.delay(_delayDefault);
  
  leds[12] = color5;
  leds[13] = color6;
  leds[6] = color7;
  leds[7] = color8;
  leds[8] = color9; 
  leds[11] = color10;
  leds[18] = color11; 
  leds[17] = color12; 
  leds[16] = _black;
  FastLED.show(); FastLED.delay(_delayDefault);
  
  leds[12] = color6;
  leds[13] = color7;
  leds[6] = color8;
  leds[7] = color9;
  leds[8] = color10; 
  leds[11] = color11;
  leds[18] = color12; 
  leds[17] = _black; 
  FastLED.show(); FastLED.delay(_delayDefault);
  
  leds[12] = color7;
  leds[13] = color8;
  leds[6] = color9;
  leds[7] = color10;
  leds[8] = color11; 
  leds[11] = color12;
  leds[18] = _black; 
  FastLED.show(); FastLED.delay(_delayDefault);
  
  leds[12] = color8;
  leds[13] = color9;
  leds[6] = color10;
  leds[7] = color11;
  leds[8] = color12; 
  leds[11] = _black;
  FastLED.show(); FastLED.delay(_delayDefault);
  
  leds[12] = color9;
  leds[13] = color10;
  leds[6] = color11;
  leds[7] = color12;
  leds[8] = _black; 
  FastLED.show(); FastLED.delay(_delayDefault);
  
  leds[12] = color10;
  leds[13] = color11;
  leds[6] = color12;
  leds[7] = _black;
  FastLED.show(); FastLED.delay(_delayDefault);
  
  leds[12] = color11;
  leds[13] = color12;
  leds[6] = _black;
  FastLED.show(); FastLED.delay(_delayDefault);
  
  numLeds = 2;
  int leds2_2[] = {12, 13};
  CRGB colors2_2[] = {color12, _black};
  updateLedsArray(numLeds, leds2_2, colors2_2, _delayDefault);
  
  numLeds = 1;
  int leds1_2[] = {13};
  CRGB colors1_2[] = {_black};
  updateLedsArray(numLeds, leds1_2, colors1_2, _delayDefault);
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

int numbColorProfiles = 9;
int colorProfiles[] = {Candle, Tungsten40W, Tungsten100W, Halogen, CarbonArc, 
  HighNoonSun, DirectSunlight, OvercastSky, ClearBlueSky};
//int colorProfile = colorProfiles[random(numbColorProfiles)]; //set default
FastLED.setTemperature( colorProfiles[random(numbColorProfiles)] );

// How many seconds to show each temperature before switching
int DISPLAYTIME=20;
  
  // draw a generic, no-name rainbow
  static uint8_t starthue = 100;
  
  for(int i=0; i<DISPLAYTIME*100; i++)
  {
    fill_rainbow( leds, NUM_LEDS, --starthue, 20);
    
    FastLED.show();
    FastLED.delay(8);
  }
}

//Read LDR and set BRIGHTNESS
void ldrSensor(){
  ldrValue = analogRead(pinLdr);
  
  if(ldrValue < ldrThreshold) //low light
  {
    ledValue = 1;
    BRIGHTNESS = constrain((int)(ldrValue * 0.75), MIN_BRIGHTNESS, MAX_BRIGHTNESS);
        
  }else{
    ledValue = 0;
    BRIGHTNESS = MAX_BRIGHTNESS;
  }
      
  digitalWrite(pinLdrLed, ledValue);
  
  setFramesPerSecond();
}

void setFramesPerSecond(){    
  _updatesPerSecond = constrain((int)(ldrValue/100)*3,2,20);
  _delayDefault = 1000/_updatesPerSecond;
}


void displayTemperature(){
  CRGB color;
  
  for(int i=0; i<tempShowCount; i++){
    tmp36Sensor();
  
    String tempFstr = String((int)(tempF + .5f), DEC); //adding .5 then cast to int (which truncates) is same as round
    
    if(tempF < 65.){
      color = CHSV(HUE_BLUE, 255, BRIGHTNESS);
    }else if(tempF >= 75.){
      color = CHSV(HUE_RED, 255, BRIGHTNESS);
    }else{
      color = CHSV(HUE_GREEN, 255, BRIGHTNESS);
    }
    
      for(int i=0; i<tempFstr.length(); i++){
        numberToIntArray(tempFstr[i], color);
    }
    
    letterToIntArray('F', CHSV(HUE_YELLOW, 255, BRIGHTNESS));
  }
}

void tmp36Sensor(){
  int reading = analogRead(pinTmp);
  float tmp36Voltage = reading * 5.0;
  tmp36Voltage /= 1024.0;
  
  tempC = (tmp36Voltage - .5)*100.0 + tempCadjust;
  tempF = (tempC * 9.0/5.0) + 32.;
  
  //Serial.print("Temp F: "); Serial.println(tempF);
}

void numberToIntArray(char number, CRGB color){
//  Serial.print("from numberToIntArray: ");
//Serial.print("number: "); Serial.println(myNumber);
  
  int myNumber = number - '0'; //converts single char to int
  
  switch(myNumber){
    case 0:
      showCharacter(numb0,sizeof(numb0)/sizeof(int), color);
      break;
    case 1:
      showCharacter(numb1,sizeof(numb1)/sizeof(int), color);
      break;
    case 2:
      showCharacter(numb2,sizeof(numb2)/sizeof(int), color);
      break;
    case 3:
      showCharacter(numb3,sizeof(numb3)/sizeof(int), color);
      break;
    case 4:
      showCharacter(numb4,sizeof(numb4)/sizeof(int), color);
      break;
    case 5:
      showCharacter(numb5,sizeof(numb5)/sizeof(int), color);
      break;
    case 6:
      showCharacter(numb6,sizeof(numb6)/sizeof(int), color);
      break;
    case 7:
      showCharacter(numb7,sizeof(numb7)/sizeof(int), color);
      break;
    case 8:
      showCharacter(numb8,sizeof(numb8)/sizeof(int), color);
      break;
    case 9:
      showCharacter(numb9,sizeof(numb9)/sizeof(int), color);
      break;
    default:
      showCharacter(oye,sizeof(oye)/sizeof(int), color);
  }
}

void letterToIntArray(char letter, CRGB rgb){
  
  switch(letter){
    case 'F':
      //Serial.println("Yes!! F");
      showCharacter(letterF,sizeof(letterF)/sizeof(int), rgb);
      break;
    default:
      showCharacter(oye,sizeof(oye)/sizeof(int), CRGB::Red);
  }
}

void showCharacter(int number[], int n, CRGB rgb){
  FastLED.clear();
  FastLED.delay(1000);
  
  for(int i=0; i< n; i++){
    leds[number[i]] = rgb;
  }
  
  FastLED.setBrightness(BRIGHTNESS);
  FastLED.show();
  
  FastLED.delay(1000);
  FastLED.clear();
}

void updateLedsArray(int numbLeds, int ledsToLight[], CRGB colors[], int delayValue){
  for(int i=0; i<numbLeds; i++){
    leds[ledsToLight[i]] = colors[i];
  }
  
  FastLED.show();
  FastLED.delay(delayValue);
}




