#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include "DHT.h"

//#define IS_TEST
#define ARRAY_SIZE(A) (sizeof(A) / sizeof((A)[0]))

//DHT11 setup
#define DHTTYPE DHT11
const int _pinDHT = A2;
DHT dht(_pinDHT, DHTTYPE);

//DHT11 variables
unsigned int _statusDht, _statusPreviousDht;
float _hum, _tempC, _tempF, _heatIndexF;
unsigned long _dhtLastUpdate;
const unsigned int _dhtUpdateDelay = 3000;

//LCD variables
LiquidCrystal_I2C lcd(0x3F, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);
const unsigned int _lcdColumns = 16, _lcdRows = 2;
String _blankChar = " ";

//Runtime variables
unsigned long _runTimeLastUpdate;
const unsigned int _runTimeUpdateDelay = 1000;

void setup() {
  delay(250); //short delay
  // put your setup code here, to run once:
  #if defined(IS_TEST)
    pinMode(LED_BUILTIN, OUTPUT);
  #endif

  dht.begin(); //initialize DHT 11

  lcd.begin(_lcdColumns, _lcdRows); //initialize LCD
  lcdIntroduction(); delay(2000); //introduction message for 2 seconds
  lcdMainDisplay(); //main display

  #if defined(IS_TEST)
    Serial.begin(9600);
    Serial.println(F("DHTxx test!"));
  #endif
}

void loop() {  
  // put your main code here, to run repeatedly:
  getDataDHT();
  runTime();
}

void getDataDHT(){  
  if(millis() - _dhtLastUpdate >= _dhtUpdateDelay || _dhtLastUpdate > millis()){
    showDataDHT();
    _dhtLastUpdate = millis();
  }
 }
 
 unsigned int readDHT(){
  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  _hum = dht.readHumidity();
  // Read temperature as Celsius (the default)
  _tempC = dht.readTemperature();
  // Read temperature as Fahrenheit (isFahrenheit = true)
  _tempF = dht.readTemperature(true);
  
  #if defined(IS_TEST)
    Serial.print("Run time:\t"); Serial.print(millis()); Serial.print("\t");
    Serial.print("Temp:\t"); Serial.print(_tempF); Serial.print("\t");
    Serial.print("Hum:\t"); Serial.print(_hum);
  #endif
      
    // Check if any reads failed and exit early (to try again).
  if (isnan(_hum) || isnan(_tempC) || isnan(_tempF)) {
    #if defined(IS_TEST)
      Serial.println(F("Failed to read from DHT sensor!"));
    #else
      lcdUpdate(0, 0, "Failed read DHT!");
    #endif
    
    _statusDht = 0;
    return false;
  }
  
    // Compute heat index in Fahrenheit (the default)
  _heatIndexF = dht.computeHeatIndex(_tempF, _hum);
  // Compute heat index in Celsius (isFahreheit = false)
  //float hic = dht.computeHeatIndex(t, h, false);
  
  #if defined(IS_TEST)
    Serial.print("\tHI:\t"); Serial.println(_heatIndexF);
  #endif
  
  _statusDht = 1;
  return true;
 }

// DISPLAY DATA
 void showDataDHT(){  
  _statusDht = readDHT();
  if(_statusDht == false){ 
    _statusPreviousDht = _statusDht;
    return; 
  }
  
  if(_statusDht == 1 && _statusPreviousDht == 0){ 
    lcdMainDisplay();
  }
  
  _statusPreviousDht = _statusDht;
  
  char humidToPrint[4], tempFToPrint[5], heatIndexToPrint[5];
  dtostrf(_tempF, 3, 0, tempFToPrint);
  dtostrf(_hum, 3, 0, humidToPrint);
  dtostrf(_heatIndexF, 3, 0, heatIndexToPrint);
  
  lcdUpdate(2, 0, tempFToPrint);
  lcdUpdate(11, 0, humidToPrint);
  lcdUpdate(2, 1, heatIndexToPrint);
}

void runTime(){
  if((millis() - _runTimeLastUpdate) > _runTimeUpdateDelay || _runTimeLastUpdate > millis()){
    
    unsigned long runTimeSeconds = 0;
    unsigned int hours, minutes, seconds;
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
    
    String strHours = String(hours, DEC);
    String strMinutes = String(minutes, DEC);
    String strSeconds = String(seconds, DEC);
   
    if(hours<10){ strHours = String("0" + strHours); }
    if(minutes<10){ strMinutes = String("0" + strMinutes); }
    if(seconds<10){ strSeconds = String("0" + strSeconds); }
    
    lcdUpdate(8, 1, strHours + ":" + strMinutes + ":" + strSeconds);
    
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

void lcdMainDisplay() {
  lcd.clear();
  
  lcdUpdate(0, 0, "T");
  lcdUpdate(5, 0, String(char(223)) + String("F"));
  lcdUpdate(10, 0, "H"); lcdUpdate(14, 0, "%");
  lcdUpdate(0, 1, "HI");
}

void lcdIntroduction() {
  lcd.clear();
  
  lcdUpdate(0, 0, "Created VNeyman");
  lcdUpdate(0, 1, "April 2019");
}

String repeatString(String * charToRepeat, int * repeat){
  String stringValue = "";
  unsigned int count = 1;
  
  do{
    stringValue += *charToRepeat;
    count++;
  }while(count < *repeat);
  
  return stringValue;
}
