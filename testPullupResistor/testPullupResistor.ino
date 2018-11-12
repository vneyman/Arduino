const int _pinLed = 9;
const int _pinSensor = 2;
int _ledStatus = LOW;

//Circuit explanation: https://www.instructables.com/id/Understanding-the-Pull-up-Resistor-With-Arduino/
//        +   Gnd
//        |   10k resistor
//        |   |-->Vout to arduino pin
//        |   |
//  btn( Vcc Gnd )
//defaults to low; when pressed will be high
//
//Can avoid using an external pull-up or pull-down resistors, 
//simply by using the internal existing pull-up resistor within Arduino. 
//This may make life easier when we use pushbuttons or external sensors.
//In void setup() {} add digitalWrite(sensorPin1,HIGH);
//Internally you set a pull up resistor to the input pin. 
//The state is HIGH when the button in not pressed, and LOW when it is pressed.
//https://www.instructables.com/id/Working-Without-a-Pull-up-Pull-down-Resistor-With-/

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(_pinLed, OUTPUT);
  pinMode(_pinSensor, INPUT);

  while(!Serial){}

  digitalWrite(_pinLed, HIGH);
  delay(250);
  digitalWrite(_pinLed, LOW);
}

void loop() {
  // put your main code here, to run repeatedly:
  int sensorValue = digitalRead(_pinSensor);
  //Serial.println(sensorValue);
  //delay(500);
  
  if(sensorValue == HIGH){
    if(_ledStatus == LOW){
      digitalWrite(_pinLed, HIGH);
      _ledStatus = HIGH;
    }
    }else{
      if(_ledStatus == HIGH){
        digitalWrite(_pinLed, LOW);
        _ledStatus = LOW;
      }
    }
    
}
