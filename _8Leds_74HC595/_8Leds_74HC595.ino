int lightPin = 0;
int latchPin = 11;
int clockPin = 9;
int dataPin = 12;

byte leds = 0;


void updateShiftRegister(){
  digitalWrite(latchPin, LOW);
  shiftOut(dataPin, clockPin, LSBFIRST, leds);
  digitalWrite(latchPin, HIGH);
  
  Serial.print("leds: ");
  Serial.println(leds);
}


void setup(){
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
  
  Serial.begin(9600);
}

void loop() {
  int reading = analogRead(lightPin);
  Serial.print("photocell: ");
  Serial.println(reading);
  
  leds = 0;
  updateShiftRegister();
  delay(1500);
  
  
  for(int i=0; i<8; i++){
    bitSet(leds,i);
    updateShiftRegister();
    delay(1000);
  }
  
}
