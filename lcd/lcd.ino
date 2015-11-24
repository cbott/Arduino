/*
  LiquidCrystal Library - Hello World

 Demonstrates the use a 16x2 LCD display.  The LiquidCrystal
 library works with all LCD displays that are compatible with the
 Hitachi HD44780 driver. There are many of them out there, and you
 can usually tell them by the 16-pin interface.

 This sketch prints "Hello World!" to the LCD
 and shows the time.

  The circuit:
 * LCD RS pin to digital pin 12
 * LCD Enable pin to digital pin 11
 * LCD D4 pin to digital pin 5
 * LCD D5 pin to digital pin 4
 * LCD D6 pin to digital pin 3
 * LCD D7 pin to digital pin 2
 * LCD R/W pin to ground
 * LCD VSS pin to ground
 * LCD VCC pin to 5V
 * 10K resistor:
 * ends to +5V and ground
 * wiper to LCD VO pin (pin 3)

 Library originally added 18 Apr 2008
 by David A. Mellis
 library modified 5 Jul 2009
 by Limor Fried (http://www.ladyada.net)
 example added 9 Jul 2009
 by Tom Igoe
 modified 22 Nov 2010
 by Tom Igoe

 This example code is in the public domain.

 http://www.arduino.cc/en/Tutorial/LiquidCrystal
 */

// include the library code:
#include <LiquidCrystal.h>

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(7, 8, 9, 10, 11, 12);

byte heart[8] = {
  0b00000,
  0b01010,
  0b11111,
  0b11111,
  0b11111,
  0b01110,
  0b00100,
  0b00000
};
byte check[8] = {
 0b10101,0b1010,0b10101,0b1010,0b10101,0b1010,0b10101 
};


const int NUMFRAMES = 2;
String text1[4] = {"Colin Bott | 2016   ", "GPA: 4.0 (4.0 Scale)", "FRC Robotics 4 years", "Varsity crew 3 years"};
String text2[4] = {"Colin Bott | Testing", "SAT: 2200 | ACT:35  ", " SAT Math II : 800  ", "SAT Chemistry: 800  ",};

String* frames[NUMFRAMES] = {&text1[0], &text2[0]};

void setup() {
  // set up the LCD's number of columns and rows:
  lcd.begin(20, 4);
  
  lcd.createChar(0, check);
}

void loop() {
  for(int frame=0; frame<NUMFRAMES; frame++){
    for( int i = 0; i<4; i++){
      lcd.setCursor(0,i);
      lcd.print(frames[frame][i]);
    }
    delay(10000);
    lcd.noDisplay();
    delay(500);
    lcd.display();
  }
}

