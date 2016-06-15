#include "AnalogJoystick.h"
#include <Servo.h>

//joystick analog inputs
static const int XPIN = 0;
static const int YPIN = 1;
//joystick SEL button DIO
static const int BTNPIN = 7;

//lock servo pin
static const int SERVOPIN = 8;

//lock password constants
static const int PASSLEN = 4;
static const int PASSWORD[] = {1,1,3,4};
//   1
// 4   2
//   3
int enter[PASSLEN];

bool zeroed;


AnalogJoystick rightStick(XPIN, YPIN, BTNPIN);
Servo lock;

void setup() {
  Serial.begin(9600); 
  zeroed = false;
  lock.attach(SERVOPIN);
  setLocked();
}

void loop() {
  float x = rightStick.GetX();
  float y = rightStick.GetY();
  boolean pressed = rightStick.GetPressed();
  
  int val = 0;

  if ( y > 0.5 ){
    val = 1;
  } else if ( x > 0.5 ){
    val = 2;
  } else if ( y < -0.5 ){
    val = 3;
  } else if ( x < -0.5){
    val = 4;
  }
  
  
  if( val && zeroed ){ //new value is tapped
    Serial.println(val);
    
    for ( int i = 0; i < PASSLEN - 1; i++){
      enter[i] = enter[i+1];
    }
    enter[PASSLEN - 1] = val;
    
    /*
    Serial.print(enter[0]);
    Serial.print(enter[1]);
    Serial.print(enter[2]);
    Serial.println(enter[3]);
    */
    
    if ( arraysEqual( enter, PASSWORD, PASSLEN ) ){
        setUnlocked();
    }
  }
  
  if (pressed){//lock if you click the joystick
    setLocked();
  }
  
  if( val == 0 ){
    zeroed = true;
  } else {
    zeroed = false;
  }
  
}

void setUnlocked(){
  Serial.println("GOT IT!");    
  lock.attach(SERVOPIN);
  lock.write(170);//unlocked position
  delay(500);
  lock.detach();
}

void setLocked(){  
  lock.attach(SERVOPIN);
  lock.write(80);//locked position
  delay(500);
  lock.detach();
}

boolean arraysEqual( const int arr1[], const int arr2[], int elements ){
  for( int i = 0; i < elements; i++ ){
    if( arr1[i] != arr2[i] ){
      return false;
    }
  }
  return true;
}
