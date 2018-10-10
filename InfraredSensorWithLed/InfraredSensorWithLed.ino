//Ground and Vcc pins
const int pinGrd = 5, pinVcc = 4;

//Infrared sensor module (obstacle avoidance)
const int pinObstacle = 6;
int isObstacle = HIGH;

//LED Pin
const int pinLed = 13;

void setup(){
  pinMode(pinVcc, OUTPUT);
  pinMode(pinGrd, OUTPUT);
  digitalWrite(pinVcc, HIGH);
  digitalWrite(pinGrd, LOW);
  
  pinMode(pinObstacle, INPUT);
  pinMode(pinLed, OUTPUT);
}

void loop(){
  isObstacle = digitalRead(pinObstacle);
  
  if(isObstacle){
    digitalWrite(pinLed, HIGH);
    delay(1000);
  }else{
    digitalWrite(pinLed, LOW);
  }
}
