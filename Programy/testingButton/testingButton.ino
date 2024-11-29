const byte BUTTON_PIN_START = 6;

void setup() {
  pinMode(BUTTON_PIN_START, INPUT);

  Serial.begin(9600);
}

void loop() {
	
    if(digitalRead(BUTTON_PIN_START) == HIGH){
        Serial.println("Button is high");
    }
    else if(){
        Serial.println("Button is low");
    }
}