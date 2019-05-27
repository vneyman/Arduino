//Temperature TMP 36
const int pinTmp=A0;
const unsigned int tmpUpdateDelay = 1000;
unsigned long tmpLastUpdate;
float tempC, tempF, tempCadjust = -1.0;

void setup(){
  Serial.begin(9600);
  Serial.println("Fenrenheit \tCelcius");
  
  pinMode(pinTmp, INPUT);
}

void loop(){
  
  if(millis() - tmpLastUpdate > tmpUpdateDelay || tmpLastUpdate > millis()){
    tmp36Sensor();
    
    Serial.print(tempF);
    Serial.print("\t\t");
    Serial.println(tempC);
    
    tmpLastUpdate = millis();
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
