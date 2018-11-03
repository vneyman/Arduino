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

//Display Adriano's name
char _name[] = "ADRIANO";
const unsigned long _nameDelay = 130000;
unsigned long _nameLastUpdate;
const int _nameShowCount = 2;

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
    
    CRGB colorHappy = CRGB(random(MIN_BRIGHTNESS),MAX_BRIGHTNESS,random(BRIGHTNESS));
    showCharacter(happy,sizeof(happy)/sizeof(int), colorHappy);
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
  
  if(millis() - _nameLastUpdate > _nameDelay || _nameLastUpdate > millis() || _nameLastUpdate==0){      
    displayName();
    _nameLastUpdate = millis();
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

  
  leds[24] = color1;
  FastLED.show(); FastLED.delay(_delayDefault);
  
  leds[23] = color1;
  leds[24] = color2;
  FastLED.show(); FastLED.delay(_delayDefault);
  
  leds[22] = color1;
  leds[23] = color2;
  leds[24] = color3;
  FastLED.show(); FastLED.delay(_delayDefault);
  
  leds[21] = color1;
  leds[22] = color2;
  leds[23] = color3;
  leds[24] = color4;
  FastLED.show(); FastLED.delay(_delayDefault);
  
  leds[20] = color1;
  leds[21] = color2;
  leds[22] = color3;
  leds[23] = color4;
  leds[24] = color5;
  FastLED.show(); FastLED.delay(_delayDefault);
  
  leds[19] = color1;
  leds[20] = color2;
  leds[21] = color3;
  leds[22] = color4;
  leds[23] = color5;
  leds[24] = color6;
  FastLED.show(); FastLED.delay(_delayDefault);
  
  leds[10] = color1;
  leds[19] = color2;
  leds[20] = color3;
  leds[21] = color4;
  leds[22] = color5;
  leds[23] = color6;
  leds[24] = _black;
  FastLED.show(); FastLED.delay(_delayDefault);
  
  leds[9] = color1;
  leds[10] = color2;
  leds[19] = color3;
  leds[20] = color4;
  leds[21] = color5;
  leds[22] = color6;
  leds[23] = _black;
  FastLED.show(); FastLED.delay(_delayDefault);
  
  leds[0] = color1;
  leds[9] = color2;
  leds[10] = color3;
  leds[19] = color4;
  leds[20] = color5;
  leds[21] = color6;
  leds[22] = _black;
  FastLED.show(); FastLED.delay(_delayDefault);
  
  leds[1] = color1;
  leds[0] = color2;
  leds[9] = color3;
  leds[10] = color4;
  leds[19] = color5;
  leds[20] = color6;
  leds[21] = _black;
  FastLED.show(); FastLED.delay(_delayDefault);
  
  leds[2] = color1;
  leds[1] = color2;
  leds[0] = color3;
  leds[9] = color4;
  leds[10] = color5;
  leds[19] = color6;
  leds[20] = _black;
  FastLED.show(); FastLED.delay(_delayDefault);
  
  leds[3] = color1;
  leds[2] = color2;
  leds[1] = color3;
  leds[0] = color4;
  leds[9] = color5;
  leds[10] = color6;
  leds[19] = _black;
  FastLED.show(); FastLED.delay(_delayDefault);
  
  leds[4] = color1;
  leds[3] = color2;
  leds[2] = color3;
  leds[1] = color4;
  leds[0] = color5;
  leds[9] = color6;
  leds[10] = _black;
  FastLED.show(); FastLED.delay(_delayDefault);
  
  leds[5] = color1;
  leds[4] = color2;
  leds[3] = color3;
  leds[2] = color4;
  leds[1] = color5;
  leds[0] = color6;
  leds[9] = _black;
  FastLED.show(); FastLED.delay(_delayDefault);
 
 leds[14] = color1;
  leds[5] = color2;
  leds[4] = color3;
  leds[3] = color4;
  leds[2] = color5;
  leds[1] = color6;
  leds[0] = _black;
  FastLED.show(); FastLED.delay(_delayDefault);
  
 leds[15] = color1;
 leds[14] = color2;
  leds[5] = color3;
  leds[4] = color4;
  leds[3] = color5;
  leds[2] = color6;
  leds[1] = _black;
  FastLED.show(); FastLED.delay(_delayDefault);
  
 leds[16] = color1;
 leds[15] = color2;
 leds[14] = color3;
  leds[5] = color4;
  leds[4] = color5;
  leds[3] = color6;
  leds[2] = _black;
  FastLED.show(); FastLED.delay(_delayDefault);
  
 leds[17] = color1;
 leds[16] = color2;
 leds[15] = color3;
 leds[14] = color4;
  leds[5] = color5;
  leds[4] = color6;
  leds[3] = _black;
  FastLED.show(); FastLED.delay(_delayDefault);
 
 leds[18] = color1;
 leds[17] = color2;
 leds[16] = color3;
 leds[15] = color4;
 leds[14] = color5;
  leds[5] = color6;
  leds[4] = _black;
  FastLED.show(); FastLED.delay(_delayDefault);
 
 leds[11] = color1;
 leds[18] = color2;
 leds[17] = color3;
 leds[16] = color4;
 leds[15] = color5;
 leds[14] = color6;
  leds[5] = _black;
  FastLED.show(); FastLED.delay(_delayDefault);

 leds[8] = color1;
 leds[11] = color2;
 leds[18] = color3;
 leds[17] = color4;
 leds[16] = color5;
 leds[15] = color6;
 leds[14] = _black;
  FastLED.show(); FastLED.delay(_delayDefault);

 leds[7] = color1;
 leds[8] = color2;
 leds[11] = color3;
 leds[18] = color4;
 leds[17] = color5;
 leds[16] = color6;
 leds[15] = _black;
  FastLED.show(); FastLED.delay(_delayDefault);  
 
 leds[6] = color1;
  leds[7] = color2;
 leds[8] = color3;
 leds[11] = color4;
 leds[18] = color5;
 leds[17] = color6;
 leds[16] = _black;
  FastLED.show(); FastLED.delay(_delayDefault);

 leds[13] = color1;
 leds[6] = color2;
 leds[7] = color3;
 leds[8] = color4;
 leds[11] = color5;
 leds[18] = color6;
 leds[17] = _black;
 FastLED.show(); FastLED.delay(_delayDefault);
 
 leds[12] = color1;
 leds[13] = color2;
 leds[6] = color3;
 leds[7] = color4;
 leds[8] = color5;
 leds[11] = color6;
 leds[18] = _black;
 FastLED.show(); FastLED.delay(_delayDefault);
 
 leds[12] = color2;
 leds[13] = color3;
 leds[6] = color4;
 leds[7] = color5;
 leds[8] = color6;
 leds[11] = _black;
 FastLED.show(); FastLED.delay(_delayDefault);
 
 leds[12] = color3;
 leds[13] = color4;
 leds[6] = color5;
 leds[7] = color6;
 leds[8] = _black;
 FastLED.show(); FastLED.delay(_delayDefault);
 
 leds[12] = color4;
 leds[13] = color5;
 leds[6] = color6;
 leds[7] = _black;
 FastLED.show(); FastLED.delay(_delayDefault);
 
 leds[12] = color5;
 leds[13] = color6;
 leds[6] = _black;
 FastLED.show(); FastLED.delay(_delayDefault);
 
 leds[12] = color6;
 leds[13] = _black;
 FastLED.show(); FastLED.delay(_delayDefault);
 
 leds[12] = _black;
 FastLED.show(); FastLED.delay(_delayDefault);
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
  CRGB rgb;
  
  for(int i=0; i<tempShowCount; i++){
    tmp36Sensor();
  
    String tempFstr = String((int)(tempF + .5f), DEC); //adding .5 then cast to int (which truncates) is same as round
    
    if(tempF < 65.){
      rgb = CHSV(HUE_BLUE, 255, BRIGHTNESS);
    }else if(tempF >= 75.){
      rgb = CHSV(HUE_RED, 255, BRIGHTNESS);
    }else{
      rgb = CHSV(HUE_GREEN, 255, BRIGHTNESS);
    }
    
      for(int i=0; i<tempFstr.length(); i++){
        numberToIntArray(tempFstr[i], rgb);
    }
    
    letterToIntArray('F', CHSV(HUE_YELLOW, 255, BRIGHTNESS));
  }
}

void displayName(){
  CRGB rgb = CRGB(constrain(random8(), MIN_BRIGHTNESS, BRIGHTNESS), constrain(random8(), MIN_BRIGHTNESS, BRIGHTNESS), constrain(random8(), MIN_BRIGHTNESS, BRIGHTNESS));
  
  for(int i=0; i<_nameShowCount; i++){
    
    for(int i=0; i<sizeof(_name)/sizeof(char)-1; i++){
      Serial.print("char of name: "); Serial.println(_name[i]);
      letterToIntArray(_name[i], rgb);
    }
    
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

void numberToIntArray(char number, CRGB rgb){
//  Serial.print("from numberToIntArray: ");
//Serial.print("number: "); Serial.println(myNumber);
  
  int myNumber = number - '0'; //converts single char to int
  
  switch(myNumber){
    case 0:
      showCharacter(numb0,sizeof(numb0)/sizeof(int), rgb);
      break;
    case 1:
      showCharacter(numb1,sizeof(numb1)/sizeof(int), rgb);
      break;
    case 2:
      showCharacter(numb2,sizeof(numb2)/sizeof(int), rgb);
      break;
    case 3:
      showCharacter(numb3,sizeof(numb3)/sizeof(int), rgb);
      break;
    case 4:
      showCharacter(numb4,sizeof(numb4)/sizeof(int), rgb);
      break;
    case 5:
      showCharacter(numb5,sizeof(numb5)/sizeof(int), rgb);
      break;
    case 6:
      showCharacter(numb6,sizeof(numb6)/sizeof(int), rgb);
      break;
    case 7:
      showCharacter(numb7,sizeof(numb7)/sizeof(int), rgb);
      break;
    case 8:
      showCharacter(numb8,sizeof(numb8)/sizeof(int), rgb);
      break;
    case 9:
      showCharacter(numb9,sizeof(numb9)/sizeof(int), rgb);
      break;
    default:
      showCharacter(sad,sizeof(sad)/sizeof(int), rgb);
  }
}

void letterToIntArray(char letter, CRGB rgb){
  
  switch(letter){
    case 'F':
      showCharacter(letterF,sizeof(letterF)/sizeof(int), rgb);
      break;
    case 'A':
      showCharacter(letterA,sizeof(letterA)/sizeof(int), rgb);
      break;
    case 'D':
      showCharacter(letterD,sizeof(letterD)/sizeof(int), rgb);
      break;
    case 'R':
      showCharacter(letterR,sizeof(letterR)/sizeof(int), rgb);
      break;
    case 'I':
      showCharacter(letterI,sizeof(letterI)/sizeof(int), rgb);
      break;
    case 'N':
      showCharacter(letterN,sizeof(letterN)/sizeof(int), rgb);
      break;
    case 'O':
      showCharacter(letterO,sizeof(letterO)/sizeof(int), rgb);
      break;
    default:
      showCharacter(sad,sizeof(sad)/sizeof(int), CRGB::Red);
  }
}

void showCharacter(int number[], int n, CRGB rgb){
  FastLED.clear();
  FastLED.delay(500);
  
  FastLED.setBrightness(BRIGHTNESS);
  
  for(int i=0; i< n; i++){
    leds[number[i]] = rgb;
    FastLED.show();
    FastLED.delay(10);
  }
  
  FastLED.delay(1000);
  FastLED.clear();
}

void updateLedsArray(int numbLeds, int ledsToLight[], CRGB rgbs[], int delayValue){
  for(int i=0; i<numbLeds; i++){
    leds[ledsToLight[i]] = rgbs[i];
  }
  
  FastLED.show();
  FastLED.delay(delayValue);
}




