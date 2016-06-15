// include the library code:
#include <LiquidCrystal.h>

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(7, 8, 9, 10, 11, 12);


void setup()
{
  // set up the LCD's number of columns and rows:
  lcd.begin(20, 4);
  // initialize serial communications
  Serial.begin(9600); 
}

void loop()
{
  int sensor, inches, x;
  
  // read the analog output of the EZ1 from analog input 0
  sensor = analogRead(0);
  
  // convert the sensor reading to inches
  inches = sensor / 2;
  
  // print out the decimal result
  Serial.print(inches,DEC);
  
  // print out a graphic representation of the result
  Serial.print(" ");
  for (x=0;x<(inches/5);x++)
  {
    Serial.print(".");
  }
  Serial.println("|");

  lcd.setCursor(0,0);
  lcd.print("    ");
  lcd.setCursor(0,0);
  lcd.print(inches);
  // pause before taking the next reading
  delay(100);                     
}

