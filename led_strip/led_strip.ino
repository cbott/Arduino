#include "LPD8806.h"
#include "SPI.h" // Comment out this line if using Trinket or Gemma
#ifdef __AVR_ATtiny85__
 #include <avr/power.h>
#endif

// Example to control LPD8806-based RGB LED Modules in a strip

/*****************************************************************************/

// Number of RGB LEDs in strand:
int nLEDs = 12;

int buttonPin = 3;
// Chose 2 pins for output; can be any valid output pins:
int dataPin  = 8;
int clockPin = 9;
// First parameter is the number of LEDs in the strand.  The LED strips
// are 32 LEDs per meter but you can extend or cut the strip.  Next two
// parameters are SPI data and clock pins:
LPD8806 strip = LPD8806(nLEDs, dataPin, clockPin);

// You can optionally use hardware SPI for faster writes, just leave out
// the data and clock pin parameters.  But this does limit use to very
// specific pins on the Arduino.  For "classic" Arduinos (Uno, Duemilanove,
// etc.), data = pin 11, clock = pin 13.  For Arduino Mega, data = pin 51,
// clock = pin 52.  For 32u4 Breakout Board+ and Teensy, data = pin B2,
// clock = pin B1.  For Leonardo, this can ONLY be done on the ICSP pins.
//LPD8806 strip = LPD8806(nLEDs);

volatile bool triggered = false;

int pattern = 0;
void setup() {
#if defined(__AVR_ATtiny85__) && (F_CPU == 16000000L)
  clock_prescale_set(clock_div_1); // Enable 16 MHz on Trinket
#endif
  pinMode(buttonPin, INPUT);

  attachInterrupt(digitalPinToInterrupt(buttonPin), click, RISING);
  // Start up the LED strip
  strip.begin();

  // Update the strip, to start they are all 'off'
  strip.show();

  //Serial.begin(9600);

  pinMode(12, OUTPUT);
  digitalWrite(12,HIGH);

  delay(1000);
  triggered = false;
}


void loop() {
  /*
  bool buttonState = digitalRead(buttonPin);
  if(!buttonState)
    pattern++;
  */
  switch(pattern){
    case 0:
      huron_fill_forward(200);
      break;
    case 1:
      huron_fade(50);
      break;
    case 2:
      usa(1000);
      break;
    case 3:
      blank(1000);
      break;
    case 4:
      huron_const(1000);
      break;
    case 5:
      michigan_const(1000);
      break;
    case 6:
      rainbow(1000);
      pattern++;
      break;
    case 7:
      michigan_const(1000);
      break;
    case 8:
      rainbowCycle(3);
      break;
    default:
      pattern = -1;
  }
  if(triggered){
    triggered = false;
    pattern++;
  }
}
void click(){
  triggered = true;
}


float bright = 0.5;
uint32_t green = strip.Color(0,127*bright,0);
uint32_t gold = strip.Color(114*bright,86*bright,0);
uint32_t blue = strip.Color(0,0,127*bright);
uint32_t maize = strip.Color(127*bright,127*bright,0);
uint32_t white = strip.Color(50*bright,50*bright,50*bright);
uint32_t red = strip.Color(127*bright,0,0);

void blank(int wait){
  for(int i=0; i<strip.numPixels(); i++) strip.setPixelColor(i, 0);
  strip.show();
  delay(wait);
}

void usa(int wait){
  for(int i=0; i<strip.numPixels()-2; i+=3){
    strip.setPixelColor(i, red);
    strip.setPixelColor(i+1, white);
    strip.setPixelColor(i+2, blue);
  }
  strip.show();
  delay(wait);
}

void huron_fill_forward(int wait){
  int i;
  /*
  for(i=0; i<strip.numPixels()/2; i++){
    strip.setPixelColor(i, 0);
    strip.setPixelColor(strip.numPixels()-i-1, 0);
    strip.show();
    delay(wait);
  }
  */
  for(i=0; i<strip.numPixels()/2; i++){
    strip.setPixelColor(i, green);
    strip.setPixelColor(strip.numPixels()-i-1, green);
    strip.show();
    delay(wait);
  }
  for(i=0; i<strip.numPixels()/2; i++){
    strip.setPixelColor(i, gold);
    strip.setPixelColor(strip.numPixels()-i-1, gold);
    strip.show();
    delay(wait);
  }
}
void huron_fill_reverse(int wait){
  int i;
  for(i=strip.numPixels()/2-1; i>=0; i--){
    strip.setPixelColor(i, 0);
    strip.setPixelColor(strip.numPixels()-i-1, 0);
    strip.show();
    delay(wait);
  }
  for(i=strip.numPixels()/2-1; i>=0; i--){
    strip.setPixelColor(i, green);
    strip.setPixelColor(strip.numPixels()-i-1, green);
    strip.show();
    delay(wait);
  }
  for(i=strip.numPixels()/2-1; i>=0; i--){
    strip.setPixelColor(i, gold);
    strip.setPixelColor(strip.numPixels()-i-1, gold);
    strip.show();
    delay(wait);
  }
}

void huron_const(int wait){
  for(int i=0; i<strip.numPixels(); i+=2){
    strip.setPixelColor(i, green);
    strip.setPixelColor(i+1, gold);
  }
  strip.show();
  delay(wait);
}

void huron_fade(int wait){
  int i, j, g, r;
  for(i=0; i<200; i++){
    j=i;
    if(i>100)
      j = 200-i;
    g = 127*bright*(j/100.0);
    for(j=0; j<strip.numPixels(); j++){
      strip.setPixelColor(j, strip.Color(0,g,0));
    }
    strip.show();
    delay(wait);
  }
  for(i=0; i<200; i++){
    j=i;
    if(i>100)
      j = 200-i;
    r = 114*bright*(j/100.0);
    g = 86*bright*(j/100.0);
    for(j=0; j<strip.numPixels(); j++){
      strip.setPixelColor(j, strip.Color(r,g,0));
    }
    strip.show();
    delay(wait);
  }
}
void bounce(int r, int g, int b, int wait){
  uint32_t color = strip.Color(r,g,b);
  int i;

  // Start by turning all pixels off:
  for(i=0; i<strip.numPixels(); i++) strip.setPixelColor(i, 0);

  // Then display one pixel at a time:
  for(i=0; i<strip.numPixels(); i++) {
    strip.setPixelColor(i, color); // Set new pixel 'on'
    strip.show();              // Refresh LED states
    strip.setPixelColor(i, 0); // Erase pixel, but don't refresh!
    delay(wait);
  }
  for(i=strip.numPixels()-2; i>=0; i--) {
    strip.setPixelColor(i, color);
    strip.show();
    strip.setPixelColor(i, 0);
    delay(wait);
  }
  strip.show();
}

void rainbow(int wait) {
  int i, j;
   
  for (j=0; j < 384; j++) {     // 3 cycles of all 384 colors in the wheel
    for (i=0; i < strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel( (i + j) % 384, 0.15));
    }  
    strip.show();   // write all the pixels out
    delay(wait);
  }
}

// Slightly different, this one makes the rainbow wheel equally distributed 
// along the chain
void rainbowCycle(uint8_t wait) {
  uint16_t i, j;
  
  for (j=0; j < 384; j++) {     // 5 cycles of all 384 colors in the wheel
    for (i=0; i < strip.numPixels(); i++) {
      // tricky math! we use each pixel as a fraction of the full 384-color wheel
      // (thats the i / strip.numPixels() part)
      // Then add in j which makes the colors go around per pixel
      // the % 384 is to make the wheel cycle around
      strip.setPixelColor(i, Wheel( ((i * 384 / strip.numPixels()) + j) % 384, 0.75) );
    }  
    strip.show();   // write all the pixels out
    delay(wait);
  }
}

void michigan_fill_forward(int wait){
  int i;
  for(i=0; i<strip.numPixels()/2; i++){
    strip.setPixelColor(i, 0);
    strip.setPixelColor(strip.numPixels()-i-1, 0);
    strip.show();
    delay(wait);
  }
  for(i=0; i<strip.numPixels()/2; i++){
    strip.setPixelColor(i, blue);
    strip.setPixelColor(strip.numPixels()-i-1, blue);
    strip.show();
    delay(wait);
  }
  for(i=0; i<strip.numPixels()/2; i++){
    strip.setPixelColor(i, maize);
    strip.setPixelColor(strip.numPixels()-i-1, maize);
    strip.show();
    delay(wait);
  }
}
void michigan_fill_reverse(int wait){
  int i;
  for(i=strip.numPixels()/2-1; i>=0; i--){
    strip.setPixelColor(i, 0);
    strip.setPixelColor(strip.numPixels()-i-1, 0);
    strip.show();
    delay(wait);
  }
  for(i=strip.numPixels()/2-1; i>=0; i--){
    strip.setPixelColor(i, blue);
    strip.setPixelColor(strip.numPixels()-i-1, blue);
    strip.show();
    delay(wait);
  }
  for(i=strip.numPixels()/2-1; i>=0; i--){
    strip.setPixelColor(i, maize);
    strip.setPixelColor(strip.numPixels()-i-1, maize);
    strip.show();
    delay(wait);
  }
}
void michigan_const(int wait){
  for(int i=0; i<strip.numPixels(); i+=2){
    strip.setPixelColor(i, blue);
    strip.setPixelColor(i+1, maize);
  }
  strip.show();
  delay(wait);
}

void michigan_fade(int wait){
  int i, j, b, g;
  for(i=0; i<200; i++){
    j=i;
    if(i>100)
      j = 200-i;
    b = 127*bright*(j/100.0);
    for(j=0; j<strip.numPixels(); j++){
      strip.setPixelColor(j, strip.Color(0,0,b));
    }
    strip.show();
    delay(wait);
  }
  for(i=0; i<200; i++){
    j=i;
    if(i>100)
      j = 200-i;
    b = 127*bright*(j/100.0);
    g = b;
    for(j=0; j<strip.numPixels(); j++){
      strip.setPixelColor(j, strip.Color(b,g,0));
    }
    strip.show();
    delay(wait);
  }
}
/* Helper functions */

//Input a value 0 to 384 to get a color value.
//The colours are a transition r - g -b - back to r

uint32_t Wheel(uint16_t WheelPos, float power)
{
  byte r, g, b;
  switch(WheelPos / 128)
  {
    case 0:
      r = 127 - WheelPos % 128;   //Red down
      g = WheelPos % 128;      // Green up
      b = 0;                  //blue off
      break; 
    case 1:
      g = 127 - WheelPos % 128;  //green down
      b = WheelPos % 128;      //blue up
      r = 0;                  //red off
      break; 
    case 2:
      b = 127 - WheelPos % 128;  //blue down 
      r = WheelPos % 128;      //red up
      g = 0;                  //green off
      break; 
  }
  return(strip.Color(r*power,g*power,b*power));
}
