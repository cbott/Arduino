#include "sn754410.h"

sn754410 hbridge(8,9,10,11);

void setup() {
  Serial.begin(9600);
}

void loop() {
  /*
  for(float i = -1; i<1; i+=0.01){
    hbridge.set(i,0.5);
    delay(100); 
  }
  for(float i = 1; i>-1; i-=0.01){
    hbridge.set(i,-0.5);
    delay(100); 
  }
  */
  hbridge.test();
}

