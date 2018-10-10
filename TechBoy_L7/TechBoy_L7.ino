int redLedPin = 9;
int yellowLedPin = 10;
int redLedOn = 1000;
int yellowLedOn = 750;
int redLedOff = 500;
int yellowLedOff = 500;
int redLedNumBlinks;
int yellowLedNumBlinks;
int defaultNumBlinks = 5;
int waitUserInputTime = 10;
String redLedMessage = "***Begin Red LED blink***";
String yellowLedMessage = "***Begin Yellow LED blink***";
String blinkCountMessage = "  Blink count: ";

void blinkLed(int ledPin, int numBlink, int ledOnTime, int ledOffTime, String message) {
  int i = 1;

  while(i<=numBlink){
    Serial.print(message);
    Serial.println(i);
      
    digitalWrite(ledPin, HIGH);
    delay(ledOnTime);
    digitalWrite(ledPin, LOW);
    delay(ledOffTime);
    
    i=i+1;
  }  

}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  
  pinMode(redLedPin, OUTPUT);
  pinMode(yellowLedPin, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.println("How many times do you want RED LED to blink?");
  int tempDelay = 0;
  while(Serial.available() == 0 && tempDelay <= waitUserInputTime){
    delay(1000);
    tempDelay = tempDelay + 1;
  }
  redLedNumBlinks = Serial.parseInt();
  if(redLedNumBlinks <=0) {redLedNumBlinks = defaultNumBlinks;}
  
  Serial.println("How many times do you want YELLOW LED to blink?");
  tempDelay = 0;
  while(Serial.available() == 0 && tempDelay <= waitUserInputTime){
    delay(1000);
    tempDelay = tempDelay + 1;
  }
  yellowLedNumBlinks = Serial.parseInt();
  if(yellowLedNumBlinks <=0) {yellowLedNumBlinks = defaultNumBlinks;}
  
  Serial.println(redLedMessage);
  blinkLed(redLedPin, redLedNumBlinks, redLedOn, redLedOff, blinkCountMessage);
  Serial.println(yellowLedMessage);
  blinkLed(yellowLedPin, yellowLedNumBlinks, yellowLedOn, yellowLedOff, blinkCountMessage);

}
