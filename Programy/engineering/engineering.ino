// Include the AccelStepper Library
#include <AccelStepper.h>

// Define pin connections
const int dirPin = 8;
const int stepPin = 9;
const int stepsForRevolution = 200;
const int multiplier = 16;
const int speed = 15*4;

const byte BUTTON_PIN_START = 6;

// Define motor interface type
#define motorInterfaceType 1

// Creates an instance
AccelStepper myStepper(motorInterfaceType, stepPin, dirPin);

void setup() {
	myStepper.setMaxSpeed(speed*200);
	myStepper.setAcceleration(multiplier*5);
	myStepper.setSpeed(multiplier*speed);
	myStepper.moveTo(multiplier*100);
	pinMode(BUTTON_PIN_START, INPUT);
}

void loop() {

	if (myStepper.distanceToGo() == 0) 
		myStepper.moveTo(-myStepper.currentPosition());

	// Move the motor one step
	if(digitalRead(BUTTON_PIN_START) == LOW)
		myStepper.run();
}