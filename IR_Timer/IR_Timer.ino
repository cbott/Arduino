#include <LiquidCrystal.h>

LiquidCrystal lcd(7, 8, 9, 10, 11, 12);

const int THRESHHOLD = 60;

const int START_PIN = A0;
const int END_PIN = A1;

unsigned long starttime = 0;
unsigned long endtime = 0;
bool timing = false;

void setup() {
  //Serial.begin(9600);
  lcd.begin(20, 4);
  lcd.setCursor(0,0);
  lcd.print("Timer");
}

void loop() {
  int read1 = analogRead(START_PIN);
  int read2 = analogRead(END_PIN);
  /*
  Serial.print("Detector 1: ");
  Serial.print(read1);
  Serial.print("  Detector 2: ");
  Serial.println(read2);
  */
  
  if(read1 < THRESHHOLD && !timing){
    starttime = millis();
    //Serial.println("Starting!!!");
    lcd.setCursor(0,0);
    lcd.print("Timing...    ");
    lcd.setCursor(0,2);
    lcd.print("                    ");
    timing = true;
  }
  else if(read2 < THRESHHOLD && timing){
    endtime = millis();
    //Serial.print("FINISH: ");
    lcd.setCursor(0,0);
    lcd.print("Timer Stopped");
    //Serial.print(endtime - starttime);
    //Serial.println("ms");
    timing = false;
  }
  if(timing){
    //Serial.println(millis()-starttime);
    lcd.setCursor(0,1);
    lcd.print("Elased time(ms):");
    lcd.setCursor(0,2);
    lcd.print(millis()-starttime);
  }
  lcd.setCursor(0,3);
  lcd.print(read2);
  delay(5);
}
