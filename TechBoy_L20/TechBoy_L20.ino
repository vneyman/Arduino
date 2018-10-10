#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Servo.h>
#include <math.h>

//Ultrasonic sensor US-100 or Sr04
const int trigPin = 12;
const int echoPin = 5;
const int sensorMinDistance=2, sensorMaxDistance=100; //cm
float pingTime;
float speedSound = 0.0;
float distance; //inch
float tempF, tempC; //temperature
float humidity = 50; //relative humidity in %

//Thermistor
const int thermPin = A0;
float R1static = 10000.;
float Vout, R2, R2log, A, B, C, T;

//Servo settings
Servo myServo;
const int servoPin = 3;
const int servoDelay = 25;
const int servoMinAngle = 35;
const int servoMaxAngle = 180;
int servoAngle = 0;

//LCD settings
LiquidCrystal_I2C lcd(0x3F, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE); // Set the LCD I2C address
const String tempStr = "Temperature:";
const int tempCharStart = 0, tempLine=0;
const String distStr = "Distance:";
const int distCharStart = 0, distLine=1;
const String runtStr = "Run time:";
const int runtCharStart = 0, runtLine=3;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
      
  myServo.attach(servoPin);
  
  pinMode(thermPin, INPUT);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  
  lcd.begin(20,4);
  lcdUpdate(tempCharStart, tempLine, tempStr);
  lcdUpdate(distCharStart, distLine, distStr);  
  lcdUpdate(runtCharStart, runtLine, runtStr);
  
  //while(!Serial){}
}

void loop() {
  // put your main code here, to run repeatedly:
  //int inputValue = readPotentiometer();
  //blinkPins(redPin, yellowPin, inputValue);
   
  distance = ultraSonicSensorDistance(); 
  servoAngleFromDistance(distance);
  
  distance = (float)((int)(distance*100.))/100.;
  
  char tempBuffer[9], distBuffer[9];
  char tempSuffix[3] = " F", distSuffix[4] = " cm";
  dtostrf(tempF,5,1,tempBuffer);
  dtostrf(distance,5,1,distBuffer);
  strcat(tempBuffer, tempSuffix);
  strcat(distBuffer, distSuffix);
  
  lcdUpdate(tempCharStart + tempStr.length() + 1, tempLine, tempBuffer);
  lcdUpdate(distCharStart + distStr.length() + 1, distLine, distBuffer);
  lcdUpdate(runtCharStart + runtStr.length() + 1, runtLine, (String)(millis()/1000) + " s");
  delay(500);
}

void lcdUpdate(uint8_t charStart, uint8_t line, String value){
  int valueLength = value.length();
  int count = 0;
  String blank = " ";
  
  do{
    blank += " ";
    count++;
  }while(count<=valueLength);
  
  lcd.setCursor(charStart, line);
  lcd.print(blank);
  lcd.setCursor(charStart, line);
  lcd.print(value);
}

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

float ultraSonicSensorDistance(){
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
  
  /*
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
  */
  
  if(distance>=sensorMaxDistance){
    distance = sensorMaxDistance;
  }
  
  if(distance<=sensorMinDistance){
    distance = sensorMinDistance;
  }
  
  return distance;
}


void servoAngleFromDistance(const float objectDistance){    
  servoAngle = servoMaxAngle - (objectDistance - sensorMinDistance)*(servoMaxAngle - servoMinAngle)/(sensorMaxDistance - sensorMinDistance);
  myServo.write(servoAngle);
  
  delay(servoDelay);
}

