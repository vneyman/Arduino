int led = 5;
int buttonA = 8;
int buttonB = 9;

void setup(){
  Serial.begin(9600);
  
  pinMode(led,OUTPUT);
  pinMode(buttonA, INPUT_PULLUP); //INPUT_PULLUP: pin is used as in input. If nothing is connected then set to high
  pinMode(buttonB, INPUT_PULLUP);
  
  while(! Serial);
}


void loop(){
  int buttonAstatus = digitalRead(buttonA);
  
  Serial.print("buttonA: ");
  Serial.println(buttonAstatus);
  
  if(digitalRead(buttonA) == LOW){
    digitalWrite(led,HIGH);
  }
  
  if(digitalRead(buttonB) == LOW){
    digitalWrite(led,LOW);
  }
}
