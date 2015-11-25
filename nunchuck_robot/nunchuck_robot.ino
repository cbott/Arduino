/*
elbow - up = 15, down = 130
base - left = 0, right = 180
shoulder - back = 0, forward = 180
wrist - down = 0, up = 180
wrist rotate - left = 0, right = 180
gripper - open = 10, closed = 180
*/


#include <Servo.h>  // servo library
#include "sn754410.h"

#include "Wire.h"
#include "WiiChuck.h"
WiiChuck chuck = WiiChuck();
const int BUFFLEN = 10;
//buffers for smoothing the pitch/roll values
float wbuf[BUFFLEN];//wrist
float rbuf[BUFFLEN];//rotate
float ebuf[BUFFLEN];//elbow

//PWM's : 3, 5, 6, 9, 10, 11
const int BASE_PWM = 3;
const int SHOULDER_PWM = 5;
const int ELBOW_PWM = 6;
const int WRIST_PWM = 9;
const int W_ROTATE_PWM = 10;
const int GRIPPER_PWM = 11;

const int L_DRIVE_1 = 2;
const int L_DRIVE_2 = 4;
const int R_DRIVE_1 = 7;
const int R_DRIVE_2 = 8;

//analog pins
const int PRESSURE_PIN = 0;

//gripper constants:
const int OPEN = 15;
const int CLOSED = 170;

//Motor declarations
Servo base;
Servo shoulder;
Servo elbow;
Servo wrist;
Servo wrist_rotate;
Servo gripper;

sn754410 drive(L_DRIVE_1, L_DRIVE_2, R_DRIVE_1, R_DRIVE_2);

void setup()
{
  for (int i = 0; i < BUFFLEN-1; i++) {
    wbuf[i] = 20; rbuf[i] = 90; ebuf[i] = 100;
  }
  //Set up digital outputs
  pinMode(L_DRIVE_1, OUTPUT);
  pinMode(L_DRIVE_2, OUTPUT);
  pinMode(R_DRIVE_1, OUTPUT);
  pinMode(R_DRIVE_2, OUTPUT);
  //enable servos
  base.attach(BASE_PWM);
  shoulder.attach(SHOULDER_PWM);
  elbow.attach(ELBOW_PWM);
  gripper.attach(GRIPPER_PWM);
  wrist_rotate.attach(W_ROTATE_PWM);
  wrist.attach(WRIST_PWM);
  
  //nunchuck
  chuck.begin();
  chuck.update();
  
  //begin serial transmission
  Serial.begin(9600);

  Home();
}


void loop()
{  
  delay(20);
  chuck.update();
  float y = chuck.readJoyY();
  float x = chuck.readJoyX();
  bool z = chuck.buttonZ;
  bool c = chuck.buttonC;
  float pitch = chuck.readPitch();
  float roll = chuck.readRoll();
  if(abs(y) < 0.1){
    y=0; 
  }
  if(abs(x) < 0.1){
    x=0; 
  }
  
  if(z && c){ //drive if z and c are pressed
    float l=0;
    float r=0;
    float div = PI/4;
    if(x==0 && y!=0){
      l=y; r=y;
    } else if (y==0 && x!=0){
      l=x; r=-x;  
    } else if (x>0 && y>0) { //quadrant 1
      l=1; r=atan(y/x)/div-1;
    } else if (x<0 && y>0) { //quadrant 2
      l=atan(-y/x)/div-1; r=1;
    } else if (x<0 && y<0) { //quadrant 3
      l=-1; r=atan(-y/x)/div+1;
    } else if (x>0 && y<0) { //quadrant 4
      l=atan(y/x)/div+1; r=-1;
    }
    float dist = sqrt(pow(x,2)+pow(y,2));
    l = l*dist;
    r = r*dist;
    drive.set(l,r);
  } else if(z){ //base, shoulder, elbow controls if z button is pressed
    float etotal=0;
    for(int i=0; i<BUFFLEN-1; i++){
      ebuf[i] = ebuf[i+1]; etotal+=ebuf[i];
    }
    ebuf[BUFFLEN-1] = pitch; etotal+=pitch;
    /*
    elbow.write(145-constrain(etotal/BUFFLEN, 15, 130));
    shoulder.write(constrain(shoulder.read()+(y>0)-(y<0), 0, 180));
    */
    shoulder.write(constrain(180-etotal/BUFFLEN, 10, 170));
    elbow.write(constrain(elbow.read()-(y>0.5)+(y<-0.5), 15, 130));
    base.write(constrain(base.read()+(x>0.5)-(x<-0.5), 0, 180));
  } else if(c) { //gripper, wrist, and wrist rotate controls if c is pressed
    float wtotal=0;
    float rtotal=0;
    for(int i=0; i<BUFFLEN-1; i++){
      wbuf[i] = wbuf[i+1]; wtotal+=wbuf[i];
      rbuf[i] = rbuf[i+1]; rtotal+=rbuf[i];
    }
    wbuf[BUFFLEN-1] = pitch; wtotal+=pitch;
    rbuf[BUFFLEN-1] = roll+90; rtotal+=roll+90;
    wrist.write(wtotal/BUFFLEN);
    wrist_rotate.write(rtotal/BUFFLEN);
    gripper.write(constrain(gripper.read()+(x>0)-(x<0), OPEN, CLOSED));
  } else { //home if no buttons are pressed
    Home();
  }
}

void Home(){
  //set the robot to a resting position
  drive.set(0,0);
  set(shoulder, 80, 1); 
  set(elbow, 100, 1);
  set(base, 100, 1);
  set(wrist, 10, 1);
  set(wrist_rotate, 90, 1);
  //set(gripper, OPEN, 1);
}
void set(Servo motor, int new_position, int spd){
  //spd=0,1,2    0=slow   1=normal   2=max
  int Delay;
  if (spd==0){
    Delay=35; 
  }else if (spd==2){
    Delay=0; 
  }else{
    Delay=15; 
  }
  
  int current_position = motor.read();
  if (current_position < new_position){
    for (current_position; current_position < new_position; current_position++){
      motor.write(current_position);
      delay(Delay);
    }
  }
  else if(current_position > new_position){
    for (current_position; current_position > new_position; current_position--){
      motor.write(current_position);
      delay(Delay);
    } 
  } 
}
