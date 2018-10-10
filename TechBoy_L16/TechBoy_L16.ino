#include <Servo.h>

int redPin = 9;
int yellowPin = 10;

int potPin = 5;
int potValue = 0;
const float maxVolts = 5.0;

Servo myServo;
int servoPin = 3;
int servoDelay = 25;
int servoAngle = 0;

int analogReadToWrite(int readValue){
  int outputValue = map(readValue,0,1023,0,255);

  return outputValue;
}

int analogReadToSeconds(int readValue){
  float writeValue = 250.0 + readValue*4750.0/1023.0;
  int outputValue = (int)round(writeValue);
  
  Serial.print("ms delay: ");
  Serial.println(outputValue);
  return outputValue;
}

int readPotentiometer(){
  potValue = analogRead(potPin);
  Serial.print("Potentiometer value: ");
  Serial.println(potValue);

  return potValue;
}

void blinkPins(int pin1, int pin2, int inputValue){
  digitalWrite(pin1, HIGH);
  delay(analogReadToSeconds(inputValue));
  digitalWrite(pin1, LOW);
  digitalWrite(pin2, HIGH);
  delay(analogReadToSeconds(inputValue));
  digitalWrite(pin2, LOW);
}

void servoInputAngle(){
  Serial.print("Servo angle: ");
  
  while(Serial.available() == 0){}
  servoAngle = Serial.parseInt();
  myServo.write(servoAngle);
  delay(servoDelay);
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  
  pinMode(redPin, OUTPUT);
  pinMode(yellowPin, OUTPUT);
  pinMode(potPin, INPUT);
  myServo.attach(servoPin);
  
  while(!Serial){}
}

void loop() {
  // put your main code here, to run repeatedly:
  int inputValue = readPotentiometer();
  blinkPins(redPin, yellowPin, inputValue);
  delay(10);
  
  servoInputAngle();
}
