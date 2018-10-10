const int pinLDR = A0;
const int pinLED = 5;

int valueLDR;
boolean stateLED = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  
  pinMode(pinLDR, INPUT);
  pinMode(pinLED, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  valueLDR = analogRead(pinLDR);  

  if(valueLDR > 300){
    stateLED=1;
    }else{
      stateLED=0;
      }

  digitalWrite(pinLED, stateLED);
}
