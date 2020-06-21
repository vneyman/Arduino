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

// 74HC595 PINS
#define _pinLatch 8 //ST_CP
#define _pinClock 12 //SH_CP
#define _pinData 11 //DS

const int _countLeds = 7; //number of pair of eyeballs
boolean _registers[_countLeds];
byte _stateLeds = 0, _stateLedsPrevious = 0; //0 is off; 1 is random pattern; 2 is wave pattern

//LDR settings
#define _pinLdr A0
int _ldrThreshold = 400; //initiate wave pattern when LDR below threshold (_stateLeds 2) 

//Infrared Sensor settings
const int _pinIrInterrupt = 1; //Interrupt pin conversion: pin 2 = 0; pin 3 = 1

void setup(){  
  #if DEBUG_ENABLE
    Serial.begin(115200);
    DEBUGS("\n[74HC595 Testing]");
  #endif
  
  pinMode(_pinLatch, OUTPUT);
  pinMode(_pinClock, OUTPUT);
  pinMode(_pinData, OUTPUT);
  pinMode(_pinLdr, INPUT);

  attachInterrupt(_pinIrInterrupt, getIr, RISING);
}

void loop() { 
  getLdr();

  switch(_stateLeds){
    case 1: //IR sensor high (motion detected)
      oneByOneRandomWrapper();
      break;
     case 2: //night time
      wavePattern();
      break;
     default: //day time, IR sensor not detected
      if(_stateLedsPrevious != _stateLeds){ offLeds(); }
      break;
    }

  _stateLedsPrevious = _stateLeds;
  _stateLeds = 0;
}

void wavePattern(){
  for(int i = 0; i < _countLeds; i++){
    Serial.println(_stateLeds); //will not work without this? DO NOT KNOW WHY.
    if(_stateLeds == 1) { wavePatternIrSensor(); break; }
    
    _registers[i] = HIGH; 
    delay(500);
    writeRegister();
    }

  for(int i = _countLeds - 1; i >= 0; i--){
    Serial.println(_stateLeds); //will not work without this? DO NOT KNOW WHY.
    if(_stateLeds == 1) { wavePatternIrSensor(); break; }

    _registers[i] = LOW;
    delay(500);
    writeRegister();
    }
  }

void wavePatternIrSensor(){
      offLeds(); 
      oneByOneRandomWrapper(); 
  }

void oneByOneRandomWrapper(){
    for(int i = 0; i < 20; i++){
      oneByOneRandom();
      delay(500);
    }
}

void writeRegister(){
  digitalWrite(_pinLatch, LOW);

  for(int i = _countLeds - 1; i >= 0; i--){
    digitalWrite(_pinClock, LOW);
    digitalWrite(_pinData, _registers[i]); //write value of register
    digitalWrite(_pinClock, HIGH);
    }

  digitalWrite(_pinLatch, HIGH);
 }

void oneByOneRandom(){
  int bitToSet = random(_countLeds);

  // write to the shift register with the correct bit set high:
  registerWrite(bitToSet, HIGH);
}

void registerWrite(int whichPin, int whichState) {
// the bits you want to send
  byte bitsToSend = 0;

  // turn off the output so the pins don't light up
  // while you're shifting bits:
  digitalWrite(_pinLatch, LOW);

  // turn on the next highest bit in bitsToSend:
  bitWrite(bitsToSend, whichPin, whichState);

  // shift the bits out:
  shiftOut(_pinData, _pinClock, MSBFIRST, bitsToSend);

    // turn on the output so the LEDs can light up:
  digitalWrite(_pinLatch, HIGH);
}

void getLdr(){
  int ldrValue = analogRead(_pinLdr);
  
  #if DEBUG_ENABLE
    DEBUG("\nLDR:\t", ldrValue);
    delay(500);
  #endif
  
  if(ldrValue < _ldrThreshold){ _stateLeds = 2; }
}

void getIr(){  
  #if DEBUG_ENABLE
    DEBUGS("\n IR sensor HIGH");
  #endif

  _stateLeds = 1;
}

void offLeds(){
  for(int i = _countLeds - 1; i >= 0; i--){
    _registers[i] = LOW;
    delay(10);
    writeRegister();
   }
}
