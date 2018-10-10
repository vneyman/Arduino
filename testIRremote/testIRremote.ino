#include <IRremote.h>

const int greenLed = 5;
const int redLed = A1;
const int irReceivePin = 12;

IRrecv irrecv(irReceivePin);
decode_results results;
unsigned long key_value;
int keyValueConverted;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  irrecv.enableIRIn();
  irrecv.blink13(true);

  pinMode(greenLed, OUTPUT);
  pinMode(redLed, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  if (irrecv.decode(&results)){
        Serial.println(results.value, HEX);
        
        keyValueConverted = getKey();
        analogWrite(greenLed, keyValueConverted * 15);       

        doRedPin(keyValueConverted);
        
        irrecv.resume();
  }
}

  
void doRedPin(int keyValue){  
  static int pinStatus;
  
  switch(keyValue){
    case 10:
      pinStatus = LOW;
      break;
    case 11:
      pinStatus = HIGH;
      break;
    case 12:
      if(pinStatus == HIGH)
        { pinStatus = LOW; }
       else{
          pinStatus = HIGH;
        }
      break;
    default:
        Serial.print("do nothing");
    }

    digitalWrite(redLed, pinStatus);
  }

int getKey(){
  static int key;
  
  if (results.value == 0XFFFFFFFF)
        results.value = key_value;

    switch(results.value){
      case 0xFFA25D:
        Serial.println("1");
        key = 1;
      break;
      case 0xFF629D:
        Serial.println("2");
        key = 2;
      break;
      case 0xFFE21D:
        Serial.println("3");
        key = 3;
      break;
      case 0xFF22DD:
        Serial.println("4");
        key = 4;
      break;
      case 0xFF02FD:
      Serial.println("5");
        key = 5;
      break ;  
      case 0xFFC23D:
      Serial.println("6");
        key = 6;
      break ;               
      case 0xFFE01F:
      Serial.println("7");
        key = 7;
      break ;  
      case 0xFFA857:
      Serial.println("8");
        key = 8;
      break ;  
      case 0xFF906F:
        Serial.println("9");
        key = 9;
      break ;  
      case 0xFF6897:
        Serial.println("*");
        key = 10;
      break ;  
      case 0xFF9867:
        Serial.println("0");
        key = 0;
        break;
      case 0xFFB04F:
        Serial.println("#");
        key = 11;
      break ;
      case 0xFF10EF:
        Serial.println("left");
        key -= 2;
      break ;
      case 0xFF18E7:
        Serial.println("up");
        key += 1;
      break ;
      case 0xFF4AB5:
        Serial.println("down");
        key -= 1;
      break ;
      case 0xFF5AA5:
      Serial.println("right");
        key += 2;
      break ;
      case 0xFF38C7:
        Serial.println("OK");
        key = 12;
        break ;
    default:
        case UNKNOWN: Serial.println("UNKNOWN"); break ;
    }

    key_value = results.value;
    
    if(key > 255/15) { key = 17;}
    if(key < 0) { key = 0;}    
    
    return key;
  }
