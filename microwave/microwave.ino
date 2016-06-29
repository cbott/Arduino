//microwave.ino
//Simulate a microwave using a piezo speaker and a countdown timer
//Use to prank people into thinking their food is ready when it isn't

#include <LiquidCrystal.h>

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(7, 8, 9, 10, 11, 12);

const int BEEP_PIN = 6;
const int BUTTON_PIN = 5;
const int POT_PIN = 0;
void setup() {
  pinMode(BEEP_PIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT);
  lcd.begin(20, 4);
}

void loop() {
  int t = 0;
  int freq = 2080;
  while(digitalRead(BUTTON_PIN)){
	  int potval = analogRead(POT_PIN);
	  lcd.setCursor(0,0);
	  lcd.print("Pot:");
	  lcd.print(potval);
	  lcd.print("    ");

	  lcd.setCursor(0,1);
	  lcd.print("Hz:");
	  lcd.print(freq);
	  lcd.print("    ");

	  t = map(potval, 0, 1020, 1, 120);
	  lcd.setCursor(0,2);
	  lcd.print("Time:");
	  lcd.print(t);
	  lcd.print("    ");
	  lcd.display();
	  delay(10);
  }
  for(int i=t; i>=0; i--){
	lcd.setCursor(0,2);
	lcd.print("Time:");
	lcd.print(i);
	lcd.print("    ");
	lcd.display();  
	delay(1000);	
  }
  while(1){
	tone(BEEP_PIN, freq);
	delay(500);
	noTone(BEEP_PIN);
	delay(500);
  }
}
