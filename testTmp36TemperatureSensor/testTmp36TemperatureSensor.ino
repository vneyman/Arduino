//Temperature TMP 36
const int pinTmp=A2;
const unsigned int tmpUpdateDelay = 1000;
unsigned long tmpLastUpdate;
float tempC, tempF, tempCadjust = 0.0;

void setup(){
  Serial.begin(115200);
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
  int tmp36Reading = analogRead(pinTmp);
  float tmp36Voltage = tmp36Reading * 5.0;
  tmp36Voltage /= 1024.0;
  
  tempC = (tmp36Voltage - .5)*100.0 + tempCadjust;
  tempF = (tempC * 9.0/5.0) + 32.;
  
  Serial.print("Temp F: "); Serial.print(tempF);
  Serial.print(F("\t tmp36Reading: ")); Serial.println(tmp36Reading);
}
