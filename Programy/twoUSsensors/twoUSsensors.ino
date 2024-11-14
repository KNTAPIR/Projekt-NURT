const int echoPin1 = 2;
const int trigPin1 = 1;

const int echoPin2 = 12; //->12
const int trigPin2 = 13; //->13

void setup() {
  pinMode(echoPin1, INPUT);
  pinMode(trigPin1, OUTPUT);

  pinMode(echoPin2, INPUT);
  pinMode(trigPin2, OUTPUT);

  Serial.begin(9600);
}

void loop() {
  float dist1 = getDistance(trigPin1, echoPin1);
  delay(10);
  float dist2 = getDistance(trigPin2, echoPin2);

  Serial.print(dist1);
  Serial.print(", ");
  Serial.println(dist2);
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