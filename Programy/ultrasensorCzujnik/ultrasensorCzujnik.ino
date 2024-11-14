
const int echoPin1 = 2;
const int trigPin1 = 1;

void setup() {
  pinMode(echoPin1, INPUT);
  pinMode(trigPin1, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  Serial.println(getDistance(trigPin1,echoPin1));
}

float getDistance(int trigPin, int echoPin){
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  float duration = pulseIn(echoPin, HIGH);
  float distance =  duration * 0.034 / 2;
  return distance;
}