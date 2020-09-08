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
byte _stateLeds = 0, _stateLedsPrevious = 0; //0 is off; 1 is wave pattern; 2 is random pattern;
int delayDefault = 1750;

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

  //prepare program
  oneByOneRandomWrapper(10); offLeds();
}

void loop() { 
  getLdr();

  #if DEBUG_ENABLE
    DEBUG("\n _stateLeds:\t", _stateLeds);
  #endif

  switch(_stateLeds){
    case 1: //IR sensor high (motion detected)
      wavePatternIrSensor();
      break;
     case 2: //night time
      oneByOneRandomWrapper(20);
      break;
     default: //day time, IR sensor not detected
      if(_stateLedsPrevious != _stateLeds){ offLeds(); }
      break;
    }

  _stateLedsPrevious = _stateLeds;
  _stateLeds = 0;
}

void wavePattern(int ledMaxOn){
  #if DEBUG_ENABLE
    DEBUG("\n wavePattern:\t", ledMaxOn);
  #endif
  
  int ledOrder[] = { 0, 3, 4, 1, 5, 2, 6 }; 

    for(int i = 0; i < _countLeds; i++){
      //Serial.print("\n"); Serial.println(_stateLeds); //will not work without this? DO NOT KNOW WHY.   
      
      _registers[ledOrder[i]] = HIGH;

      if(i >= ledMaxOn) {
        _registers[ledOrder[i - ledMaxOn]] = LOW;
        }

      writeRegister();      
      delay(delayDefault);
    }

    for(int i = _countLeds - 1; i >= 0; i--){
      //Serial.print("\n"); Serial.println(_stateLeds); //will not work without this? DO NOT KNOW WHY.   
      
      _registers[ledOrder[i]] = HIGH;

      if(i <= _countLeds - ledMaxOn) {
        _registers[ledOrder[i + ledMaxOn]] = LOW;
        }
                 
      writeRegister();
      delay(delayDefault);
    }
  }


void wavePatternIrSensor(){
      offLeds();
      delayDefault = 250;
      wavePattern(1);
      delayDefault = 500;
      wavePattern(2);
      offLeds();
      delayDefault = 750;
  }

void oneByOneRandomWrapper(int ledNumberToLight){
    for(int i = 0; i < ledNumberToLight; i++){
      oneByOneRandom();
      
      if(_stateLeds == 1) { wavePatternIrSensor(); break; }
      
      delay(delayDefault);
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
  static int bitToSetPrevious;
  int bitToSet = random(_countLeds);

  while(bitToSet == bitToSetPrevious){
    bitToSet = random(_countLeds);
    }

  // write to the shift register with the correct bit set high:
  registerWrite(bitToSet, HIGH);
  
  bitToSetPrevious = bitToSet;
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
  _stateLeds = 1;
}

void offLeds(){
  for(int i = _countLeds - 1; i >= 0; i--){
    _registers[i] = LOW;
    delay(5);
    writeRegister();
   }
}
