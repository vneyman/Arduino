#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <dhtnew.h>

//DHT22 variables
const int pinDHT = 12;
int readDHT, statusDHT;
float hum, tempC, tempF;
unsigned long _dhtLastUpdate;
const unsigned int _dhtUpdateDelay = 3000;
DHTNEW dht(pinDHT);

//LCD variables
LiquidCrystal_I2C lcd(0x3F, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);
const int lcdColumns = 20, lcdRows = 4;
String _blankChar = " ";

//Runtime variables
unsigned long _runTimeLastUpdate;
const unsigned int _runTimeUpdateDelay = 1000;

void setup() {
  // put your setup code here, to run once:
  lcd.begin(lcdColumns, lcdRows);
  lcd.clear();
  lcdUpdate(0, 0, "Humidity %");
  lcdUpdate(12, 0, String("Temp ") + String(char(223)) + String("F"));
  lcdUpdate(0, 3, "Run time: ");
  lcdUpdate(0, 2, "--------------------");

//  Serial.begin(9600);
//  Serial.println("DHT TEST PROGRAM ");
//  Serial.print("LIBRARY VERSION: ");
//  Serial.println(DHTNEW_LIB_VERSION);
}

void loop() {  
  // put your main code here, to run repeatedly:
  getDataDHT();
  runTime();
}

void getDataDHT(){
  static int readDHTprevious = -3;
  String lcdToPrint = "";
  
  if(millis() - dht.lastRead() >= _dhtUpdateDelay || dht.lastRead() > millis()){
    readDHT = dht.read();
  
    switch(readDHT){
      case DHTLIB_OK: //no errors, return 0
        showDataDHT();
        if(readDHTprevious != readDHT) {lcdUpdate(0, 2, "--------------------");}
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

      readDHTprevious = readDHT;
  }
 }

// DISPLAY DATA
 void showDataDHT(){
  hum = dht.humidity;
  tempC = dht.temperature;
  tempF = tempC * (9./5.) + 32.;

  char humidToPrint[4], tempToPrint[5];
  dtostrf(hum,3,0,humidToPrint);
  dtostrf(tempF,3,1,tempToPrint);
  
  lcdUpdate(2,1,humidToPrint);
  lcdUpdate(14,1,tempToPrint);
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
