#include "Shift7Segment.h"

Shift7Segment::Shift7Segment(int datapin, int clockpin, int latchpin)
    : _datapin(datapin), _clockpin(clockpin), _latchpin(latchpin) {
  // Set the three SPI pins to be outputs:
  pinMode(_datapin, OUTPUT);
  pinMode(_clockpin, OUTPUT);
  pinMode(_latchpin, OUTPUT);
}

void Shift7Segment::disp_byte(byte segments){
  shiftOut(_datapin, _clockpin, MSBFIRST, segments);
  digitalWrite(_latchpin, HIGH);
  digitalWrite(_latchpin, LOW);
}

void Shift7Segment::disp_digit(int num, boolean period = false){
  byte digit_repr = 0b00000000;
  switch (num){
    case 0:
      digit_repr = SEVEN_SEGMENT_0;
      break;
    case 1:
      digit_repr = SEVEN_SEGMENT_1;
      break;
    case 2:
      digit_repr = SEVEN_SEGMENT_2;
      break;
    case 3:
      digit_repr = SEVEN_SEGMENT_3;
      break;
    case 4:
      digit_repr = SEVEN_SEGMENT_4;
      break;
    case 5:
      digit_repr = SEVEN_SEGMENT_5;
      break;
    case 6:
      digit_repr = SEVEN_SEGMENT_6;
      break;
    case 7:
      digit_repr = SEVEN_SEGMENT_7;
      break;
    case 8:
      digit_repr = SEVEN_SEGMENT_8;
      break;
    case 9:
      digit_repr = SEVEN_SEGMENT_9;
      break;
  }
  if(period){
    bitWrite(digit_repr, 7, 1); // set bit 7
  }
  disp_byte(digit_repr);
}
