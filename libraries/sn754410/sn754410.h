/****Pinout of SN754410 quadruple half-H driver
*    ---- ----
* en1|  |_|  |+5v
* m1a|       |m2b
* T1a|       |T2b
* GND|       |GND
* GND|       |GND
* T1b|       |T2a
* m1b|       |m2a
* PWR|_______|en2
*
*  T = motor terminal
*  M = motor logic pin (from arduino)
* EN = Enable pin - Omited in code, tie to ground to disable output
* PWR= voltage supply for motors
***********************/
#ifndef sn754410_h
#define sn754410_h

#include "Arduino.h"
class sn754410 {
  public:
    sn754410(int m1a, int m1b, int m2a, int m2b);
    void set(float left, float right);
    void test();
  private:
    int _m1a; int _m1b; int _m2a; int _m2b;
};

#endif
