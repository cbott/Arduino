//gps.ino

#include <LiquidCrystal.h>
#include <SoftwareSerial.h> 
#include "TinyGPS.h"

LiquidCrystal lcd(8, 9, 10, 11, 12, 13);
SoftwareSerial GPS_Serial(3,4); // RX, TX
TinyGPS gps;

void setup() {
  lcd.begin(20, 4);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("LAT:");
  lcd.setCursor(0,1);
  lcd.print("LON:");
  lcd.setCursor(0,2);
  lcd.print("MPH:");

  Serial.begin(9600);
  GPS_Serial.begin(9600); 
  Serial.print("Beginning\n");
}
void loop() {
  char packet;
  bool newData = false;

  // Read in all available serial data from the GPS
  while(GPS_Serial.available()){
    packet = GPS_Serial.read();
    if(gps.encode(packet)) newData = true;
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
    Serial.print("LAT=");
    Serial.print(flat == TinyGPS::GPS_INVALID_F_ANGLE ? 0.0 : flat, 6);
    Serial.print(" LON=");
    Serial.print(flon == TinyGPS::GPS_INVALID_F_ANGLE ? 0.0 : flon, 6);
    Serial.print(" SAT=");
    Serial.print(gps.satellites() == TinyGPS::GPS_INVALID_SATELLITES ? 0 : gps.satellites());
    Serial.print(" PREC=");
    Serial.print(gps.hdop() == TinyGPS::GPS_INVALID_HDOP ? 0 : gps.hdop());
    Serial.print(" ");
    Serial.print(hour);
    Serial.print(":");
    Serial.print(minute);
    Serial.print(":");
    Serial.print(second);
    Serial.print("\n");

    lcd.setCursor(4,0);
    lcd.print(flat, 8);
    lcd.print("   ");
    lcd.setCursor(4,1);
    lcd.print(flon, 8);
    lcd.print("   ");
    lcd.setCursor(4,2);
    lcd.print(gps.f_speed_mph());
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
  }

}

/*
$GPRMC,045159.00,A,4215.68690,N,08342.40503,W,0.046,,271216,,,D*64

$GPVTG,,T,,M,0.046,N,0.085,K,D*29

$GPGGA,045159.00,4215.68690,N,08342.40503,W,2,09,1.09,269.0,M,-34.7,M,,0000*6C

$GPGSA,A,3,11,13,28,30,51,01,46,07,08,,,,2.20,1.09,1.91*00

$GPGSV,3,1,11,01,35,140,35,07,68,168,29,08,51,050,28,11,58,126,35*7F

$GPGSV,3,2,11,13,16,316,27,17,10,227,18,28,42,293,38,30,68,292,35*78

$GPGSV,3,3,11,46,42,202,35,48,21,240,,51,36,213,36*46

$GPGLL,4215.68690,N,08342.40503,W,045159.00,A,D*7E
*/
