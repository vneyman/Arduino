//LCD library
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#include <Servo.h>

//Motor variables
//Left
const int pinENB = 11;
const int pinIN3 = 8;
const int pinIN4 = 9;

//Right
const int pinENA = 5;
const int pinIN1 = 6;
const int pinIN2 = 7;

//Speed settings
const int speedDefault = 125, speedMax = 255, speedMin = 0, speedIncrement = 20;
volatile int speedCurrent = speedMin;
const int delayRightLeftTurns = 50;

//Bluetooth variables
char btNew, btCurrent;
const char btValidCommands[] = {'f','l','r','b','s','t','w','k','n'};
int isBT = 0;

//Servo variables
Servo myServo; // create servo object to control servo
int pinServo = 3;
const int servoMinAngle = 20, servoMaxAngle = 160;

//Ultrasonic variables
const int pinEcho = A4;
const int pinTrig = A5;
float tempC = 25. , humidity = .5;
float pingTime;
float speedSound = 0.; //meters per sec
float distance = 0.; //cm
const int minDistance= 30; //cm
int rightDistance = 0, leftDistance = 0, middleDistance = 0;

//Line tracking variables
const int pinLT1 = 10, pinLT2 = 4, pinLT3 = 2;
boolean signalLT1 = digitalRead(pinLT1);
boolean signalLT2 = digitalRead(pinLT2);
boolean signalLT3 = digitalRead(pinLT3);
volatile int isLineTracking = 0;

// initialize the library with the numbers of the interface pins
LiquidCrystal_I2C lcd(0x3F, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);
const int lcdRowLength = 16;
const char* message[] = {"Ready", "Set", "GO!!"};
const int sizeOfMessage = sizeof(message)/sizeof(char *);

//Global variables
char currentDirection = 's';
const char validDirections[] = {'f','b','s'};
volatile int isChangeDirection = 0;



void setup() {
  Serial.begin(9600);
  
    // set up the LCD's number of columns and rows: 
  lcd.begin(lcdRowLength, 2);
  // Print a message to the LCD.
  lcd.print("hello, Adriano!");
  
  myServo.attach(pinServo);
  pinMode(pinEcho, INPUT);    
  pinMode(pinTrig, OUTPUT);
  
  pinMode(pinENB, OUTPUT);
  pinMode(pinIN3, OUTPUT);
  pinMode(pinIN4, OUTPUT);
  pinMode(pinENA, OUTPUT);
  pinMode(pinIN1, OUTPUT);
  pinMode(pinIN2, OUTPUT);
  
  mStop();
}

void loop() {
  checkObstacle();
  bluetoothRemote();
}

void bluetoothRemote(){
  btNew = Serial.read(); //The Bluetooth serial port to receive the data in the function
  delay(50);
  
    for(int i=0; i<sizeof(btValidCommands); i++){
      if(btNew == btValidCommands[i]) {
        ChangeDirection(btNew);
        
        btCurrent = btNew;
        break;
      }
    }
    
    btCurrent = commandToMotorAction(btCurrent);    
}

char commandToMotorAction(char newCommand){
  
   switch(newCommand){
    case 'f':
      mForward();
      break;
    case 'l':
      mLeft();      
      newCommand = currentDirection;
      break;
    case 'r':
      mRight();
      newCommand = currentDirection;
      break;
    case 'b':
      mBack();
      break;
    case 's':
      mStop();
      break;
    case 't':
      mFaster();
      newCommand = currentDirection;
      break;
    case 'w':
      mSlower();
      newCommand = currentDirection;
      break;
    case 'k':
      lineTracker();
      newCommand = currentDirection = 'k';
      break;
    case 'n':
      mStop();
      newCommand = currentDirection = 'f';
      break;
    default:
      break; //do nothing for now
  }
  
  return newCommand;
}

void ChangeDirection(char newCommand){
  //check if changing direction
  if(newCommand != currentDirection) 
  { 
    for(int i=0; i<sizeof(validDirections); i++){
      if(newCommand == validDirections[i]) {
        isChangeDirection = 1;
        mStop(); //stop before changing direction
        speedCurrent = speedDefault;
        currentDirection = newCommand;
        break;
      }
    }  
  }else{
    mFaster();
    isChangeDirection = 0; }
}

void mForward(){
  if(speedCurrent == speedMin) { speedCurrent = speedDefault;}
  
  leftForward();
  rightForward();
}

void mLeft(){
  speedCurrent = (int)(speedMax*.9);
  leftBack();
  rightForward();
  delay(delayRightLeftTurns);
  speedCurrent = speedDefault;
}

void mRight(){
  speedCurrent = (int)(speedMax*.9);
  leftForward();
  rightBack();
  delay(delayRightLeftTurns);
  speedCurrent = speedDefault;
}

void mBack(){
  if(speedCurrent == speedMin) { speedCurrent = speedDefault;}
  
  leftBack();
  rightBack();
}

void mStop(){
  speedCurrent = speedMin;
  leftBrake();
  rightBrake();
}

void mFaster(){
  speedCurrent+=speedIncrement;
  if(speedCurrent>speedMax) {speedCurrent = speedMax;}
}

void mSlower(){
  speedCurrent-=speedIncrement;
  if(speedCurrent < speedMin) {mStop();}
}


float ultraSonicSensorDistance(){
  //tempC = thermistorCelcius();

  //create pulse
  digitalWrite(pinTrig, LOW);
  delayMicroseconds(2);
  digitalWrite(pinTrig, HIGH);
  delayMicroseconds(10);
  digitalWrite(pinTrig, LOW);
  
  pingTime = pulseIn(pinEcho, HIGH); //time that signal was returned in microseconds
  
  speedSound = 331.4 + (.606 * tempC) + (.0124 * humidity); //Speed of sound in meters per second
  distance = speedSound * pingTime/2. / pow(10,6) * 100.; //distance in millimeters
  
  return (int)distance;
}

void checkObstacle() 
{ 
    myServo.write(90);//setservo position according to scaled value
    delay(100); 
    middleDistance = ultraSonicSensorDistance();
    
    #ifdef send
    Serial.print("middleDistance=");
    Serial.println(middleDistance);
    #endif

    if(middleDistance<=minDistance)
    {     
      mStop();
      delay(100); 	  
      myServo.write(servoMinAngle);//10°-180°          
      delay(100);
      rightDistance = ultraSonicSensorDistance();

      #ifdef send
      Serial.print("rightDistance=");
      Serial.println(rightDistance);
      #endif

      myServo.write(90);              
      delay(100);                                       
      myServo.write(servoMaxAngle);            
      delay(100); 
      leftDistance = ultraSonicSensorDistance();

      #ifdef send
      Serial.print("leftDistance=");
      Serial.println(leftDistance);
      #endif

      delay(100);
      myServo.write(90);           
      delay(100);
      
      if( rightDistance<=minDistance && leftDistance<=minDistance ){
        mBack();
        delay(200);
      }
      else if(rightDistance>leftDistance)  
      {
        mRight();
      }
      else if(rightDistance<leftDistance)
      {
        mLeft();
      }
      else
      {
        mForward();
        delay(100);
      }
      
      checkObstacle();
    }
    //else
    //    mForward();        
}

void lineTracker(){
  if(signalLT2){
    mForward();
  }else if(signalLT1){
    mLeft();
    while(signalLT1);
  }else if(signalLT3){
    mRight();
    while(signalLT1);
  }else{
    mForward();
  }
}

void leftBrake(){
  analogWrite(pinENB, speedCurrent);
  digitalWrite(pinIN3, LOW);
  digitalWrite(pinIN4, LOW);
}

void rightBrake(){
  analogWrite(pinENA, speedCurrent);
  digitalWrite(pinIN1, LOW);
  digitalWrite(pinIN2, LOW);
}

void leftForward(){
  analogWrite(pinENB, speedCurrent);
  digitalWrite(pinIN3, LOW);
  digitalWrite(pinIN4, HIGH);
}

void rightForward(){
  analogWrite(pinENA, speedCurrent);
  digitalWrite(pinIN1, HIGH);
  digitalWrite(pinIN2, LOW);
}

void leftBack(){
  analogWrite(pinENB, speedCurrent);
  digitalWrite(pinIN3, HIGH);
  digitalWrite(pinIN4, LOW);
}

void rightBack(){
  analogWrite(pinENA, speedCurrent);
  digitalWrite(pinIN1, LOW);
  digitalWrite(pinIN2, HIGH);
}

void lcdUpdate(uint8_t charStart, uint8_t line, String value){
  int valueLength = value.length();
  //int count = 0;
  String blank = repeatString(" ", valueLength);
  
  lcd.setCursor(charStart, line);
  lcd.print(blank);
  lcd.setCursor(charStart, line);
  lcd.print(value);
}


String repeatString(String charToRepeat, int repeat){
  String stringValue = "";
  int count = 1;
  
  do{
    stringValue += charToRepeat;
    count++;
  }while(count<repeat);
  
  return stringValue;
}

/*
double thermistorCelcius() {
 Vout = analogRead(thermPin);

 A = 1.129148e-03, B = 2.34125e-04, C = .876741e-07;
 R2 = R1static * (1023.0 / Vout - 1.0);
 R2log = log(R2);
 T = (1.0 / (A + B*R2log + C*R2log*R2log*R2log));
 T = T - 273.15;     
 return T;
}*/
