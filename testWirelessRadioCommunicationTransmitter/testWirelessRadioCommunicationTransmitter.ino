#include <SPI.h>
#include "nRF24L01.h" //is this necessary?
#include "RF24.h"
#include <Esplora.h>

// Turn on debug statements to the serial output
#define  DEBUG_ENABLE  1

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

/****************** User Config ***************************/
/***      Set this radio as radio number 0 or 1         ***/
bool radioNumber = 0;

/* Hardware configuration: Set up nRF24L01 radio on SPI bus plus pins 7 & 8 (on Uno/Nano) */
// pin 0 & 1 (on Esplora)
RF24 radio(0, 1);

const byte address[6] = "A0001";
byte addresses[][6] = {"A0001","A0002"};

// Used to control whether this node is sending or receiving
bool role = 0;

//Esplora controls
int _sliderValue = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  DEBUGS("\n[Wireless Radio Communication test: Transmitter]");

  radio.begin();
  
  radio.setPALevel(RF24_PA_LOW);

  // Open a writing and reading pipe on each radio, with opposite addresses
  if(radioNumber){
    radio.openWritingPipe(addresses[1]);
    radio.openReadingPipe(1,addresses[0]);
  }else{
    radio.openWritingPipe(addresses[0]);
    radio.openReadingPipe(1,addresses[1]);
  }

  radio.openWritingPipe(address);

  radio.setDataRate(RF24_250KBPS);
  radio.setChannel(119);

  radio.stopListening();

  delay(50);
}

void loop() {
  // put your main code here, to run repeatedly:
  Esplora.writeRGB(0, 200, 0);
  
  //const char text[] = "Hello from Esplora";
  //radio.write(&text, sizeof(text));
  //delay(5);
  
  transmitSlider();
  
  delay(1000);
  Esplora.writeRGB(0, 0, 0);
  delay(500);
}

void transmitSlider(){
  _sliderValue = Esplora.readSlider();
  _sliderValue = map(_sliderValue, 0, 1023, 0, 255);

  //Serial.print("_sliderValue:\t"); Serial.println(_sliderValue);
  
  radio.write(&_sliderValue, sizeof(_sliderValue));

  }
