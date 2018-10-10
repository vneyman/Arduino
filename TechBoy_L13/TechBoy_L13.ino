int redPin = 11;
int greenPin = 10;
int bluePin = 9;
int maxBrightness = 200;

void rgbLedWriteUp(int ledPin){
    for(int i=0;i<=maxBrightness; i=i+20){
      if(i>maxBrightness) {i=maxBrightness;}
      analogWrite(ledPin,i);
      delay(300);
  }

  delay(1000);
}

void rgbLedWriteDown(int ledPin){
    for(int i=maxBrightness;i>=0; i=i-20){
      if(i<0) {i=0;}
      analogWrite(ledPin,i);
      delay(300);
  }

  delay(1000);
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin,OUTPUT);  
}

void loop() {
  // put your main code here, to run repeatedly:

  analogWrite(redPin,0);
  analogWrite(greenPin,0);
  analogWrite(bluePin,0);

  delay(2000);
  
  rgbLedWriteUp(redPin);
  rgbLedWriteUp(greenPin);
  rgbLedWriteUp(bluePin);

  rgbLedWriteDown(redPin);
  rgbLedWriteDown(greenPin);
  rgbLedWriteDown(bluePin);  
}
