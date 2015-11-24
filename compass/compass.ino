#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_HMC5883_U.h>

#include <LiquidCrystal.h>
LiquidCrystal lcd(7, 8, 9, 10, 11, 12);

/* Assign a unique ID to this sensor at the same time */
Adafruit_HMC5883_Unified mag = Adafruit_HMC5883_Unified(12345);

void setup() {
  lcd.begin(20, 4);
  
  /* Initialise the sensor */
  if(!mag.begin())
  {
    /* There was a problem detecting the HMC5883 ... check your connections */
    lcd.setCursor(0,3);
    lcd.print("No HMC5883 detected");
    while(1);
  }
}

void loop() {
  sensors_event_t event; 
  mag.getEvent(&event);
  
  lcd.setCursor(0,0);
  lcd.print("X:");
  //lcd.setCursor(2,0);
  lcd.print(event.magnetic.x);
  
  lcd.setCursor(0,1);
  lcd.print("Y:");
  //lcd.setCursor(2,1);
  lcd.print(event.magnetic.y);
  
  lcd.setCursor(0,2);
  lcd.print("Z:");
  //lcd.setCursor(2,2);
  lcd.print(event.magnetic.z);
  
    // Hold the module so that Z is pointing 'up' and you can measure the heading with x&y
  // Calculate heading when the magnetometer is level, then correct for signs of axis.
  float heading = atan2(event.magnetic.y, event.magnetic.x);
  
  // Once you have your heading, you must then add your 'Declination Angle', which is the 'Error' of the magnetic field in your location.
  // Find yours here: http://www.magnetic-declination.com/
  // Mine is: -13* 2' W, which is ~13 Degrees, or (which we need) 0.22 radians
  // If you cannot find your Declination, comment out these two lines, your compass will be slightly off.
  float declinationAngle = 0.11;
  heading += declinationAngle;
  
  // Correct for when signs are reversed.
  if(heading < 0)
    heading += 2*PI;
    
  // Check for wrap due to addition of declination.
  if(heading > 2*PI)
    heading -= 2*PI;
   
  // Convert radians to degrees for readability.
  float headingDegrees = heading * 180/M_PI; 
  
  lcd.setCursor(0,3);
  lcd.print("   ");
  lcd.setCursor(0,3);
  lcd.print((int)headingDegrees);
  
  delay(100);
}
