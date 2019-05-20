//Works with N-Channel Enhancement Mode MOSFET type
//For N-Channel MOSFET, the source is connected to ground. 
//To turn MOSFET on, need to raise the voltage on the gate.
//To turn MOSFET off, need to connect the gate to ground.
//Will stay on after voltage is raised, even if the gate is not connected to load. 
//Need to toggle gate between voltage and ground to open/close the circuit.

/*MOSFET layout. Model number is shown in front

##########
#### #####
###   ####
#### #####
##########
#IRLB8721#
##########
  # # #
  # # #
  G D S

*/

#define IS_TEST
#define ARRAY_SIZE(A) (sizeof(A) / sizeof((A)[0]))

const unsigned int _pinMosfet = 9;

void setup() {
  // put your setup code here, to run once:
  #if defined(IS_TEST)
    pinMode(LED_BUILTIN, OUTPUT);
  #endif

  pinMode(_pinMosfet, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  pinDigitalHighLow(_pinMosfet, HIGH, 1000);
}

void pinDigitalHighLow(unsigned int pin, byte pinStatus, unsigned int delayMillSeconds){
  #if defined(IS_TEST)
    digitalWrite(pin, pinStatus);
    delay(delayMillSeconds);
    digitalWrite(pin, !pinStatus);
    delay(delayMillSeconds);
  #endif

}

void testFunction(){
  #if defined(IS_TEST)
    digitalWrite(LED_BUILTIN, HIGH);
    delay(1000);
    digitalWrite(LED_BUILTIN, LOW);
    delay(1000);
  #endif

}
