#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <dhtnew.h>

//DHT22 variables
const int _pinDHT = 12;
DHTNEW dht(_pinDHT);

int readDHT, statusDHT;
float _humidity, _tempC, _tempF;
unsigned long _dhtLastUpdate;
const unsigned int _dhtUpdateDelay = 3000;
const float _tempPerfect = 72., _tempMax = 85., _tempMin = 60.;
const float _humidPerfect = 60., _humidMax = 80., _humidMin = 40.;

//LCD variables
LiquidCrystal_I2C lcd(0x3F, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);
const int _lcdColumns = 20, _lcdRows = 4;
String _blankChar = " ";

//Runtime variables
unsigned long _runTimeLastUpdate;
const unsigned int _runTimeUpdateDelay = 1000;

//RGB Variables
int _pinCount;
int _pinTemps[] = {11, 10, 9};
int _pinHumids[] = {6, 5, 3};
const float _maxBrightness = 255;

//Sound Sensor module
const byte _pinSoundAnalog = A0, _pinSoundDigital = 2, _pinSoundDigitalInterrupt = 0;//digital is an interrupt PIN 2
int _soundValueAnalog;
volatile int _soundValueDigital;

//Sound sensor LEDs
int _pinDigitalLeds[] = {4,7,8};
int _pinDigitalLedsCount;
int _ledDelay = 500, _ledDelayNone=0;

void setup() {
  delay(500);
  Serial.begin(115200);
  // put your setup code here, to run once:
  lcd.begin(_lcdColumns, _lcdRows);
  lcd.clear();
  lcdUpdate(0, 0, "Humidity %");
  lcdUpdate(12, 0, String("Temp ") + String(char(223)) + String("F"));
  lcdUpdate(0, 3, "Run time: ");
  lcdUpdate(0, 2, "--------------------");

//Set PIN modes of the PWM pins
  _pinCount = sizeof(_pinTemps)/sizeof(int);

  for(int i=0; i< _pinCount; i++){
    pinMode(_pinTemps[i], OUTPUT);
  }
  
   for(int i=0; i< _pinCount; i++){
    pinMode(_pinHumids[i], OUTPUT);
  }
  
  //Sound sensor input pins
  pinMode(_pinSoundAnalog, INPUT);
  pinMode(_pinSoundDigital, INPUT);
  
  //Sound sensor output pins
  _pinDigitalLedsCount = sizeof(_pinDigitalLeds)/sizeof(int);
  
  for(int i=0; i< _pinDigitalLedsCount; i++){
    pinMode(_pinDigitalLeds[i], OUTPUT);
  }
  
  attachInterrupt(_pinSoundDigitalInterrupt, detectSound, CHANGE);
}

void loop() {  
  // put your main code here, to run repeatedly:
  getDataDHT();
  onSoundDetected();
  runTime();
}

void detectSound(){
  _soundValueDigital = digitalRead(_pinSoundDigital);
}

void onSoundDetected(){
  int ledValues[] = {0, 0, 0};
  
  if(_soundValueDigital == HIGH){
    _soundValueAnalog=analogRead(_pinSoundAnalog);
    //Serial.print("analog Sound: "); Serial.println(_soundValueAnalog);
    ledValues[0] = HIGH;
    
    if(_soundValueAnalog > 500){
      ledValues[1] = HIGH;
      _ledDelay = 1000;
    }
    
    if(_soundValueAnalog > 550){
      ledValues[2] = HIGH;
      _ledDelay = 2000;
    }
    
    digitalPinsHighLow(_pinDigitalLeds, ledValues, &_ledDelay);
  }else{
     
    ledValues[0] = LOW;
    ledValues[1] = LOW;
    ledValues[2] = LOW;
    
    digitalPinsHighLow(_pinDigitalLeds, ledValues, &_ledDelayNone);
    _soundValueDigital = LOW;
  }
}


void getDataDHT(){
  static int readDHTprevious = -3, isError;
  String lcdToPrint = "";
  
  if(millis() - dht.lastRead() >= _dhtUpdateDelay || dht.lastRead() > millis()){
    readDHT = dht.read();
  
    switch(readDHT){
      case DHTLIB_OK: //no errors, return 0
        if(isError == 1) { 
          lcdUpdate(0, 1, "                    ");
          isError = 0;
        }
        showDataDHT();
        if(readDHTprevious != readDHT) {lcdUpdate(0, 2, "--------------------");}
        break;
      case DHTLIB_ERROR_CHECKSUM: //Checksum error, return -1
        lcdUpdate(0, 1, "**Checksum error**");
        isError = 1;
        break;
      case DHTLIB_ERROR_TIMEOUT: //Time out error, return -2
        lcdUpdate(0, 1, "**Time out error**");
        isError = 1;
        break;
      default: 
        lcdUpdate(0, 1, "**Unknown out error**");
        isError = 1;
        break;      
      }  

      readDHTprevious = readDHT;
  }
 }

// DISPLAY DATA
 void showDataDHT(){
  _humidity = dht.humidity;
  _tempC = dht.temperature;
  _tempF = _tempC * (9./5.) + 32.;

  setTempColors();
  setHumidityColors();
  
  char humidToPrint[4], tempToPrint[5];
  dtostrf(_humidity,3,0,humidToPrint);
  dtostrf(_tempF,3,1,tempToPrint);
  
  lcdUpdate(2,1,humidToPrint);
  lcdUpdate(14,1,tempToPrint);
}

void setHumidityColors()
{  
  static int rgbHumidColors[] = {0,(int)_maxBrightness,0};
  
  float diff = _humidity - _humidPerfect;
  float adjust;
  
  //Serial.print("tempDiff "); Serial.println(tempDiff);
  
  if(diff > 1.){
    adjust = (diff/(_humidMax - _humidPerfect)) * _maxBrightness;
    //Serial.println(String("Humid adjust:\t") + String((int)adjust,DEC));
    rgbHumidColors[2] = adjust; //blue
    rgbHumidColors[1] = 255. - adjust; //green
    rgbHumidColors[0] = 0.;
  }else if(diff < -1.){
    adjust = diff/(_humidMin - _humidPerfect)*_maxBrightness;

    rgbHumidColors[0] = adjust; //red
    rgbHumidColors[1] = 255. - adjust; //green
    rgbHumidColors[2] = 0.;
  }else{
    rgbHumidColors[0] = 0; //red
    rgbHumidColors[1] = (int)_maxBrightness; //green
    rgbHumidColors[2] = 0; //blue
  }
  
  for(int i=0; i<_pinCount; i++){
    //Serial.print(millis()); Serial.print("\t");
    //Serial.print(String(i) + String("color:\t")); Serial.println(rgbTempColors[i]);
    
    setColor(&_pinHumids[i], &rgbHumidColors[i]);
  }
}

void setTempColors()
{  
  static int rgbTempColors[] = {0,(int)_maxBrightness,0};
  
  float diff = _tempF - _tempPerfect;
  float adjust;
  
  //Serial.print("temp diff "); Serial.println(diff);
  
  if(diff > .5){
    adjust = (diff/(_tempMax - _tempPerfect)) * _maxBrightness;
    //Serial.println(String("temp adjust:\t") + String((int)adjust,DEC));
    
    rgbTempColors[0] = adjust; //red
    rgbTempColors[1] = 255. - adjust; //green
    rgbTempColors[2] = 0.; //blue
  }else if(diff < -.5){
    adjust = diff/(_tempMin - _tempPerfect)*_maxBrightness;
    rgbTempColors[2] = adjust; //blue
    rgbTempColors[1] = 255. - adjust; //green
    rgbTempColors[0] = 0.; //red
  }else{
    rgbTempColors[0] = 0.; //red
    rgbTempColors[1] = (int)_maxBrightness; //green
    rgbTempColors[2] = 0.; //blue
  }
  
  for(int i=0; i<_pinCount; i++){
    //Serial.print(millis()); Serial.print("\t");
    //Serial.print(String(_pinTemps[i]) + String(" pin color:\t")); Serial.println(rgbTempColors[i]);
    
    setColor(&_pinTemps[i], &rgbTempColors[i]);
  }
  
}

void runTime(){
  if((millis() - _runTimeLastUpdate) > _runTimeUpdateDelay){
    
    unsigned long runTimeSeconds = 0;
    int hours, minutes, seconds;
    runTimeSeconds = millis()/1000;
    
    if(runTimeSeconds >= 3600){
      hours = (int)(runTimeSeconds/3600);
      seconds = runTimeSeconds - (int)(hours*3600); //remaining seconds
      minutes = (int)(seconds/60);
      seconds = seconds - (int)(minutes*60);
    }else if(runTimeSeconds >= 60){
      hours = 0;
      seconds = runTimeSeconds - (int)(hours*3600);
      minutes = (int)(seconds/60);
      seconds = seconds - minutes*60;
    }else{
      hours = 0;
      seconds = runTimeSeconds - (int)(hours*3600);
      minutes = 0;
      seconds = seconds - minutes*60;
    }
    
    String strHours = String(hours,DEC);
    String strMinutes = String(minutes,DEC);
    String strSeconds = String(seconds,DEC);
   
    if(hours<10){ strHours = String("0" + strHours); }
    if(minutes<10){ strMinutes = String("0" + strMinutes); }
    if(seconds<10){ strSeconds = String("0" + strSeconds); }
    
    lcdUpdate(11,3,strHours + ":" + strMinutes + ":" + strSeconds);
    
    _runTimeLastUpdate = millis();
  }
}

void lcdUpdate(uint8_t charStart, uint8_t line, String value){
  int valueLength = value.length();
  String blank = repeatString(&_blankChar, &valueLength);
  
  lcd.setCursor(charStart, line);
  lcd.print(blank);
  lcd.setCursor(charStart, line);
  lcd.print(value);
}

String repeatString(String * charToRepeat, int * repeat){
  String stringValue = "";
  int count = 1;
  
  do{
    stringValue += *charToRepeat;
    count++;
  }while(count<*repeat);
  
  return stringValue;
}

void setColor(int * pin, int * colorValue)
{
  analogWrite(*pin, *colorValue);
  delay(2);
}

void digitalPinsHighLow(int pins[], int values[], int *delayValue){
  for(int i=0; i< _pinDigitalLedsCount; i++){
    digitalWrite(pins[i], values[i]);
  }
  
  delay(*delayValue);
}
