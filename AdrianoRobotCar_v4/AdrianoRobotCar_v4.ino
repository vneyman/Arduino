#include <Servo.h>
#include <IRremote.h>

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

//Send signals to Nano variables
const int pinNano1 = A2, pinNano2 = A0;
volatile unsigned int nanoStatus;
unsigned long lastNanoChange;
unsigned int nanoUpdateDelay = 2000;

//IR variables
const int pinIRreceive = 12;
char lastKey;
IRrecv irrecv(pinIRreceive);
decode_results results;

//Global variables
char currentDirection = 's';
const char validDirections[] = {'f','b','s'};
volatile int isChangeDirection = 0;


void setup() {
  Serial.begin(9600);
  
  irrecv.enableIRIn();
  
  myServo.attach(pinServo);
  pinMode(pinEcho, INPUT);
  pinMode(pinTrig, OUTPUT);
  
  pinMode(pinENB, OUTPUT);
  pinMode(pinIN3, OUTPUT);
  pinMode(pinIN4, OUTPUT);
  pinMode(pinENA, OUTPUT);
  pinMode(pinIN1, OUTPUT);
  pinMode(pinIN2, OUTPUT);
  
  pinMode(pinNano1, OUTPUT);
  pinMode(pinNano2, OUTPUT);
  
  mStop();
}

void loop() {
  checkObstacle();
  bluetoothRemote();
  IRRemote();
  
  turnOffObstacleDetectedToNano();
}


void writeObstacleDetectedToNano(){   
    digitalWrite(pinNano1, HIGH);
    digitalWrite(pinNano2, HIGH);
    
    nanoStatus = 1;
}

void turnOffObstacleDetectedToNano(){
  if((millis() - lastNanoChange) > nanoUpdateDelay && nanoStatus==1){
    
    digitalWrite(pinNano1, LOW);
    digitalWrite(pinNano2, LOW);
    
    nanoStatus = 0;
    lastNanoChange = millis();
  }
}

void IRRemote(){
  if (irrecv.decode(&results)){
        Serial.println(results.value, HEX);
        
        
        irrecv.resume();
  }  
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
    
    //#ifdef send
    Serial.print("middleDistance=");
    Serial.println(middleDistance);
    //#endif

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

      //delay(100);
      myServo.write(90);           
      //delay(100);
      
      if( rightDistance<=minDistance && leftDistance<=minDistance ){
        mBack();
        delay(100);
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
        //mForward();
        delay(100);
      }
      
      checkObstacle();
    }
     
}

char IRKeyToDirection(){
  char key;
  
  if (results.value == 0XFFFFFFFF)
        results.value = lastKey;

    switch(results.value){
      case 0xFFA25D:
        Serial.println("1");
        key = 1;
      break;
      case 0xFF629D:
        Serial.println("2");
        key = 2;
      break;
      case 0xFFE21D:
        Serial.println("3");
        key = 3;
      break;
      case 0xFF22DD:
        Serial.println("4");
        key = 4;
      break;
      case 0xFF02FD:
      Serial.println("5");
        key = 5;
      break ;  
      case 0xFFC23D:
      Serial.println("6");
        key = 6;
      break ;               
      case 0xFFE01F:
      Serial.println("7");
        key = 7;
      break ;  
      case 0xFFA857:
      Serial.println("8");
        key = 8;
      break ;  
      case 0xFF906F:
        Serial.println("9");
        key = 9;
      break ;  
      case 0xFF6897:
        Serial.println("*");
        key = 10;
      break ;  
      case 0xFF9867:
        Serial.println("0");
        key = 0;
        break;
      case 0xFFB04F:
        Serial.println("#");
        key = 11;
      break ;
      case 0x52A3D41F:
        key = 'l';
        break ;
      case 0x511DBB:
        key = 'f';
        break ;
      case 0xA3C8EDDB:
        key = 'b';
        break ;
      case 0x20FE4DBB:
        key = 'r';
        break ;
      case 0xD7E84B1B:
        key = 'k';
        break ;
    default:
        case UNKNOWN: Serial.println("UNKNOWN"); break ;
    }
    
    lastKey = key;
    
    return key;
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

