/*
Test for buttons stuff
 */

const int BAUD_RATE= 9600;
const int LED= 13;
const int BUTTON= 7;

void setup() {                
  // initialize stuff.
  Serial.begin(BAUD_RATE);
  pinMode(LED, OUTPUT);
  pinMode(BUTTON, INPUT);
  //pinMode(12, OUTPUT);
}


void loop() {
  Serial.println("hello world");
  int BUTTON_STATE= digitalRead(BUTTON);
  int old_button_state = LOW;
  if (BUTTON_STATE == HIGH && old_button_state==LOW){
    digitalWrite(LED, HIGH);
    delay(50);              // wait for stabilization
  }
  if (BUTTON_STATE == LOW){
    digitalWrite(LED, LOW);
    delay(50);
  }
  old_button_state = BUTTON_STATE;
}


