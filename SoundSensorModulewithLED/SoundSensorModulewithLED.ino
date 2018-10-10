const int pinSoundA = A0, pinSoundD = 3;
const int pinLEDred = 9, pinLEDgreen = 6, pinLEDblue = 3;

int valueSound1, valueSound2;
int stateLED;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  
  pinMode(pinSoundA, INPUT);
  pinMode(pinLEDred, OUTPUT);
  pinMode(pinLEDgreen, OUTPUT);
  pinMode(pinLEDblue, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  valueSound1 = analogRead(pinSoundA);
  delay(1);
  valueSound2 = analogRead(pinSoundA);

  getValueLED(valueSound1, valueSound2);
}

void getValueLED(int* sound1, int* sound2){
  int valueDelay = 1000;
  stateLED = 1;
  
  int valueLED = map(sound2, 100, 1023, 150, 255);
  
  if(sound2 - sound1 > 10){        
    valueLED = map(sound2, 100, 1023, 230, 255);
    analogWrite(pinLEDgreen, valueLED);
        
    //Serial.println(valueLED);
  }
  else if(sound2 - sound1 > 5){
    analogWrite(pinLEDblue, valueLED);
  }
  else if(sound2 - sound1 > 1){
    analogWrite(pinLEDred, valueLED);
  }else if(sound2 > 100){
    analogWrite(pinLEDred, sound2);    
    analogWrite(pinLEDgreen, sound2);  
    analogWrite(pinLEDblue, sound2);
    valueDelay = 5;
    }
  else{
    stateLED = 0;
    valueLED = 0;
    valueDelay = 0;
    analogWrite(pinLEDred, valueLED);
    analogWrite(pinLEDgreen, valueLED);
    analogWrite(pinLEDblue, valueLED);    
    }

  delay(valueDelay);
  
}
