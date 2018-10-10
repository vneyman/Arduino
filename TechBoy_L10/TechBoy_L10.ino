int redPin = 9;
int yellowPin = 10;

int potPin = 5;
int potValue = 0;
const float maxVolts = 5.0;

int analogReadToWrite(int readValue){
  float writeValue = readValue*255.0/1023.0;
  int outputValue = (int)round(writeValue);
  
  Serial.print("Analog output: ");
  Serial.println(outputValue);
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


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  
  pinMode(redPin, OUTPUT);
  pinMode(yellowPin, OUTPUT);
  pinMode(potPin, INPUT);
  
  while(!Serial){}
}

void loop() {
  // put your main code here, to run repeatedly:
  int inputValue = readPotentiometer();
  blinkPins(redPin, yellowPin, inputValue);
}
