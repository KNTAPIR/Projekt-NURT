const int echoPin = 12;
const int trigPin = 13;

const float maxDistance = 30.0;

void setup() {

  pinMode(echoPin, INPUT);
  pinMode(trigPin, OUTPUT);
  Serial.begin(9600);            // initialize UART with baud rate of 9600

}

void loop() {

  if(getDistance(trigPin, echoPin)<maxDistance){
    Serial.println('2');
  }
  else{
    Serial.println('1');
  }

}

float getDistance(int trigPin, int echoPin){
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  float duration = pulseIn(echoPin, HIGH);
  float distance = duration * 0.034 / 2;
  return distance;
}