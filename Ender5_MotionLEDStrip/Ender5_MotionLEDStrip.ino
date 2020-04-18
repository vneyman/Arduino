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

//PIR Sensor
const int _pinPir = 3;
const int _pinPirInterrupt = 1; //Interrupt pin conversion: pin 2 = 0; pin 3 = 1
volatile byte _pirState = HIGH;

//Strip LEDs
const int _pinLedStrip = 6;      // the number of the LED pin
const unsigned int _ledStripOnTime = 60000; //1 second: 1000 seconds; 60 seconds: 60*1000
unsigned long _ledStripLastTimeStart;
bool _ledStripState, _ledStripStatePrevious;

const int _ledStripBrightnessMin = 20, _ledStripBrightnessMax = 220; //max brightness of each Strip color
unsigned int _ledStripBrightness = _ledStripBrightnessMax;

// the following variables are long's because the time, measured in miliseconds,
// will quickly become a bigger number than can be stored in an int.

void setup() {
  #if DEBUG_ENABLE
    Serial.begin(115200);
    DEBUGS("\n[Ender 5 Led Strip]");
  #endif

  pinMode(_pinPir, INPUT); //digital
  pinMode(_pinLedStrip, OUTPUT); //digital

  #if DEBUG_ENABLE    
    setLedStripAnalog(150); delay(500); //on
    setLedStripAnalog(0); delay(500); //off
  #endif

  // set initial LED state
  setLedStrip(0);

  attachInterrupt(_pinPirInterrupt, readPirSensor, CHANGE);
}

void loop() { 
  enableDisableLedStrip();
}
  
void readPirSensor(){
  _pirState = digitalRead(_pinPir);
  }

void enableDisableLedStrip(){
  #if DEBUG_ENABLE
    DEBUGS("\nAuto Play");
  #endif

  #if DEBUG_ENABLE
    DEBUG("\n_pirState: ", _pirState);
  #endif

  if(_pirState){
      _ledStripState = 1;
      _ledStripLastTimeStart = millis();
      delay(1);
    }

  if(_ledStripState == 1 && (millis() - _ledStripLastTimeStart) < _ledStripOnTime){
        if(!_ledStripStatePrevious) { 
          setLedStrip(1); //turn on when it is NOT already on 
        }
    }else{
        _ledStripState = 0;
        setLedStrip(0); //turn off
      }

    _ledStripStatePrevious = _ledStripState;
}

void setLedStrip(bool isOn)
{   
  digitalWrite(_pinLedStrip, isOn);
}

void setLedStripAnalog(int brightness)
{   
  analogWrite(_pinLedStrip, brightness);
}
