#include "Arduino.h"
#include "sn754410.h"

sn754410::sn754410(int m1a, int m1b, int m2a, int m2b){
  pinMode(m1a, OUTPUT); pinMode(m1b, OUTPUT);
  pinMode(m2a, OUTPUT); pinMode(m2b, OUTPUT);
  _m1a = m1a; _m1b = m1b; 
  _m2a = m2a; _m2b = m2b;  
}

void sn754410::set(float left, float right){
  //Takes a value from -1 to 1 and sets the output from 0 to 255 in the correct direction
  digitalWrite(_m1a, LOW); digitalWrite(_m1b, LOW);
  digitalWrite(_m2a, LOW); digitalWrite(_m2b, LOW);
  float leftmap = 255.0 * abs(constrain(left, -1, 1));
  float rightmap = 255.0 * abs(constrain(right, -1, 1));
  if(left > 0){
     analogWrite(_m1a, leftmap);
     digitalWrite(_m1b, LOW);
  } else if (left < 0){
     digitalWrite(_m1a, LOW);
     analogWrite(_m1b, leftmap);
  } else {
     digitalWrite(_m1a, LOW);
     digitalWrite(_m1b, LOW);
  }
  if(right > 0){
     analogWrite(_m2a, rightmap);
     digitalWrite(_m2b, LOW);
  } else if (right < 0){
     digitalWrite(_m2a, LOW);
     analogWrite(_m2b, rightmap);
  } else {
     digitalWrite(_m2a, LOW);
     digitalWrite(_m2b, LOW);
  }
}

void sn754410::test(){
  Serial.println("Forward");
  digitalWrite(_m1a, HIGH);
  digitalWrite(_m1b, LOW);
  digitalWrite(_m2a, HIGH);
  digitalWrite(_m2b, LOW);
  delay(2000);
  
  Serial.println("Reverse");
  digitalWrite(_m1a, LOW);
  digitalWrite(_m1b, HIGH);
  digitalWrite(_m2a, LOW);
  digitalWrite(_m2b, HIGH);
  delay(2000);
  
  Serial.println("Left");
  digitalWrite(_m1a, HIGH);
  digitalWrite(_m1b, LOW);
  digitalWrite(_m2a, LOW);
  digitalWrite(_m2b, HIGH);
  delay(2000);
  
  Serial.println("Right");
  digitalWrite(_m1a, LOW);
  digitalWrite(_m1b, HIGH);
  digitalWrite(_m2a, HIGH);
  digitalWrite(_m2b, LOW);
  delay(2000);
  
  Serial.println("Stop");
  digitalWrite(_m1a, LOW);
  digitalWrite(_m1b, LOW);
  digitalWrite(_m2a, LOW);
  digitalWrite(_m2b, LOW);
  delay(2000);
}
