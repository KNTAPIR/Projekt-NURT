// Include the AccelStepper Library
#include <AccelStepper.h>

// Define pin connections
const int dirPin = 8;
const int stepPin = 9;
const int stepsForRevolution = 200;
const int multiplier = 16;

const int potPin = A0;

int potVal = 0;

// Define motor interface type
#define motorInterfaceType 1

// Creates an instance
AccelStepper myStepper(motorInterfaceType, stepPin, dirPin);

void setup() {
	myStepper.setMaxSpeed(multiplier*200);
	myStepper.setAcceleration(multiplier*50);
	myStepper.setSpeed(multiplier*200);
	myStepper.moveTo(multiplier*100);
  pinMode(potPin, INPUT);
}

void loop() {
  int val = map(potVal, 0, 1023, 0, 200);
  potVal = analogRead(potPin);
	// Change direction once the motor reaches target position
  myStepper.setMaxSpeed(multiplier*val);
	if (myStepper.distanceToGo() == 0) 
		myStepper.moveTo(-myStepper.currentPosition());

	// Move the motor one step
	myStepper.run();
}