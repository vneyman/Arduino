#define IS_TEST
#define ARRAY_SIZE(A) (sizeof(A) / sizeof((A)[0]))

//Temperature TMP 36
const int _pinTmp=A4, _pinHigh = A3, _pinLow = A5;
const unsigned int tmpUpdateDelay = 1000;
unsigned long tmpLastUpdate;
float _tempC, _tempF, _tempCadjust = 0.0;
const int tempShowCount = 3;

//Temperature Settings
const float _tempPerfect = 70., _tempPerfectRange = 2.0, _tempMax = 90., _tempMin = 40.;

//RGB Variables
int _pinRgbLed[] = {3, 5, 6};
const int _pinRgbLedLow = 4; //ground PIN
int _pinRgbLedCount = ARRAY_SIZE(_pinRgbLed);
const float _maxBrightness = 255.;

void setup(){
  #if defined(IS_TEST)
    Serial.begin(115200);
    Serial.println("Fenrenheit \tCelcius");
  #endif

  pinMode(_pinHigh, OUTPUT);
  pinMode(_pinLow, OUTPUT);
  pinMode(_pinTmp, INPUT);
  pinMode(_pinRgbLedLow, OUTPUT);

  for(int i=0; i< _pinRgbLedCount; i++){
    pinMode(_pinRgbLed[i], OUTPUT);
  }

  //Set High and Low for the TMP36 sensor
  digitalWrite(_pinHigh, HIGH);
  digitalWrite(_pinLow, LOW);
  
  //Set ground for the RGB led
  digitalWrite(_pinRgbLedLow, LOW);
}

void loop(){
  static int tempFold = 999;
  
  if(millis() - tmpLastUpdate > tmpUpdateDelay || tmpLastUpdate > millis()){
    if(_tempF != tempFold){      
      tmp36Sensor();
      showTmpRgbLed();

      tempFold = (int)_tempF;
    }

    #if defined(IS_TEST)
      Serial.print(_tempF);
      Serial.print("\t\t");
      Serial.println(_tempC);
    #endif
        
    tmpLastUpdate = millis();
  }
}

void showTmpRgbLed(){
  static int rgbTempColors[] = {0,0,0};

  float diff = _tempF - _tempPerfect;
  float adjust;

  if(diff > _tempPerfectRange){
    adjust = (diff/(_tempMax - _tempPerfect)) * _maxBrightness;
    adjust = constrain(adjust, 0, _maxBrightness);
    
    rgbTempColors[0] = adjust;
    rgbTempColors[1] = _maxBrightness - adjust; //need to add to this
    rgbTempColors[2] = 0;
  }else if(diff < -1.*_tempPerfectRange){
    adjust = (diff/(_tempMin - _tempPerfect)) * _maxBrightness;
    adjust = constrain(adjust, 0, _maxBrightness);
    
    rgbTempColors[0] = 0;
    rgbTempColors[1] = _maxBrightness - adjust; //need to add to this
    rgbTempColors[2] = adjust;
  }else{
    rgbTempColors[0] = 0;
    rgbTempColors[1] = _maxBrightness; //need to add to this
    rgbTempColors[2] = 0;
  }

  for(int i=0; i<_pinRgbLedCount; i++){
    #if defined(IS_TEST)
      Serial.print(millis()); Serial.print("\t");
      Serial.print(String(_pinRgbLed[i]) + String(" pin color:\t")); Serial.println(rgbTempColors[i]);
    #endif
    
    setRgbColor(&_pinRgbLed[i], &rgbTempColors[i]);
  }
}

void setRgbColor(int * pin, int * colorValue)
{
  analogWrite(*pin, *colorValue);
  delay(2);
}


void tmp36Sensor(){
  int reading = analogRead(_pinTmp);
  float tmp36Voltage = reading * 5.0;
  tmp36Voltage /= 1024.0;
  
  _tempC = (tmp36Voltage - .5)*100.0 + _tempCadjust;
  _tempF = (_tempC * 9.0/5.0) + 32.;
  
  //Serial.print("Temp F: "); Serial.println(tempF);
}
