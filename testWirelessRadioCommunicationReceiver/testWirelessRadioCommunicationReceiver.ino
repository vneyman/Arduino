#include <SPI.h>
#include "nRF24L01.h" //is this necessary?
#include "RF24.h"

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

//pins
const unsigned int _ledPin = 5;

/****************** User Config ***************************/
/***      Set this radio as radio number 0 or 1         ***/
bool radioNumber = 1;

/* Hardware configuration: Set up nRF24L01 radio on SPI bus plus pins 7 & 8 (on Uno/Nano) */
// pin 0 & 1 (on Esplora)
RF24 radio(7,8);

const byte address[6] = "A0001";
byte addresses[][6] = {"A0001","A0002"};

// Used to control whether this node is sending or receiving
bool role = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  DEBUGS("\n[Wireless Radio Communication test: Receiver]\n");

  radio.begin();
  
  radio.setPALevel(RF24_PA_MAX);

  // Open a writing and reading pipe on each radio, with opposite addresses
  if(radioNumber){
    radio.openWritingPipe(addresses[1]);
    radio.openReadingPipe(1,addresses[0]);
  }else{
    radio.openWritingPipe(addresses[0]);
    radio.openReadingPipe(1,addresses[1]);
  }

  radio.openReadingPipe(0, address);

  radio.setDataRate(RF24_250KBPS);
  radio.setChannel(119);
  
  radio.startListening();

  delay(50);
}

void loop() {
  // put your main code here, to run repeatedly:
  /*
  if(radio.available()){
    char text[32] = "";

    radio.read(&text, sizeof(text));
    Serial.println(text);
    
    }
    */

   listenToEsploraSlider();
}

void listenToEsploraSlider(){
  if(radio.available()){
    while(radio.available()){
      unsigned int ledValue = 0;

      radio.read(&ledValue, sizeof(ledValue));
      
      Serial.print("led value\t"); Serial.println(ledValue);            
      analogWrite(_ledPin, ledValue);
      delay(50);
      }
    }
  }
  
