// Library to control a 7-segment LED display through a shift register

/*  Layout of SN74HC595N 8-bit Shift Register
*         --_--
*  OUT 1 |     | 5v
*  OUT 2 |     | OUT 0
*  OUT 3 |     | Serial Data
*  OUT 4 |     | ~Output Enable (GND)
*  OUT 5 |     | Latch
*  OUT 6 |     | Clock
*  OUT 7 |     | ~SRCLR (5v)
*    GND |_____| ~OUT 7
*

*  Layout of 7-segment display:
*
*         Register Numbers
*         ----------------
*   __          _0_
*  |__|      1|     |2
*  |__|.  =     -3- 
*            4|     |5
*               -6-      .7
*                  
*/

#ifndef Shift7Segment_H
#define Shift7Segment_H

#include <Arduino.h>

#define SEVEN_SEGMENT_0 0b01110111
#define SEVEN_SEGMENT_1 0b00100100
#define SEVEN_SEGMENT_2 0b01011101
#define SEVEN_SEGMENT_3 0b01101101
#define SEVEN_SEGMENT_4 0b00101110
#define SEVEN_SEGMENT_5 0b01101011
#define SEVEN_SEGMENT_6 0b01111011
#define SEVEN_SEGMENT_7 0b00100101
#define SEVEN_SEGMENT_8 0b01111111
#define SEVEN_SEGMENT_9 0b00101111

class Shift7Segment {
  private:
    int _datapin, _clockpin, _latchpin;
  public:
    Shift7Segment(int datapin, int clockpin, int latchpin);
    void disp_byte(byte segments);
    void disp_digit(int num, boolean period = false);
};

#endif
