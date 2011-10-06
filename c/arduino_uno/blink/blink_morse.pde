/*
  Blink
  Turns on an LED on for one second, then off for one second, repeatedly.
 
  This example code is in the public domain.
 */

typedef void (* morsecode) ();

const int INTER_WORD= 300;
const int BAUD_RATE= 9600;
const char name[] = "DomeNi-co";

const morsecode mychars[] = {
    a,
    b, 
    c, 
    d, 
    e, 
    f, 
    g, 
    h,
    i,
    j,
    k, 
    l, 
    m, 
    n, 
    o, 
    p, 
    q, 
    r, 
    s, 
    t, 
    u, 
    v,
    w,
    x,
    y, 
    z };


void setup() {                
  // initialize the digital pin as an output.
  // Pin 13 has an LED connected on most Arduino boards:
  Serial.begin(BAUD_RATE);
  pinMode(13, OUTPUT);
  pinMode(12, OUTPUT);
}

void line() {
    Serial.print("-");
    digitalWrite(13, HIGH);
    delay(1000);
    digitalWrite(13, LOW);
    delay(300);
}
  
void dot(){
  Serial.print(".");
  digitalWrite(13, HIGH);
  delay(400);
  digitalWrite(13,LOW);
  delay(300);
}

void stopword(){
  digitalWrite(12,HIGH);
  delay(700);
  digitalWrite(12,LOW);
}

void spell_name(const char n[]){
  // n[0]-97
  int i = 0;
  while(n[i]!='\0'){
    int elem = n[i];
    if (elem>=65 && elem<=90) // manage upper case char
      elem = elem+32;
    if (elem >= 97 && elem <=122)
      mychars[elem-97]();
    else
      Serial.println("this char isn't alphanumeric");
    i++;
  }
}

void loop() {
  Serial.println("hello world");
  spell_name(name);
  //delay(3000);              // wait for a second
  stopword();                 // use stop word led
}

void a(){
   dot();
   line();
   delay(INTER_WORD);
   Serial.print("a\n");
}

void b(){
   line();
   dot();
   dot();
   dot();
   delay(INTER_WORD);
   Serial.print("b\n");
   
}

void c(){
  line();
  dot();
  line();
  dot();
  delay(INTER_WORD);
  Serial.print("c\n");
}

void d()
{
   line();
   dot();
   dot();
   delay(INTER_WORD);
   Serial.print("d\n");
}

void e(){
  dot();
  delay(INTER_WORD);
  Serial.print("e\n");
}

void f(){
  dot();
  dot();
  line();
  dot();
  delay(INTER_WORD);
  Serial.print("f\n");
}

void g(){
  line();
  line();
  dot();
  delay(INTER_WORD);
  Serial.print("g\n");
}

void h(){
  dot();
  dot();
  dot();
  dot();
  delay(INTER_WORD);
  Serial.print("h\n");
}

void i(){
  dot();
  dot();
  delay(INTER_WORD);
  Serial.print("i\n");
}

void j(){
  dot();
  line();
  line();
  line();
  delay(INTER_WORD);
  Serial.print("j\n");
}

void k(){
  line();
  dot();
  line();
  delay(INTER_WORD);
  Serial.print("k\n");
}

void l(){
  dot();
  line();
  dot();
  dot();
  delay(INTER_WORD);
  Serial.print("l\n");
}

void m(){
  line();
  line();
  delay(INTER_WORD);
  Serial.print("m\n");
}

void n(){
  line();
  dot();
  delay(INTER_WORD);
  Serial.print("n\n");
}

void o(){
  line();
  line();
  line();
  delay(INTER_WORD);
  Serial.print("o\n");
}

void p(){
  dot();
  line();
  line();
  dot();
  delay(INTER_WORD);
  Serial.print("p\n");
}

void q(){
  line();
  line();
  dot();
  line();
  delay(INTER_WORD);
  Serial.print("q\n");
}

void r(){
  dot();
  line();
  dot();
  delay(INTER_WORD);
  Serial.print("r\n");
}

void s(){
  dot();
  dot();
  dot();
  delay(INTER_WORD);
  Serial.print("s\n");
}

void t(){
  line();
  delay(INTER_WORD);
  Serial.print("t\n");
}

void u(){
  dot();
  dot();
  line();
  delay(INTER_WORD);
  Serial.print("u\n");
}

void v(){
  dot();
  dot();
  dot();
  line();
  delay(INTER_WORD);
  Serial.print("v\n");
}

void w(){
  dot();
  line();
  line();
  delay(INTER_WORD);
  Serial.print("w\n");
}

void x(){
  line();
  dot();
  dot();
  line();
  delay(INTER_WORD);
  Serial.print("x\n");
}

void y(){
  line();
  dot();
  line();
  line();
  delay(INTER_WORD);
  Serial.print("y\n");
}

void z(){
  line();
  line();
  dot();
  dot();
  delay(INTER_WORD);
  Serial.print("z\n");
}

