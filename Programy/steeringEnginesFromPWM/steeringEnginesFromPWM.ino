// Define pin connections
const int dirPin = 8;
const int stepPin = 9;
const int stepsForRevolution = 200;
const int multiplier = 16;

 
//timer isr
ISR(TIMER1_OVF_vect)
{
//   TCNT1 = 64935; // Timer Preloading
  // Handle The 2.4ms Timer Interrupt
//   Serial.println("TURCJAAAA");



}

// 
void setup()
{

    //pins configuration
    pinMode(stepPin, OUTPUT);
    pinMode(dirPin, OUTPUT);

    //timer configuration
    TCCR1A = 0;           // Init Timer1
    TCCR1B = 0;           // Init Timer1
    TCCR1B |= B00000011;  // Prescalar = 64
    TCNT1 = 64935;        // Timer Preloading
    TIMSK1 |= B00000001;  // Enable Timer Overflow Interrupt
  
    //registers
    Serial.begin(9600);
    
    //output 
}
void loop()
{
  if(TCNT1 >= 2000)
  {
    digitalWrite(stepPin, HIGH);
    if(TCNT1 >= 2100)
    {
        digitalWrite(stepPin, LOW);
        Serial.println("CHIN");
    }
    TCNT1 = 0;
  }
  Serial.println(TCNT1);
}
