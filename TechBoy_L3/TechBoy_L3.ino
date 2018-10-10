int redLedPin = 9;
int yellowLedPin = 10;
int redLedOn = 1000;
int yellowLedOn = 750;
int redLedOff = 500;
int yellowLedOff = 500;
int redLedNumBlink = 5;
int yellowLedNumBlink = 5;
String redLedMessage = "***Begin Red LED blink***";
String blinkCountMessage = "  Blink count: ";

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  
  pinMode(redLedPin, OUTPUT);
  pinMode(yellowLedPin, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.println(redLedMessage);
  
  for(int i=0; i<redLedNumBlink; i=i+1){
    Serial.print(blinkCountMessage);
    Serial.println(i+1);
    
    digitalWrite(redLedPin, HIGH);
    delay(redLedOn);
    digitalWrite(redLedPin, LOW);
    delay(redLedOff);
    }

  Serial.println("***Begin Yellow LED blink***");
    for(int i=0; i<yellowLedNumBlink; i=i+1){
      Serial.print(blinkCountMessage);
      Serial.println(i+1);
    
      digitalWrite(yellowLedPin, HIGH);
      delay(yellowLedOn);
      digitalWrite(yellowLedPin, LOW);
      delay(yellowLedOff);
    }
}
