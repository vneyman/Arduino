//const int _pinRedT = 9, _pinGreenT = 10, _pinBlueT = 11;
//const int _pinRedH = 3, _pinGreenH = 5, _pinBlueH = 6;

int _pinCount;
int _pinTemps[] = {9, 10, 11};
int _pinHumids[] = {3, 5, 6};

int _maxBrightness = 250, _minBrightness = 0;
int _colorChangeIncrement = 5;
int _pinSetToChangeColor;

//uncomment this line if using a Common Anode LED
//#define COMMON_ANODE

void setup()
{
  //Serial.begin(115200);
  delay(1000);
  _pinCount = sizeof(_pinTemps)/sizeof(int);
  
  for(int i=0; i< _pinCount; i++){
    pinMode(_pinTemps[i], OUTPUT);
  }
  
   for(int i=0; i< _pinCount; i++){
    pinMode(_pinHumids[i], OUTPUT);
  }
}

void loop()
{
  _pinSetToChangeColor = random(2);
  
  if(_pinSetToChangeColor == 0){
    setTempColors();
  }else{
    setHumidColors();
  }
  
  delay(100);
}

void setTempColors()
{  
  int i = random(_pinCount);
  static int rgbTempColors[] = {0,0,0};
  
  //Serial.print("random pin "); Serial.println(i); 
  
  if(rgbTempColors[i] >= _maxBrightness/2) {
    for(int j=_maxBrightness; j>_minBrightness; j-=_colorChangeIncrement){
      rgbTempColors[i] = j;
      setColor(&_pinTemps[i], &rgbTempColors[i]);
    }
  }else{
    for(int j=_minBrightness; j<_maxBrightness; j+=_colorChangeIncrement){
      rgbTempColors[i] = j;
      setColor(&_pinTemps[i], &rgbTempColors[i]);
    }
  }
  
//  Serial.print("isUp "); Serial.println(isUp);
//  
//  for(int k=0; k<3;k++){
//    Serial.print(String(k,DEC) + String(" rgb:\t")); Serial.println(rgbColors[k]);
//  }
}

void setHumidColors()
{  
  int i = random(_pinCount);
  static int rgbHumidColors[] = {0,0,0};
  
  //Serial.print("random pin "); Serial.println(i); 
  
  if(rgbHumidColors[i] >= _maxBrightness/2) {
    for(int j=_maxBrightness; j>_minBrightness; j-=_colorChangeIncrement){
      rgbHumidColors[i] = j;
      setColor(&_pinHumids[i], &rgbHumidColors[i]);
    }
  }else{
    for(int j=_minBrightness; j<_maxBrightness; j+=_colorChangeIncrement){
      rgbHumidColors[i] = j;
      setColor(&_pinHumids[i], &rgbHumidColors[i]);
    }
  }
}

void setColor(int * pin, int * colorValue)
{
  analogWrite(*pin, *colorValue);
  delay(100);
}
