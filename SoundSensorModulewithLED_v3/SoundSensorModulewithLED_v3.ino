const int _pinSoundAnalog = A0, _pinSoundDigital = 2;

//int valueSound1, valueSound2;
//int stateLED;

//Digital pins
int _pinDigitalLeds[] = {4,7,8};
int _pinDigitalLedsCount;
int _ledDelay = 500, _ledDelayNone=0;

//Baseline sound level
int _soundBaseline, _soundValueAnalog;
unsigned long _soundBaselineLastUpdate;
const unsigned long _soundBaselineDelay = 10000;
const unsigned int _soundBaseLineObservations = 10;

void setup() {
  delay(500);
  // put your setup code here, to run once:
  Serial.begin(9600);
  
  pinMode(_pinSoundAnalog, INPUT);
  pinMode(_pinSoundDigital, INPUT);
  
  _pinDigitalLedsCount = sizeof(_pinDigitalLeds)/sizeof(int);
  
  for(int i=0; i< _pinDigitalLedsCount; i++){
    pinMode(_pinDigitalLeds[i], OUTPUT);
  }
  
  setBaseline();
  
  //Serial.print("Baseline sound: "); Serial.println(_soundBaseline);
}

void loop() {
  // put your main code here, to run repeatedly:
  detectSound();
}

void detectSound(){
  int ledValues[] = {0, 0, 0};
  
  if(digitalRead(_pinSoundDigital) == HIGH){
    _soundValueAnalog=analogRead(_pinSoundAnalog);
    Serial.print("analog Sound: "); Serial.println(_soundValueAnalog);
    ledValues[0] = HIGH;
    
    if(_soundValueAnalog > 530){
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
  }
}

void digitalPinsHighLow(int pins[], int values[], int *delayValue){
  for(int i=0; i< _pinDigitalLedsCount; i++){
    digitalWrite(pins[i], values[i]);
    //delay(1);
  }
  
  delay(*delayValue);
}

void setBaseline(){
  float soundBaseline = 0., observations = _soundBaseLineObservations;
  
    for(int i=0; i< _soundBaseLineObservations; i++){
      soundBaseline += analogRead(_pinSoundAnalog);
      delay(100);
      //Serial.print(i); Serial.print(" Baseline sound: "); Serial.println(valueSound1);
  }
  
  _soundBaseline = soundBaseline/observations + .5; //add .5 to round to closes integer
  //Serial.print("Baseline sound: "); Serial.println(_soundBaseline);
  delay(1000);
}

