//gps.ino

#include <EEPROM.h>
#include <Encoder.h> 
#include <LiquidCrystal.h>
#include <SoftwareSerial.h>
#include "TinyGPS.h"

const int EEPROM_LAT_ADDRESS = 0;
const int EEPROM_LON_ADDRESS = 4; // sizeof(float) = 4

const byte PIN_IMG[8] = {0b1110,0b10001,0b10101,0b10001,0b1010,0b1010,0b100,0b100};
const byte PERSON_IMG[8] = {0b100,0b1010,0b100,0b1110,0b10101,0b100,0b1010,0b1010};

const int ENCODER_A = 2;
const int ENCODER_B = 3;
const int RED_LED = 5;
const int GREEN_LED = 6;
const int BUTTON_PIN = 7;

LiquidCrystal lcd(8, 9, 10, 11, 12, 13);
Encoder knob(ENCODER_A, ENCODER_B);
TinyGPS gps;

float set_lat, set_lon, cur_lat, cur_lon = 0;
long enc_pos = -999;
bool mode = 0;

void setup() {
  lcd.begin(20, 4);
  lcd.clear();

  lcd.createChar(0, PIN_IMG);
  lcd.createChar(1, PERSON_IMG);

  pinMode(BUTTON_PIN, INPUT);
  pinMode(RED_LED, OUTPUT);
  pinMode(GREEN_LED, OUTPUT);
  digitalWrite(RED_LED, LOW);
  digitalWrite(GREEN_LED, HIGH);

  Serial.begin(9600);

  EEPROM.get(EEPROM_LAT_ADDRESS, set_lat);
  EEPROM.get(EEPROM_LON_ADDRESS, set_lon);
}

void loop() {

  long newpos;
  newpos = knob.read();

  if(newpos != enc_pos) {
    enc_pos = newpos;
    mode = abs(enc_pos/4) % 2;
    lcd.clear();
    digitalWrite(RED_LED, mode);
    digitalWrite(GREEN_LED, !mode);
  }

  char packet;
  bool newData = false;

  // Read in all available serial data from the GPS
  while(Serial.available()){
    packet = Serial.read();
    if(gps.encode(packet)){
      newData = true;
      digitalWrite(RED_LED, HIGH);
      digitalWrite(GREEN_LED, LOW);
    }
  }

  // If we got new info, print it out
  if(newData){
    int year;
    byte month, day, hour, minute, second, hundredths;
    unsigned long fix_age;
    gps.crack_datetime(&year, &month, &day, &hour, &minute, &second, &hundredths, &fix_age);

    float flat, flon;
    unsigned long age;
    gps.f_get_position(&flat, &flon, &age);
    if(flat == TinyGPS::GPS_INVALID_F_ANGLE){
      flat = 0;
    } else {
      cur_lat = flat;
    }
    if(flon == TinyGPS::GPS_INVALID_F_ANGLE){
      flon = 0;
    } else {
      cur_lon = flon;
    }

    switch(mode){
      case 0:
        lcd.setCursor(0,0);
        lcd.write(byte(0));
        lcd.print(" ");
        lcd.print(set_lat, 4);
        lcd.print(",");
        lcd.print(set_lon, 4);

        lcd.setCursor(0,1);
        lcd.write(byte(1));
        lcd.print(" ");
        lcd.print(cur_lat, 4);
        lcd.print(",");
        lcd.print(cur_lon, 4);

        lcd.setCursor(0,3);
        lcd.print("DIST:");
        lcd.print(gps.distance_between(set_lat, set_lon, cur_lat, cur_lon));
        lcd.print("m");
        break;
      case 1:
        lcd.setCursor(0,0);
        lcd.print("LAT:");
        lcd.print(cur_lat, 8);
        lcd.print("   ");

        lcd.setCursor(0,1);
        lcd.print("LON:");
        lcd.print(cur_lon, 8);
        lcd.print("   ");
        
        lcd.setCursor(0,2);
        lcd.print("MPH:");
        lcd.print(gps.f_speed_mph());
        lcd.print("   ");

        lcd.setCursor(0,3);
        lcd.print(hour);
        lcd.print(":");
        char m[3];
        sprintf(m, "%02d", minute);
        lcd.print(m);
        lcd.print(":");
        char s[3];
        sprintf(s, "%02d", second);
        lcd.print(s);
        lcd.print("  ");
        break;
    }
  }
  if(mode == 0 && digitalRead(BUTTON_PIN)){
    lcd.clear();
    lcd.setCursor(0,2);
    set_lat = cur_lat;
    set_lon = cur_lon;
    EEPROM.put(EEPROM_LAT_ADDRESS, set_lat);
    EEPROM.put(EEPROM_LON_ADDRESS, set_lon);
    digitalWrite(RED_LED, LOW);
    digitalWrite(GREEN_LED, HIGH);
  }
}
