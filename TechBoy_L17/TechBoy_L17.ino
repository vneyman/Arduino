#include <Servo.h>
#include <math.h>

int redPin = 9;
int yellowPin = 10;

int potPin = 5;
int potValue = 0;

//Ultrasonic sensor Sr04
int trigPin = 12;
int echoPin = 5;
float pingTime;
float speedSound = 0.0;
float distance = 6.; //inch
float tempF, tempC; //temperature
float humidity = 50; //relative humidity in %

//Thermistor
int thermPin = 0;
float R1static = 10000.;
float Vout, R2, R2log, A, B, C, T;

//Servo settings
Servo myServo;
int servoPin = 3;
int servoDelay = 25;
int servoMinAngle = 30;
int servoMaxAngle = 180;
int servoAngle = 0;

double thermistorCelcius() {
 Vout = analogRead(thermPin);
 //Serial.print("Vout: "); Serial.println(Vout);

 A = 1.129148e-03, B = 2.34125e-04, C = .876741e-07;
 R2 = R1static * (1023.0 / Vout - 1.0);
 R2log = log(R2);
 T = (1.0 / (A + B*R2log + C*R2log*R2log*R2log));
 T = T - 273.15;     
 return T;
}

void ultraSonicSensorDistance(){
  tempC = thermistorCelcius();
  
  //create pulse
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  pingTime = pulseIn(echoPin, HIGH); //time that signal was returned in microseconds
  
  speedSound = 331.4 + (.606 * tempC) + (.0124 * humidity); //Speed of sound in meters per second
  distance = speedSound * pingTime/2. / pow(10,6) * 100.; //distance in centimeters
  
  speedSound = speedSound * 3600./1609.34; //miles per hour
  tempF = (tempC*9./5.) + 32.;
  
  Serial.print("Temperature: ");
  Serial.print(tempF);
  Serial.println(" F");
  Serial.print("Speed of sound in air is ");
  Serial.print(speedSound);
  Serial.println(" (miles/h)");
  Serial.print("Distance of object: ");
  Serial.print(distance);
  Serial.println(" cm");
  Serial.println("------------");
  delay(1000);
}

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
  //Serial.print("Potentiometer value: ");
  //Serial.println(potValue);

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


void servoAngleFromPot(int potValue){  
  servoAngle = map(potValue,0,1023,servoMaxAngle, servoMinAngle);
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
  
  pinMode(thermPin, INPUT);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  
  while(!Serial){}
}

void loop() {
  // put your main code here, to run repeatedly:
  int inputValue = readPotentiometer();
  //blinkPins(redPin, yellowPin, inputValue);
  servoAngleFromPot(inputValue);
  
  ultraSonicSensorDistance();
}
