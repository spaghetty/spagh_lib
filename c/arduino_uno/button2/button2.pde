/*
This is a little game

how to play:

press button HIGH or LOW

then system will generate a random number between 1 and 6 

1-3 : is low
4-6 : is high

if you hit the guess you win.

schema
connect led to digital pin 13
connect button HIGH to pin 7
connect button LOW to pin 4
 */

const int BAUD_RATE= 9600;
const int LED= 13;
const int BUTTON_HIGH= 7;
const int BUTTON_LOW= 4;

void setup() {                
  // initialize stuff.
  Serial.begin(BAUD_RATE);
  randomSeed(analogRead(0));
  pinMode(LED, OUTPUT);
  pinMode(BUTTON_LOW, INPUT);
  pinMode(BUTTON_HIGH, INPUT);
  //pinMode(12, OUTPUT);
}

int check_button_high(){
  int BUTTON_STATE= digitalRead(BUTTON_HIGH);
  static int old_button_state = LOW;
  int retr = LOW;
  if (BUTTON_STATE == HIGH && old_button_state==LOW){
    retr =  HIGH;
  }
  old_button_state = BUTTON_STATE;
  return retr;
}

int check_button_low(){
  int BUTTON_STATE= digitalRead(BUTTON_LOW);
  static int old_button_state = LOW;
  int retr = LOW;
  if (BUTTON_STATE == HIGH && old_button_state==LOW){
    retr =  HIGH;
  }
  old_button_state = BUTTON_STATE;
  return retr;
}

void loop() {
  Serial.println("hello world");
  int high_button = check_button_high();
  int low_button = check_button_low();
  int choose = -1;
  long hit = -1;
  if(high_button==HIGH)
    choose = HIGH;
  
  if(low_button==HIGH)
    choose = LOW;
   
  if( choose != -1){
    hit = random(1,7);
    Serial.print("the result was: ");
    Serial.print(choose);
    Serial.println(hit); 
    if((hit > 3 && choose == HIGH)||
       (hit < 4 && choose == LOW )){
         digitalWrite(LED, HIGH);
         delay(2000);
         digitalWrite(LED,LOW);
       }
   }
   delay(100);              // wait for stabilization
}


