#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <dhtnew.h>

//DHT22 variables
const int pinDHT = 11;
int readDHT, statusDHT;
float hum, tempC, tempF;
DHTNEW dht(pinDHT);

//LCD variables
LiquidCrystal_I2C lcd(0x3F, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);
const int lcdColumns = 20, lcdRows = 4;

void setup() {
  // put your setup code here, to run once:
  lcd.begin(lcdColumns, lcdRows);
  lcd.clear();
  lcdUpdate(0, 0, "Humidity %");
  lcdUpdate(12, 0, "Temp (F)");
  lcdUpdate(0, 3, "Run time: ");
  lcdUpdate(0, 2, "--------------------");

  
  Serial.begin(115200);
  Serial.println("DHT TEST PROGRAM ");
  Serial.print("LIBRARY VERSION: ");
  Serial.println(DHTNEW_LIB_VERSION);
 
}

void loop() {
  // put your main code here, to run repeatedly:
  getDataDHT();

  //lcdUpdate(11, 3, (String)(millis()/1000) + " s");
  delay(1000);
}

void getDataDHT(){
  static int readDHTprevious = -3;
  
  if(millis() - dht.lastRead() >= 2000){
    readDHT = dht.read();    
  
    switch(readDHT){
      case DHTLIB_OK: //no errors, return 0
        showDataDHT();
        //if(readDHTprevious != readDHT) {lcdUpdate(0, 2, "--------------------");}
        break;
      case DHTLIB_ERROR_CHECKSUM: //Checksum error, return -1
        //lcdUpdate(0, 2, "**Checksum error**");
        break;
      case DHTLIB_ERROR_TIMEOUT: //Time out error, return -2
        //lcdUpdate(0, 2, "**Time out error**");
        break;
      default: 
        //lcdUpdate(0, 2, "**Unknown out error**");
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

  char humBuffer[3], tempBuffer[5];
  dtostrf(hum,3,0,humBuffer);
  dtostrf(tempF,3,1,tempBuffer);
  
  lcdUpdate(2,1,humBuffer);
  lcdUpdate(14,1,tempBuffer);  
}

void lcdUpdate(uint8_t charStart, uint8_t line, String value){
  int valueLength = value.length();
  String blank = repeatString(" ", valueLength);
  
  lcd.setCursor(charStart, line);
  lcd.print(blank);
  lcd.setCursor(charStart, line);
  lcd.print(value);
}

String repeatString(String charToRepeat, int repeat){
  String stringValue = "";
  int count = 1;
  
  do{
    stringValue += charToRepeat;
    count++;
  }while(count<repeat);
  
  return stringValue;
}
