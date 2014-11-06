#include <stdlib.h>

// Pin definitions:
// The 74HC595 uses a type of serial connection called SPI
// (Serial Peripheral Interface) that requires three pins:

int datapins[] = {2, 5, 8, 11}; 
int clockpins[] = {3, 6, 9, 12};
int latchpins[] = {4, 7, 10, 13};

float switch_val;
int my_count = 0;
boolean clicked = 0;
boolean clicked_prev = 0;

//for dtostrf
char number_array[5];

byte data = 0;


void setup()
{
  // Set the three SPI pins to be outputs:
  for(int i=2; i<14; i++){
    pinMode(i, OUTPUT);
  }
  clearAll();
}


void loop()
{
  //binaryCount();        // Bit patterns from 0 to 255
  //click_count();
  count();
  
}

/* Layout of digital display:
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



void click_count(){
  switch_val = analogRead(0);
  if (switch_val > 1000){
    clicked = true; 
  }else{
    clicked = false; 
  }
  if(clicked && !clicked_prev){
    my_count++; 
  }
  disp_num(my_count);
  clicked_prev = clicked;
}

void rand_disp(){
  disp_digit(random(9), false, 1);
  delay(500);
}

void count(){
  int delayTime = 50;
  for(int a=0; a<=9; a++){
    disp_digit(a, false, 1);
    for (int b = 0; b<=9; b++){
      disp_digit(b, false, 2);
      for(int c = 0; c<=9; c++){
        disp_digit(c, false, 3);
        for(int d = 0; d<=9; d++){
          disp_digit(d, false, 4);
          delay(delayTime); 
        }
      }
    }
  }
}




void clearAll(){
  for(int i = 7; i>=0; i--){
    shiftWrite(i, LOW, 1);
    shiftWrite(i, LOW, 2);
    shiftWrite(i, LOW, 3);
    shiftWrite(i, LOW, 4);
  } 
}

void clearDisp(int register_num){
  for(int index = 7; index >= 0; index--)
  {
    shiftWrite(index, LOW, register_num);
  }
}

void disp_num(float number){
  dtostrf(number, 4, 3, number_array);
  int j=4;
  for(int i=0; i<j; i++){
    boolean period = false;
    int num = number_array[i] - '0';
    if(number_array[i+1] == '.'){
      period = true;
      i++;
      j++;
    }
    disp_digit(num, period, i);  
  }
}

void disp_digit(int num, boolean period, int placeval){
  clearDisp(placeval);
  switch (num){
    case 0:
      shiftWrite(0, HIGH, placeval);
      shiftWrite(1, HIGH, placeval);
      shiftWrite(2, HIGH, placeval);
      shiftWrite(4, HIGH, placeval);
      shiftWrite(5, HIGH, placeval);
      shiftWrite(6, HIGH, placeval);
      break;
    case 1:
      shiftWrite(2, HIGH, placeval);
      shiftWrite(5, HIGH, placeval);
      break;
    case 2:
      shiftWrite(0, HIGH, placeval);
      shiftWrite(2, HIGH, placeval);
      shiftWrite(3, HIGH, placeval);
      shiftWrite(4, HIGH, placeval);
      shiftWrite(6, HIGH, placeval);
      break;
    case 3:
      shiftWrite(0, HIGH, placeval);
      shiftWrite(2, HIGH, placeval);
      shiftWrite(3, HIGH, placeval);
      shiftWrite(5, HIGH, placeval);
      shiftWrite(6, HIGH, placeval);
      break;
    case 4:
      shiftWrite(1, HIGH, placeval);
      shiftWrite(2, HIGH, placeval);
      shiftWrite(3, HIGH, placeval);
      shiftWrite(5, HIGH, placeval);
      break;
    case 5:
      shiftWrite(0, HIGH, placeval);
      shiftWrite(1, HIGH, placeval);
      shiftWrite(3, HIGH, placeval);
      shiftWrite(5, HIGH, placeval);
      shiftWrite(6, HIGH, placeval);
      break;
    case 6:
      shiftWrite(1, HIGH, placeval);
      shiftWrite(3, HIGH, placeval);
      shiftWrite(4, HIGH, placeval);
      shiftWrite(5, HIGH, placeval);
      shiftWrite(6, HIGH, placeval);
      shiftWrite(0, HIGH, placeval);
      break;
    case 7:
      shiftWrite(0, HIGH, placeval);
      shiftWrite(2, HIGH, placeval);
      shiftWrite(5, HIGH, placeval);
      break;
    case 8:
      shiftWrite(0, HIGH, placeval);
      shiftWrite(1, HIGH, placeval);
      shiftWrite(2, HIGH, placeval);
      shiftWrite(3, HIGH, placeval);
      shiftWrite(4, HIGH, placeval);
      shiftWrite(5, HIGH, placeval);
      shiftWrite(6, HIGH, placeval);
      break;
    case 9:
      shiftWrite(0, HIGH, placeval);
      shiftWrite(1, HIGH, placeval);
      shiftWrite(2, HIGH, placeval);
      shiftWrite(3, HIGH, placeval);
      shiftWrite(5, HIGH, placeval);
      break;
    default:
      clearDisp(placeval);
  }
  if(period==true){
    shiftWrite(7, HIGH, placeval); 
  }
    
}




void shiftWrite(int desiredPin, boolean desiredState, int register_num)

// This function lets you make the shift register outputs
// HIGH, placeval or LOW in exactly the same way that you use digitalWrite().

// Like digitalWrite(), this function takes two parameters:

//    "desiredPin" is the shift register output pin
//    you want to affect (0-7)

//    "desiredState" is whether you want that output
//    to be HIGH, placeval or LOW

// Inside the Arduino, numbers are stored as arrays of "bits",
// each of which is a single 1 or 0 value. Because a "byte" type
// is also eight bits, we'll use a byte (which we named "data"
// at the top of this sketch) to send data to the shift register.
// If a bit in the byte is "1", the output will be HIGH, placeval. If the bit
// is "0", the output will be LOW.

// To turn the individual bits in "data" on and off, we'll use
// a new Arduino commands called bitWrite(), which can make
// individual bits in a number 1 or 0.
{
  
  constrain(register_num, 1, 4);

  //change register #s 1-4 to array positions 0-3
  register_num--;
  // First we'll alter the global variable "data", changing the
  // desired bit to 1 or 0:

  bitWrite(data,desiredPin,desiredState);
  
  // Now we'll actually send that data to the shift register.
  // The shiftOut() function does all the hard work of
  // manipulating the data and clock pins to move the data
  // into the shift register:

  shiftOut(datapins[register_num], clockpins[register_num], MSBFIRST, data);

  // Once the data is in the shift register, we still need to
  // make it appear at the outputs. We'll toggle the state of
  // the latchPin, which will signal the shift register to "latch"
  // the data to the outputs. (Latch activates on the HIGH, placeval-to
  // -low transition).

  digitalWrite(latchpins[register_num], HIGH);
  digitalWrite(latchpins[register_num], LOW);
}


/*
binaryCount()

Numbers are stored internally in the Arduino as arrays of "bits",
each of which is a 1 or 0. Just like the base-10 numbers we use
every day, The position of the bit affects the magnitude of its 
contribution to the total number:

Bit position   Contribution
0              1
1              2
2              4
3              8
4              16
5              32
6              64
7              128

To build any number from 0 to 255 from the above 8 bits, just
select the contributions you need to make. The bits will then be
1 if you use that contribution, and 0 if you don't.

This function will increment the "data" variable from 0 to 255
and repeat. When we send this value to the shift register and LEDs,
you can see the on-off pattern of the eight bits that make up the
byte. See http://www.arduino.cc/playground/Code/BitMath for more
information on binary numbers.
*/

//void binaryCount()
//{
//  int delayTime = 1000; // time (milliseconds) to pause between LEDs
//                        // make this smaller for faster switching
//  
//  // Send the data byte to the shift register:
//
//  shiftOut(datapin, clockpin, MSBFIRST, data);
//
//  // Toggle the latch pin to make the data appear at the outputs:
//
//  digitalWrite(latchpin, HIGH);
//  digitalWrite(latchpin, LOW);
//  
//  // Add one to data, and repeat!
//  // (Because a byte type can only store numbers from 0 to 255,
//  // if we add more than that, it will "roll around" back to 0
//  // and start over).
//
//  data++;
//
//  // Delay so you can see what's going on:
//
//  delay(delayTime);
//}
