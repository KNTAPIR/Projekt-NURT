const int trigPin1 = 4;
const int echoPin1 = 2;

const int trigPin2 = 13;
const int echoPin2 = 12;

const int freq = 1000; //delay in milliseconds

String dataLabel1 = "UltrasonicSensor1";
String dataLabel2 = "UltrasonicSensor2";
bool label = true;

void setup() {

  pinMode(trigPin1, OUTPUT);
  pinMode(echoPin1, INPUT);
  pinMode(trigPin2, OUTPUT);
  pinMode(echoPin2, INPUT);
  Serial.begin(9600);
}

void loop() {
  while(label){ //runs once
        Serial.print(dataLabel1);
        Serial.print(",");
        Serial.println(dataLabel2);
        label=false;
  }

  float distFromSensor1 = getDistanceFromUS(trigPin1, echoPin1);
  float distFromSensor2 = getDistanceFromUS(trigPin2, echoPin2);
  Serial.print(distFromSensor1);
  Serial.print(",");
  Serial.print(distFromSensor2);
  Serial.print("\n");
  delay(freq);
}

float getDistanceFromUS(int trigPin, int echoPin){
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  float duration = pulseIn(echoPin, HIGH);
  float distance = (duration*0.0343)/2;
  return distance;
}