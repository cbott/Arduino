#include <Servo.h>
const int LED = 8;
const int POT = 0;
Servo head;
int pos = 0;
int time = 0;

void setup(){
 pinMode(LED, OUTPUT); 
 head.attach(9);
}

void loop(){
 int potVal = 0.2*  analogRead(POT);
 time += potVal;
 if (potVal > 1){
   digitalWrite(LED, HIGH);
 }
 delay(potVal);
 digitalWrite(LED, LOW);
 delay(potVal);
 if (time > 2000){
   time = 0;
   head.write(pos);
   pos = 90-pos;
 }
}
