#include "Arduino.h"
#include <SPI.h>
#include "DigitalPotentiometer.h"

DigitalPotentiometer::DigitalPotentiometer(int cs_pin, int resistance, int taps)
                : CS(cs_pin),
                  max_resistance(resistance),
                  resolution_steps(taps){
  address = 0x00;
  pinMode(CS, OUTPUT);
  SPI.begin();
}

void DigitalPotentiometer::set_resistance(float ohms){
  constrain(ohms, 0, max_resistance);
  set_raw(resolution_steps * ohms / max_resistance);
}

void DigitalPotentiometer::set_raw(int value){
  digitalWrite(CS, LOW);
  SPI.transfer(address);
  SPI.transfer(value);
  digitalWrite(CS, HIGH);
}

MCP41X1::MCP41X1(int cs_pin, int resistance, int taps)
  : DigitalPotentiometer(cs_pin, resistance, taps) {}

MCP41HVX1::MCP41HVX1(int cs_pin, int resistance, int taps)
  : DigitalPotentiometer(cs_pin, resistance, taps) {}
