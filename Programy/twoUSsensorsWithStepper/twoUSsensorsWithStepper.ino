#include <AccelStepper.h>
#include <math.h>
#define motorInterfaceType 1

const int potPin = A0;

const int echoPin1 = 2;
const int trigPin1 = 1;

const int echoPin2 = 12; //->12
const int trigPin2 = 13; //->13

const int dirPin = 8;
const int stepPin = 9;
const int stepsForRevolution = 200;
const int multiplier = 16;

float toleranceForSensors = 0.2;

int timeJump = 3000;
unsigned long timeInterval = timeJump;

int potVal = 0;

AccelStepper myStepper(motorInterfaceType, stepPin, dirPin);

float distLast1 = 0;
float distLast2 = 5;
float distLast3 = 7;

void setup() {
  pinMode(echoPin1, INPUT);
  pinMode(trigPin1, OUTPUT);

  pinMode(echoPin2, INPUT);
  pinMode(trigPin2, OUTPUT);

  myStepper.setMaxSpeed(multiplier*200);
	myStepper.setAcceleration(multiplier*50);
	myStepper.setSpeed(multiplier*200);
	myStepper.moveTo(multiplier*100);
  
  pinMode(potPin, INPUT);

  Serial.begin(9600);
}

void loop() {

  delay(5);
  float dist1 = getDistance(trigPin1, echoPin1);
  delay(5);
  float dist2 = getDistance(trigPin2, echoPin2);

  int val = map(potVal, 0, 1023, 0, 200);
  potVal = analogRead(potPin);

  myStepper.setMaxSpeed(multiplier*val);

	if (myStepper.distanceToGo() == 0) {
		myStepper.moveTo(-myStepper.currentPosition());}


//-------------------------------------------------------------------
// tu cos mu nie pasuje z ta przerwa, bez odliczania przerwy dziala OK
// mozna probowac inaczej robic moze mu sie poprawi
  if(millis() >= timeInterval){
    Serial.print(dist1);
    Serial.print(",");
    Serial.println(dist2);

    distLast3 = distLast2;
    distLast2 = distLast1;
    distLast1 = abs(dist1-dist2);
    if((distLast1 <=toleranceForSensors)&&(distLast2 <=toleranceForSensors)&&(distLast3 <=toleranceForSensors)){
      myStepper.stop();
      Serial.println("dziala");
       }

    timeInterval += timeJump;
  }
  //-------------------------------------------------

	myStepper.run();

}

float getDistance(int trigPin, int echoPin){
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(20);
  digitalWrite(trigPin, LOW);

  float duration = pulseIn(echoPin, HIGH);
  float distance =  duration * 0.034 / 2;
  return distance;
}


// void getDistanceFromTwoSensors(int trigPin1, int echoPin1,int trigPin2, int echoPin2){
//   digitalWrite(trigPin1, LOW);
//   digitalWrite(trigPin2, LOW);
//   delayMicroseconds(2);
//   digitalWrite(trigPin1, HIGH);
//   digitalWrite(trigPin2, HIGH);
//   delayMicroseconds(10);
//   digitalWrite(trigPin1, LOW);
//   digitalWrite(trigPin2, LOW);

//   float duration1 = pulseIn(echoPin1, HIGH);
//   float duration2 = pulseIn(echoPin2, HIGH);

//   float dist1 = (duration1* 0.0343)/2;
//   float dist2 = (duration2* 0.0343)/2;

//   Serial.print(dist1);
//   Serial.print(",");
//   Serial.println(dist2);
// }