#include <string.h>
#include <AccelStepper.h>

#define motorInterfaceType 1

const int dirPin = 9;
const int stepPin = 8;
const int multiplier = 16;
const int stepsForRevolution = 200*multiplier;
int wasser = 1;

AccelStepper myStepper(motorInterfaceType, stepPin, dirPin);

int myTimeInterval = 2000;
unsigned long myTime = 0;
unsigned long myDesiredTime = myTimeInterval;


void setup() {

  pinMode(dirPin, OUTPUT);
  pinMode(stepPin, OUTPUT);

  myStepper.setMaxSpeed(multiplier*200);
	myStepper.setAcceleration(multiplier*50);
	myStepper.setSpeed(multiplier*200);
	myStepper.moveTo(multiplier*100);

  pinMode(LED_BUILTIN, OUTPUT);      // set LED pin as output

  digitalWrite(LED_BUILTIN, LOW);    // switch off LED pin

  Serial.begin(9600);            // initialize UART with baud rate of 9600

}

void loop() {

  if (myStepper.distanceToGo() == 0) {
    myStepper.run();
		myStepper.moveTo(-myStepper.currentPosition());}

  myTime = millis();
  
  if(myTime == myDesiredTime){
      wasser *= -1;
       // switch LED On
    // for(int i=0; i<1000; i++){
    //   digitalWrite(LED_BUILTIN, HIGH);
    // }

      char receivedData = Serial.read();
   // while (Serial.available() >= 0) {

      
      //char receivedData = Serial.read();   // read one byte from serial buffer and save to receivedData
      
      if (receivedData == '2') {
        
        myStepper.setSpeed(multiplier*200);

      }
      else if(receivedData == '1'){

        myStepper.setSpeed(0);
       

      }

    myTime = millis();
    
    
    myDesiredTime = myTime + myTimeInterval;

  }


  

  if(wasser == 1){
    digitalWrite(LED_BUILTIN, LOW);
  }
  else{
    digitalWrite(LED_BUILTIN, HIGH);
  }

  myStepper.run();

}