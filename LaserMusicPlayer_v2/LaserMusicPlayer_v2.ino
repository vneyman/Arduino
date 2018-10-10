const int _pinLed1=9, _pinLed2=10;

//LDR setup: one end to positive (5V), other end to analog pin. Also, 10k+ resistor is required to be connected to same analog pin and ground 
const int _pinLdr1 = A0, _pinLdr2=A1;
int _ldrValue;
int _ldrThreshold = 200; //test for LDR and resistor combination. May need to set at setup

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(_pinLed1, OUTPUT);
  pinMode(_pinLdr1, INPUT);

  pinMode(_pinLed2, OUTPUT);
  pinMode(_pinLdr2, INPUT);

  _ldrValue = (analogRead(_pinLdr1) + analogRead(_pinLdr2) )/2; //average LDR value.

  if(_ldrValue > _ldrThreshold){
    _ldrThreshold = _ldrValue - 100;
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  _ldrValue = GetLdrValue(&_pinLdr1);
  TriggerLed(&_ldrValue, &_pinLed1);

  _ldrValue = GetLdrValue(&_pinLdr2);
  TriggerLed(&_ldrValue, &_pinLed2);
  
  delay(100);
}

int GetLdrValue(int *pinLdr){
  int ldrValue = analogRead(*pinLdr);
  Serial.print(*pinLdr); Serial.print(" pin LDR value:\t"); Serial.println(ldrValue);
  return ldrValue;
}

void TriggerLed(int * ldrValue, int * pinLed){
  int ledStatus;
  
  if(*ldrValue > _ldrThreshold){
    ledStatus = 1;
    }else{
      ledStatus = 0;
      }

  digitalWrite(*pinLed, ledStatus);
}
