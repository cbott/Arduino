#include <Wire.h>
#include "BMA250.h"

BMA250 accel;

void setup()
{
  Serial.begin(115200);
  Wire.begin();
  accel.begin(BMA250_range_2g, BMA250_update_time_32ms);
}

void loop()
{
  accel.read();
  //Print out the accelerometer data
  Serial.print("x: ");
  Serial.print(accel.X);
  Serial.print(", y: ");
  Serial.print(accel.Y);
  Serial.print(", z:");
  Serial.print(accel.Z);
  Serial.print(",  t: ");   
  Serial.print(accel.rawTemp/2 + 24);
  Serial.println("degC");    
  
  delay(1000);
}