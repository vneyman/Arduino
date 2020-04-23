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
volatile byte _pirState = LOW;

//Strip LEDs
const int _pinLedStrip = 6;      // the number of the LED pin
const unsigned int _ledStripOnTime = 30000; //1 second: 1000 seconds; 60 seconds: 60*1000
byte _ledStripState;
unsigned long _ledStripLastTimeStart;

// the following variables are long's because the time, measured in milliseconds,
// will quickly become a bigger number than can be stored in an int.

void setup() {
  #if DEBUG_ENABLE
    Serial.begin(115200);
    DEBUGS("\n[Ender 5 Led Strip]");
  #endif

  pinMode(_pinPir, INPUT); //digital
  pinMode(_pinLedStrip, OUTPUT); //digital
  
  setLedStripAnalog(150); delay(500); //on
  setLedStripAnalog(0); delay(500); //off

  //attachInterrupt(_pinPirInterrupt, readPirSensor, CHANGE);
}

void loop() {   
  if(digitalRead(_pinPir)){
    _pirState = HIGH; 
    turnOnLedStrip();
    _ledStripState = HIGH;
    }else{
      _pirState = LOW;
      if(_ledStripState == HIGH ) { setLedStrip(0); }
      _ledStripState = LOW;
      }
}

void turnOnLedStrip(){
  #if DEBUG_ENABLE
    DEBUGS("\n **turnOnLedStrip**");
  #endif
  
  _ledStripLastTimeStart = millis();
  if(_ledStripState == LOW) { setLedStrip(1); }
  
  while((millis() - _ledStripLastTimeStart) <= _ledStripOnTime && millis() > _ledStripLastTimeStart){
    1==1; //do nothing
    }
}

void readPirSensor(){
  _pirState = digitalRead(_pinPir);
  }

void setLedStrip(bool isOn)
{   
  digitalWrite(_pinLedStrip, isOn);
}

void setLedStripAnalog(int brightness)
{   
  analogWrite(_pinLedStrip, brightness);
}
