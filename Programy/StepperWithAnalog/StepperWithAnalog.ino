#include <LiquidCrystal.h>
#include <TMC2209.h>
#include <AccelStepper.h>
#include <math.h>

/*-------------------------------------
  Code for the movement of the 


-------------------------------------*/
// gear 4:1, microstepping 1/16


const byte BUTTON_PIN_START = 6;
// declaration and init of dist sensor pins
const byte TRIG_PIN = 5;
const byte ECHO_PIN = 4;

// declaration and init of rotation stepper pins
const byte STEPPER_X_DIR_PIN = 8;
const byte STEPPER_X_STEP_PIN = 9;
const byte STEPPER_X_EN_PIN = 13;

//declaration and init of linear stepper pins
// const byte STEPPER_Y_DIR_PIN = 6;
// const byte STEPPER_Y_STEP_PIN = 7;

//pins for lcd display
const byte LCD_RS = 12;
const byte LCD_EN = 11;

// multiplier of steps instructions, due to usage of microstepping
const int microstepMultiplier = 16;

// speeds value declarations for various modes
const int SPEED_SEARCH_CENTRE = 6.5;
const int SPEED_SEARCH_GLASS = 15;
const int SPEED_NORMAL = 35;

const int STEPS_TO_CIRC = 200;

// when sensor is "supposed" to see anything, ie object needs to be on the dist lower than that
const float WORK_DISTANCE = 35.0f;

// distance read by sensor
float distanceReadBySensor = 15.0f;

// array used to store last measured distances from sensors, first element is the newest
float lastDistances[5] = {99999,99999,99999,99999,99999};

// helper variables used to find number of steps to do in order to move the robot arm from the currently used glass
float glassCentreDistance = 0;
int stepsAtGlassStart = 0;
int stepsAtGlassCentre = 0;
int offsetToNextGlass = 50;
int leftStepsForCycle = 0;

// bool 
bool makeMeasurments = true;

// constructor for accelstepper object for cicular movement stepper 
AccelStepper STEPPER_X(AccelStepper::DRIVER, STEPPER_X_STEP_PIN, STEPPER_X_DIR_PIN);

// constructor for tmc object
TMC2209 STEPPER_Y;

// lcd constructor (really ugly)
LiquidCrystal lcd(LCD_RS, LCD_EN, A5, A3, A2, A1);

// state machine for the robot circular mode
enum RobotState {
  runNormally,
  searchForGlass,
  searchForCentre,
  pourLiquid,
  moveFromGlass,
  idle
};

RobotState robotMachine = runNormally;

void setup()
{
  // pin setup
  pinMode(LED_BUILTIN, OUTPUT);

  pinMode(LCD_RS, OUTPUT);
  pinMode(LCD_EN, OUTPUT);
  pinMode(A5, OUTPUT);
  pinMode(A3, OUTPUT);
  pinMode(A2, OUTPUT);
  pinMode(A1, OUTPUT);
  
  pinMode(TRIG_PIN, OUTPUT); //Ultrasonic Sensor Pin
  pinMode(ECHO_PIN, INPUT); //Ultrasonic Sensor Pin
  
  pinMode(STEPPER_X_DIR_PIN, OUTPUT); //stepper dir pin
  pinMode(STEPPER_X_STEP_PIN, OUTPUT); //stepper step Pin

  pinMode(STEPPER_X_EN_PIN, INPUT);

  pinMode(BUTTON_PIN_START, INPUT);

  lcd.begin(16, 2);
  lcd.home();
  lcd.print("wielkie chin");
  lcd.display();

  Serial.begin(9600);

  // stepper intial values setup
  STEPPER_X.setMaxSpeed(3000);
  STEPPER_X.setSpeed(SPEED_NORMAL*microstepMultiplier);
  STEPPER_X.moveTo(STEPS_TO_CIRC*microstepMultiplier);

  digitalWrite(STEPPER_X_EN_PIN, LOW);
  digitalWrite(LED_BUILTIN, LOW);
  //move to home position
  
}

void loop()
{
   // slow down the ranging to avoid unwanted echoes
  static unsigned long timer = 0;
  unsigned long interval = 60;


  // make readings from sensors, periodically
  if (millis() - timer >= interval)
  {
    timer = millis();
    
    if(makeMeasurments == true){
      distanceReadBySensor = getDistance(TRIG_PIN, ECHO_PIN);
    }
    // Serial.println(distanceReadBySensor);

    // if (distanceReadBySensor > 50)
    // {
    //     STEPPER_X.setSpeed(50*microstepMultiplier);
    // }

    // else
    // {
    //     STEPPER_X.setSpeed(0);
    // }
  }

  if(digitalRead(BUTTON_PIN_START) == LOW){
    robotMachine = runNormally;
    digitalWrite(LED_BUILTIN, HIGH);
  }
   
  // state machine for circular workmode
  switch (robotMachine) {
      
      // state for the robot in which it moves normally in circular mode [normal speed, less amount of measurments]
      case runNormally:
          digitalWrite(STEPPER_X_EN_PIN, LOW);
          makeMeasurments = true;

          lcd.clear();
          lcd.print("Run normal");
          lcd.print(lastDistances[0]);

          STEPPER_X.setSpeed(SPEED_NORMAL*microstepMultiplier);
          //it should also check if there is place to find next cup
          

          // TODO: add function for measuring the rdaius of the circle when the vessels overlap,
          // possible to do with angle measurments (from the number of steps multiplied by the microstepping ratio)
          // and length of arc (making small dist measurments from 2 sensor reading and converting them into full length of given arc)
          
          if(distanceReadBySensor <= WORK_DISTANCE){
            leftStepsForCycle = STEPPER_X.distanceToGo();
            glassCentreDistance = distanceReadBySensor;
            robotMachine = searchForGlass;
            stepsAtGlassStart = STEPPER_X.currentPosition();
          }
          
          break;

      case searchForGlass:
        digitalWrite(STEPPER_X_EN_PIN, LOW);
        makeMeasurments = true;
        STEPPER_X.setSpeed(SPEED_SEARCH_GLASS*microstepMultiplier);

        lcd.clear();
        lcd.print("Search Glass");
        lcd.print(lastDistances[0]);

        if(lastDistances[0] != distanceReadBySensor){
          updateLastDistances(lastDistances, 5, distanceReadBySensor);
        }

        if(checkIfLastElemLargerThanOther(lastDistances, 5)){
          // normally robotMachine = searchForCentre
          robotMachine = pourLiquid;

        }
          // Placeholder for searching for a glass
          break;

      case searchForCentre:
        lcd.clear();
        lcd.print("Search centre");
        lcd.print(lastDistances[0]);

        digitalWrite(STEPPER_X_EN_PIN, LOW);
        makeMeasurments = true;
        STEPPER_X.setSpeed(SPEED_SEARCH_CENTRE*microstepMultiplier);

        if(!checkIfAllElemsLargerThanLast(lastDistances, 5, 0.05)){
          robotMachine = pourLiquid;
        }
        // Placeholder for searching for the center point
        break;

      case pourLiquid:

        lcd.clear();
        lcd.print("Pour water");


        digitalWrite(STEPPER_X_EN_PIN, HIGH);
        makeMeasurments = false;
        stepsAtGlassCentre = STEPPER_X.currentPosition();
        delay(3000);
        // move linear actuator to the postiong of the centre
        // pour liquid
        // (optional) move linear actuator to the zero postion
        robotMachine = moveFromGlass;
        break;

      // case for the robot moving away from the detected and filled vessel
      case moveFromGlass:

        lcd.clear();
        lcd.print("Move next");


        STEPPER_X.setSpeed(SPEED_NORMAL * microstepMultiplier);
        digitalWrite(STEPPER_X_EN_PIN, LOW);
        makeMeasurments = false;
        STEPPER_X.moveTo(STEPPER_X.currentPosition() + (stepsAtGlassCentre-stepsAtGlassStart)*2 + offsetToNextGlass);
        //moveIlesTam, move aktualtor liniowy na pozycje home (albo i nie pomysli sie)
        if(STEPPER_X.distanceToGo() == 0){
          STEPPER_X.moveTo(STEPS_TO_CIRC*microstepMultiplier);
          robotMachine = runNormally;
        }
        break;

      case idle:
          lcd.clear();
          lcd.print("Idle");
          
          
          STEPPER_X.setSpeed(0);
          digitalWrite(STEPPER_X_EN_PIN, HIGH);
          break;
      default:
          robotMachine = runNormally;
          digitalWrite(STEPPER_X_EN_PIN, HIGH);
          break;

  }

  STEPPER_X.runSpeed();

}

float getDistance(int trigPin, int echoPin){
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);
    long duration = pulseIn(echoPin, HIGH);
    return (float)duration * 0.034 / 2; //Distance value,
}

bool checkIfAllElemsSimillar(float arr[], int size, float tolerance) {
    for(int i=1; i<size; i++){
      
      if(abs(arr[i-1]-arr[i])>tolerance){
        return false;
      }

    }
    return true;
}

bool checkIfLastElemLargerThanOther(float arr[], int size) {
    if(arr[0] > arr[1]){
      return true;
    }
    else{
      return false;
    }
}

bool checkIfAllElemsLargerThanLast(float arr[], int size, float tolerance) {
    for(int i=1; i<size; i++){
      
      
      if(arr[i-1]<arr[i]){
        return false;
      }

    }
    return true;
}

void updateLastDistances(float arr[], int size, float dist){
  for(int i=size-1; i>=1; i--){
    arr[i] = arr[i-1];
  }
  arr[0] = dist;
}

void clearArray(float arr[], int size){
  for(int i=0; i<size; i++){
    arr[i] = 99999;
  }
}


/* ok what to add later:
- second sensor measurments in the search for centre mode
- add linear actuator steppper in the 2 last state machine cases
- turn on saving last distances when in search for glass or search for centre mode, turn off otherwise
- add various extreme conditions to check
- look slowly at the whole code
- add home position to the robot
- add more unit tests
*/
