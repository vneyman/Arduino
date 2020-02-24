#include <VarSpeedServo.h> //https://github.com/netlabtoolkit/VarSpeedServo

// include the library code:

// Turn on debug statements to the serial output
#define  DEBUG_ENABLE  0

#if  DEBUG_ENABLE
#define DEBUG(s, x) { Serial.print(F(s)); Serial.print(x); }
#define DEBUGS(x) Serial.print(F(x))
#define DEBUGX(x) Serial.println(x, HEX)
#else
#define DEBUG(s, x)
#define DEBUGS(x)
#define DEBUGX(x)
#endif

#define ARRAY_SIZE(A) (sizeof(A) / sizeof((A)[0]))

// create servo object to control a servo 
VarSpeedServo _myServo1;
const int _pinServo1 = 5;
unsigned int _servoAngleMin = 50, _servoAngleMax = 150;
unsigned int _servoSpeed = 100;
const unsigned int _servoSpeedMin = 30, _servoSpeedMax = 255;
unsigned int _servoPosition = (_servoAngleMax + _servoAngleMin) / 2;
unsigned int _servoDelay = 300;
bool _servoState = 1; //depends on the potentiometer value. If less than _potMin then off.

//PIR Sensor
const int _pinPir = 2;
const int _pinPirInterrupt = 0; //Interrupt pin conversion: pin 2 = 0; pin 3 = 1
volatile byte _pirState = LOW;

//Laser settings
const byte _pinLaser = 7;
byte _laserState = 0, _laserAutoPlayState = 0, _laserAutoPlayStatePrevious = 0;
const unsigned int _laserAutoPlayTime = 15000;
unsigned long _laserAutoPlayTimeStart;

//LDR setting
const int _pinLdr = A0;
const unsigned int _ldrMin = 100, _ldrMax = 900;
unsigned int _ldrValue;

//Potentiometer settings
const int _pinPot = A3;
const unsigned int _potMin = 95, _potMax = 900;
unsigned int _potValue;

//RGB LEDs
int _pinLedRgb[] = {11, 6, 3};      // the number of the LED pin
unsigned int _ledRgbPinCount = ARRAY_SIZE(_pinLedRgb);
const int _ledRgbBrightnessMin = 20, _ledRgbBrightnessMax = 220; //max brightness of each RGB color
unsigned int _ledRgbBrightness = _ledRgbBrightnessMax;
//const unsigned int _ledRgbIndexCountChange = 10;

//Runtime variables
unsigned long _runTimeLastUpdate;
const unsigned int _runTimeUpdateDelay = 1000;

// the following variables are long's because the time, measured in miliseconds,
// will quickly become a bigger number than can be stored in an int.

void setup() {
  #if DEBUG_ENABLE
    Serial.begin(115200);
    DEBUGS("\n[Cat Laser Toy test]");
  #endif

  pinMode(_pinLdr, INPUT); //analog
  pinMode(_pinPot, INPUT); //analog

  pinMode(_pinPir, INPUT); //digital
  pinMode(_pinLaser, OUTPUT); //digital
  
  for(int i=0; i< _ledRgbPinCount; i++){
    pinMode(_pinLedRgb[i], OUTPUT);
  }

  //Servo setup
  _myServo1.attach(_pinServo1); //Attached the servo to correct pin
  _myServo1.write(_servoPosition, _servoSpeed, true); //Set servo position to initial position
  
  #if DEBUG_ENABLE
    //analogWrite(3, 200); delay(2000);
    
    int rgb[] = {_ledRgbBrightness, 0, 0}; //red
    
    setLedRgbColors(rgb); delay(500);
    rgb[0] = 0; rgb[1] = _ledRgbBrightness; //green
    setLedRgbColors(rgb); delay(1500);
    rgb[1] = 0; rgb[2] = _ledRgbBrightness; //blue
    setLedRgbColors(rgb); delay(500);
  #endif

  // set initial LED state
  setLedRgbOff();

  attachInterrupt(_pinPirInterrupt, readPirSensor, CHANGE);
}

void loop() {
  if ((millis() - _runTimeLastUpdate) > _runTimeUpdateDelay || _runTimeLastUpdate > millis()) {
    readLdr(); setLedRgbRandom();
    readPot(); 
    
    _runTimeLastUpdate = millis();
  }
  
  autoPlay();
}

void readPirSensor(){
  _pirState = digitalRead(_pinPir);
  }

void setServoPosition(int servoPosition){
  #if DEBUG_ENABLE
    DEBUGS("\nSet Servo Position");
    //DEBUG("\tON or OFF: ", _servoState);
  #endif

  if(_servoState){
      servoPosition = constrain(servoPosition, _servoAngleMin, _servoAngleMax);
      _myServo1.write(servoPosition, _servoSpeed, true);
    }
  }

void autoPlay(){
  #if DEBUG_ENABLE
    //DEBUGS("\nAuto Play");
  #endif

  if(_pirState){
      _laserAutoPlayState = 1;
      _laserAutoPlayTimeStart = millis();
      delay(10);
    }

  if(_laserAutoPlayState == 1 && (millis() - _laserAutoPlayTimeStart) < _laserAutoPlayTime){
        _laserState = random(0,3);
        if(_laserState == 0) { digitalWrite(_pinLaser, LOW); } else { digitalWrite(_pinLaser, HIGH); }
        
        setServoPosition(random(_servoAngleMin, _servoAngleMax));
        delay(random(_servoDelay, _servoDelay*5));
    }else{
        _laserAutoPlayState = 0;
        if(_laserAutoPlayStatePrevious) { autoPlayReset(); }
      }

    _laserAutoPlayStatePrevious = _laserAutoPlayState;
  }

void autoPlayReset(){
  #if DEBUG_ENABLE
    DEBUGS("\n***Auto Play Reset***");
  #endif
  
  _laserState = LOW;
  digitalWrite(_pinLaser, _laserState);
  _servoPosition = (_servoAngleMax + _servoAngleMin) / 2;
  setServoPosition(_servoPosition);
  }

void readLdr(){
  #if DEBUG_ENABLE
    DEBUGS("\nReading LDR");
  #endif

  _ldrValue = analogRead(_pinLdr);
  
  setLedRgbBrightnessMax();
  }

void readPot(){
  #if DEBUG_ENABLE
    DEBUGS("\nReading Potentiometer");
  #endif

  _potValue = analogRead(_pinPot);
  
  #if DEBUG_ENABLE
    DEBUG("\nPot: ", _potValue);
    //delay(200);
  #endif

    setServoSpeed();
  }

void setServoSpeed(){
  if(_potValue < _potMin) {
      _servoState = 0; _servoSpeed = 1; return;
    }

  _servoState = 1;
  _potValue = constrain(_potValue, _potMin, _potMax);
  _servoSpeed = map(_potValue, _potMin, _potMax, _servoSpeedMin, _servoSpeedMax);

    #if DEBUG_ENABLE
    DEBUG("\nServo Speed: ", _servoSpeed);
    //delay(200);
  #endif
  }

void setLedRgbBrightnessMax(){
  _ldrValue = constrain(_ldrValue, _ldrMin, _ldrMax);
  _ledRgbBrightness = map(_ldrValue, _ldrMin, _ldrMax, _ledRgbBrightnessMin, _ledRgbBrightnessMax);
  }


void setLedRgbRandom()
{
  #if DEBUG_ENABLE
      DEBUG("\n Random RGB; max brightness: ", _ledRgbBrightness);
      DEBUG("\n RGB pin count: ", _ledRgbPinCount);
  #endif

  static int rgbChangeCount = 1;
  static int rgbIndex = 0;
  
  static int rgb[] = {random((int)(_ledRgbBrightness / 2), (int)_ledRgbBrightness), 
                  random((int)(_ledRgbBrightness / 2), (int)_ledRgbBrightness), 
                  random((int)(_ledRgbBrightness / 2), (int)_ledRgbBrightness) };

 rgbIndex = random(0, _ledRgbPinCount);
 rgb[rgbIndex] = random((int)(_ledRgbBrightness / 3), _ledRgbBrightness);

 setLedRgbColors(rgb);
}

void setLedRgbColors(int colorValues[])
{   
  for(int i=0; i<_ledRgbPinCount; i++){ 
    DEBUG("\n RGB: ", _pinLedRgb[i]); DEBUG(" : ", colorValues[i]);
    setLedRgbColor(&_pinLedRgb[i], &colorValues[i]);
  }
}

void setLedRgbColor(int * pin, int * colorValue)
{
  analogWrite(*pin, *colorValue);
  delay(2);
}

void setLedRgbOff()
{
  int rgbOff[] = {0, 0, 0};

  setLedRgbColors(rgbOff);
}
