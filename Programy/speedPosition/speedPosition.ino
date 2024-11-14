#include <AccelStepper.h>
#include <math.h>
#define motorInterfaceType 1

const int potPin1 = A0;
const int potPin2 = A5;

const int dirPin = 8;
const int stepPin = 9;
const int stepsForRevolution = 200;
const int multiplier = 16;

int potVal1 = 0;
int potVal2 = 0;

AccelStepper myStepper(motorInterfaceType, stepPin, dirPin);


void setup() {
	myStepper.setMaxSpeed(multiplier*200);
	myStepper.setAcceleration(multiplier*50);
	myStepper.setSpeed(multiplier*200);
  pinMode(potPin1, INPUT);
  pinMode(potPin2, INPUT);

  Serial.begin(9600);
}

void loop() {
	
  int speedVal = map(potVal1, 0, 1023, 0, 200);
  int positionVal = map(potVal2, 0, 1023, -200, 300);
  potVal1 = analogRead(potPin1);
  potVal2 = analogRead(potPin2);

  myStepper.setMaxSpeed(multiplier*speedVal);
  myStepper.moveTo(multiplier*positionVal);
	myStepper.run();

}
