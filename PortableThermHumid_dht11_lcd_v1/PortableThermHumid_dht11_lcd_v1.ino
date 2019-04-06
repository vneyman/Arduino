#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <dhtnew.h>

#define IS_TEST
#define ARRAY_SIZE(A) (sizeof(A) / sizeof((A)[0]))

//DHT11 setup
const int _pinDHT = 12;
DHTNEW dht(_pinDHT);

//DHT11 variables
int _readDHT, _statusDHT;
float _hum, _tempC, _tempF;
unsigned long _dhtLastUpdate;
const unsigned int _dhtUpdateDelay = 3000;

//LCD variables
LiquidCrystal_I2C lcd(0x3F, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);
const int _lcdColumns = 16, _lcdRows = 2;
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

  lcd.begin(_lcdColumns, _lcdRows);
  lcd.clear();
  lcdUpdate(0, 0, "Humidity %");
  lcdUpdate(12, 0, String("Temp ") + String(char(223)) + String("F"));
  lcdUpdate(0, 3, "Run time: ");
  lcdUpdate(0, 2, "--------------------");

  #if defined(IS_TEST)
    Serial.begin(115200);
    Serial.println("DHT TEST PROGRAM ");
    Serial.print("LIBRARY VERSION: ");
    Serial.println(DHTNEW_LIB_VERSION);
  #endif
}

void loop() {  
  // put your main code here, to run repeatedly:
  getDataDHT();
  runTime();
}

void getDataDHT(){
  static int _readDHTprevious = -3;
  String lcdToPrint = "";
  
  if(millis() - dht.lastRead() >= _dhtUpdateDelay || dht.lastRead() > millis()){
    _readDHT = dht.read();
  
    switch(_readDHT){
      case DHTLIB_OK: //no errors, return 0
        showDataDHT();
        if(_readDHTprevious != _readDHT) {lcdUpdate(0, 2, "--------------------");}
        break;
      case DHTLIB_ERROR_CHECKSUM: //Checksum error, return -1
        lcdUpdate(0, 1, "**Checksum error**");
        break;
      case DHTLIB_ERROR_TIMEOUT: //Time out error, return -2
        lcdUpdate(0, 1, "**Time out error**");
        break;
      default: 
        lcdUpdate(0, 1, "**Unknown out error**");
        break;      
      }

      _readDHTprevious = _readDHT;
  }
 }

// DISPLAY DATA
 void showDataDHT(){
  _hum = dht.humidity;
  _tempC = dht.temperature;
  _tempF = _tempC * (9./5.) + 32.;

  char humidToPrint[4], tempToPrint[5];
  dtostrf(_hum, 3, 0, humidToPrint);
  dtostrf(_tempF, 3, 1, tempToPrint);

  #if defined(IS_TEST)
    Serial.print("Run time:\t"); Serial.print(millis());
    Serial.print("Temp:\t"); Serial.print(_tempF);
    Serial.print("Hum:\t"); Serial.print(_hum);
    Serial.println("");
  #else
    lcdUpdate(2,1,humidToPrint);
    lcdUpdate(14,1,tempToPrint);    
  #endif

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
    
    lcdUpdate(6, 1, strHours + ":" + strMinutes + ":" + strSeconds);
    
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
  }while(count < *repeat);
  
  return stringValue;
}
