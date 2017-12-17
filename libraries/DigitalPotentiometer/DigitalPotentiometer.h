

#ifndef DIGITAL_POTENTIOMETER_H
#define DIGITAL_POTENTIOMETER_H

#include "Arduino.h"
#include <SPI.h>

class DigitalPotentiometer {
  // Base class for MCP41-type digital potentiometers with SPI communication
  //Max resistance: 5k/10k/50k/100k (R_AB on datasheet)
  protected:
    DigitalPotentiometer(int cs_pin, int resistance = 10000, int taps = 128);
  public:
    void set_resistance(float resistance);
    void set_raw(int value);
  private:
    int CS; int max_resistance; int resolution_steps;
    byte address;
};

class MCP41X1 : public DigitalPotentiometer {
/*  Control for the MCP41X1 Digital Potentiometer
*             __  __
*       ~CS -|. --  |- 5v
*     CLOCK -|      |- POT B
* MOSI/MISO -|      |- POT Wiper
*       GND -|______|- POT A
*
* Note: MOSI and CLOCK pins are specific to arduino board
*       For the Uno: 11 and 13
*/
  public:
    MCP41X1(int cs_pin, int resistance = 10000, int taps = 128);
};

class MCP41HVX1 : public DigitalPotentiometer {
/*  Control for the MCP41HVX1 Digital Potentiometer
*         TSSOP(ST) Pinout:
*           ____________
*      5v -| .          |- HV+
*   CLOCK -|            |- POT A
*     ~CS -|            |- POT Wiper
*     SDI -|            |- POT B
*     SDO -|            |- HV-
*   ~WLAT -|            |- GND
*   ~SHDN -|____________|- Not Connected
*
*     HV: High Voltage (< 36v)
*    SDI: SPI Data In (MOSI)
*    SDO: SPI Data Out (MISO)
*  ~WLAT: Write latch pin, tie to GND
*  ~SHDN: Shutdown pin, tie to 5v
*/
  public:
    MCP41HVX1(int cs_pin, int resistance = 10000, int taps = 128);
};

#endif
