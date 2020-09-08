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

int latchPin = 8; //ST_CP
int clockPin = 12; //SH_CP
int dataPin = 11; //DS

const int countLeds = 7;

boolean registers[countLeds];

void setup(){
  #if DEBUG_ENABLE
    Serial.begin(115200);
    DEBUGS("\n[74HC595 Testing]");
  #endif
  
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
}

void loop() {  
  //oneByOneAllOn();
  //delay(1000);
  //helloWorld();
  for(int i = 0; i <= 3; i++){
    oneByOneRandom();
    delay(1000);
    }

  allOffLeds();

  wavePattern();
}

void allOffLeds(){
  for(int i = countLeds - 1; i >= 0; i--){
    registers[i] = LOW;
    delay(10);
    writeRegister();
   }
}

void writeRegister(){
  digitalWrite(latchPin, LOW);

  for(int i = countLeds - 1; i >= 0; i--){
    digitalWrite(clockPin, LOW);
    digitalWrite(dataPin, registers[i]); //write value of register
    digitalWrite(clockPin, HIGH);
    }

  digitalWrite(latchPin, HIGH);
 }

void wavePattern(){
  for(int i = 0; i < countLeds; i++){
    registers[i] = HIGH;
    delay(500);
    writeRegister();
    }

  for(int i = countLeds - 1; i >= 0; i--){
    registers[i] = LOW;
    delay(500);
    writeRegister();
    }
  }

void helloWorld(){
  for (int numberToDisplay = 0; numberToDisplay < 256; numberToDisplay++) {
    // take the latchPin low so 
    // the LEDs don't change while you're sending in bits:
    digitalWrite(latchPin, LOW);
    // shift out the bits:
    shiftOut(dataPin, clockPin, MSBFIRST, numberToDisplay);  

    //take the latch pin high so the LEDs will light up:
    digitalWrite(latchPin, HIGH);
    // pause before next value:
    delay(500);
  }
}

void oneByOneAllOn(){
  int leds = 0;
  updateShiftRegister(leds);
  delay(500);
  
  for(int i=0; i<8; i++){
    bitSet(leds,i);
    updateShiftRegister(leds);
    delay(500);
  }  
}

void oneByOneRandom(){
  int bitToSet = random(countLeds);

  // write to the shift register with the correct bit set high:
    registerWrite(bitToSet, HIGH);
  }

void registerWrite(int whichPin, int whichState) {
// the bits you want to send
  byte bitsToSend = 0;

  // turn off the output so the pins don't light up
  // while you're shifting bits:
  digitalWrite(latchPin, LOW);

  // turn on the next highest bit in bitsToSend:
  bitWrite(bitsToSend, whichPin, whichState);

  // shift the bits out:
  shiftOut(dataPin, clockPin, MSBFIRST, bitsToSend);

    // turn on the output so the LEDs can light up:
  digitalWrite(latchPin, HIGH);

}

void updateShiftRegister(int leds){
  digitalWrite(latchPin, LOW);
  shiftOut(dataPin, clockPin, MSBFIRST, leds);
  digitalWrite(latchPin, HIGH);

  #if DEBUG_ENABLE
    DEBUG("\n **leds**", leds);
  #endif
}
