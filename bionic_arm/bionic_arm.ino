#include <Servo.h>

const int STICK_PIN = A0;
const int VICTOR_PIN = 9;

Servo victor;

void setup(){
  victor.attach(VICTOR_PIN);
  Serial.begin(9600);
}

void loop(){
  //setMotor(getJoystick());
  //delay(1);
  setMotor(-0.35);
  delay(1000);
  setMotor(0);
  delay(100);
  setMotor(0.15);
  delay(900);
  setMotor(0);
  delay(100);
}

void setMotor(float val){
  val = constrain(val, -1.0, 1.0);
  int us = (val * 500) + 1500;
  victor.writeMicroseconds(us);
}

float getJoystick(){
  int raw = analogRead(STICK_PIN);
  float res = constrain((raw - 511) / (-511.0), -1.0, 1.0);

  float sign = 1.0;
  if(res < 0){
    sign = -1.0;
  }

  res = res * res * sign;

  if(abs(res) < 0.05){
    res = 0.0;
  } 
  return res;
}
  