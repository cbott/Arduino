#include <Wire.h>
#include "BMA250.h"


BMA250 accel;


void setup()
{
  Serial.begin(9600);
  Wire.begin();
  accel.begin(BMA250_range_2g, BMA250_update_time_64ms);//This sets up the BMA250 accelerometer
}

void loop()
{
  accel.read();//This function gets new data from the accelerometer
  Serial.print("X = ");
  Serial.print(accel.X);
  Serial.print("  ");
  Serial.print("Y = ");
  Serial.print(accel.Y);
  Serial.print("  ");
  Serial.print("Z = ");
  Serial.print(accel.Z);
  Serial.print("  Temperature(C) = ");
  Serial.println((accel.rawTemp*0.5)+24.0,1);
  delay(250);//We'll make sure we're over the 64ms update time set on the BMA250
}

//#include <Wire.h>
//#include "BMA250.h"
//
//BMA250 accel;
//
//const int TOLERANCE = 25;
//
//const int midPin = 8;
//const int topPin = 9;
//const int botPin = 10;
//const int lPin = 11;
//const int rPin = 12;
//
//void setup()
//{
//  Serial.begin(115200);
//  Wire.begin();
//  accel.begin(BMA250_range_2g, BMA250_update_time_32ms);
//  
//  pinMode(midPin, OUTPUT);
//  pinMode(topPin, OUTPUT);
//  pinMode(botPin, OUTPUT);
//  pinMode(lPin, OUTPUT);
//  pinMode(rPin, OUTPUT);
//}
//
//void loop()
//{
//  accel.read();
//  
//  float x = accel.X;
//  float y = accel.Y;
//  float z = accel.Z;
//  boolean y_zero = false;
//  
//  digitalWrite(midPin, LOW);
//  digitalWrite(topPin, LOW);
//  digitalWrite(botPin, LOW);
//  digitalWrite(lPin, LOW);
//  digitalWrite(rPin, LOW);
//  if (y < -TOLERANCE){
//   digitalWrite(topPin, HIGH); 
//  }
//  else if (y > TOLERANCE){
//   digitalWrite(botPin, HIGH); 
//  }else{
//   y_zero = true;
//  }
//  if (x > TOLERANCE){
//   digitalWrite(lPin, HIGH); 
//  }else if (x < -TOLERANCE){
//   digitalWrite(rPin, HIGH); 
//  }else if(y_zero){
//   digitalWrite(midPin, HIGH); 
//  }
//  //Print out the accelerometer data
//  Serial.print("x: ");
//  Serial.print(x);
//  Serial.print(", y: ");
//  Serial.print(y);
//  Serial.print(", z:");
//  Serial.print(z);
//  Serial.print(",  t: ");   
//  Serial.print(accel.rawTemp/2 + 24);
//  Serial.println("degC");    
//  
//  delay(50);
//}
