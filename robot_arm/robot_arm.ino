
/*
elbow - up = 15, down = 130
base - left = 0, right = 180
shoulder - back = 0, forward = 180
wrist - down = 0, up = 180
wrist rotate - left = 0, right = 180
gripper - open = 10, closed = 180
*/


#include <Servo.h>  // servo library


const int NORM_SPD = 15;//speed set() moves at: lower NORM_SPD means faster movement
const int SLOW_SPD = 35; //speed for spd=0
const int FAST_SPD = 0;//delay for spd=2


//PWM's : 3, 5, 6, 9, 10, 11
const int BASE_PWM = 3;
const int SHOULDER_PWM = 5;
const int ELBOW_PWM = 6;
const int WRIST_PWM = 9;
const int W_ROTATE_PWM = 10;
const int GRIPPER_PWM = 11;

const int indicLED = 12;

const int L_DRIVE_1 = 2;
const int L_DRIVE_2 = 4;
const int R_DRIVE_1 = 7;
const int R_DRIVE_2 = 8;

//analog pins
const int PRESSURE_PIN = 0;

//gripper constants:
const int OPEN = 15;
const int CLOSED = 170;

Servo base;
Servo shoulder;
Servo elbow;
Servo wrist;
Servo wrist_rotate;
Servo gripper;


void setup()
{
  //If you ever want to
  // release the servo (allowing it to be turned by hand),
  // you can call servo1.detach().
  pinMode(indicLED, OUTPUT);
  pinMode(L_DRIVE_1, OUTPUT);
  pinMode(L_DRIVE_2, OUTPUT);
  pinMode(R_DRIVE_1, OUTPUT);
  pinMode(R_DRIVE_2, OUTPUT);

  randomSeed(analogRead(0));

  base.attach(BASE_PWM);
  shoulder.attach(SHOULDER_PWM);
  elbow.attach(ELBOW_PWM);
  gripper.attach(GRIPPER_PWM);
  wrist_rotate.attach(W_ROTATE_PWM);
  wrist.attach(WRIST_PWM);
  
  Serial.begin(9600);
  
  Home();
}


void loop()
{  
  digitalWrite(indicLED, HIGH);
  rtnClose();
}
//////////////////////////////////////routines///////////////////////////////////////////
void rtnFlyTrap(){
  set(wrist,90,1);
  while (analogRead(PRESSURE_PIN)<6){
    int val = analogRead(PRESSURE_PIN);
    if (val>3){
      Serial.println(val);
    }
    set(gripper, OPEN, 2);
  }
  set(gripper, CLOSED, 2);
  delay(3000);
  set(gripper, OPEN, 0);
}
void rtnPickPlace(){
   Home();
   delay(2000);
   set(base, 135, 1);
   set(elbow, 20, 1);
   set(wrist, 15, 2);
   set(shoulder, 160, 1);
   set(gripper, CLOSED, 0);
   delay(100);
   set(shoulder, 110, 1);
   set(elbow, 30, 2);
   set(base, 45, 1);
   set(shoulder, 140, 1);
   set(wrist, 90, 1);
   set(gripper, OPEN, 0);
}

void rtnGive(){
  drive(1,1);
  delay(3000);
  drive(0,0);
  set(gripper, OPEN, 2);
  set(wrist_rotate, 90, 2);
  set(elbow, 30, 2);
  set(shoulder, 135, 1);
  set(wrist, 90, 0);
  set(gripper, CLOSED, 0);
  set(shoulder, 90, 1);
  set(wrist, 0, 1);
  drive(1,-1);
  delay(3000);
  drive(0,0);
  delay(500);
  drive(1,1);
  delay(3000);
  drive(0,0);
  set(shoulder, 135, 0);
  set(wrist, 90, 0);
  delay(1000);
  set(gripper, OPEN, 1);
  delay(1000);
  set(shoulder, 90, 1);
  set(wrist, 0, 1);
  drive(1,-1);
  delay(3000);
  drive(0,0);
  delay(5000);
}

void rtnDrive(){
  drive(1,1);
  delay(5000);
  drive(1,-1);
  delay(5000);
  drive(-1,1);
  delay(5000);
  drive(-1,-1);
  delay(5000);
}

void rtnPen(){
   set(base, 90, 2);
   set(shoulder, 110, 2);
   set(elbow, 120, 2);
   set(wrist, 135, 2);
   set(wrist_rotate, 90, 2);
   set(gripper, 140, 2);
   while(true){
     set(base,130,1);
     delay(500);
     set(base, 60,1);
     delay(500);
   }
}



void rtnPiano(){
   int DOWN = 85;
   int UP = 50;
   set(wrist, 0 , 1);
   set(shoulder, 100, 1);
   set(base, 80, 1);
   set(gripper, OPEN, 2);
   set(wrist_rotate, 10, 1);
   set(elbow, UP, 1);
   
   set(elbow, DOWN, 1);
   set(elbow, UP, 1);
   set(base, 100, 1);
   set(wrist_rotate, 0, 1); 
   set(elbow, DOWN, 1);
   set(elbow, UP, 1);
}

void rtnLaser(){
  //routine to hold a laser pointer and have a dog chase the beam  
  int HOLD = 120;//angle for gripper to holp laser without turning it on
  int PRESS = 140;//angle for gripper to turn on laser
  
  set(base, 90, 1);
  set(shoulder, 80, 1);
  set(elbow, 50, 1);
  set(wrist, 60, 1);
  set(wrist_rotate, 90, 1);
  delay(3000);
  set(gripper, HOLD, 0);
  delay(1000);
  set(gripper, PRESS, 0);
  
  while(true){
  set(base, random(30, 150), 1);
  set(wrist, random(35, 75), 0);
  drive(random(-1,2), random(-1,2));
  delay(1000);
  }
}

void rtnScoop(){
  set(gripper, CLOSED, 1);
  
  set(base, 40, 1);
  set(shoulder, 90, 1);
  set(elbow, 30, 1);
  set(wrist_rotate, 0, 1);
  set(wrist, 20, 1);
  delay(500);
  set(shoulder, 120, 1);
  set(wrist, 65, 0);
  set(shoulder, 100, 0);
  set(wrist, 50, 0);
  set(base, 140, 1);
  set(wrist_rotate, 180, 1);
}

void rtnMelon(){
  //ball a melon
  set(shoulder, 90, 1);
  set(base, 50, 1);
  set(elbow, 80, 1);
  set(wrist, 90, 1);
  set(wrist_rotate, 0, 1);
  set(gripper, CLOSED, 1);
  delay(500);
  set(elbow, 110, 1);
  set(wrist_rotate, 180, 1);
  set(wrist, 110, 1);
  set(elbow, 80, 1); 
  set(base, 120, 1);
  set(wrist_rotate, 0, 1);
  for(int i=0; i<3; i++){    
    set(wrist, 100, 2);
    delay(100);
    set(wrist, 80, 2);
    delay(100);
  }
 
}

void rtnOpen(int turns){
  set(base, 90, 1);
  set(shoulder, 90, 1);
  set(elbow, 120, 1);
  set(wrist, 35, 1);
  for(int i=0; i<turns; i++){
     set(gripper, OPEN, 1);
     set(wrist_rotate, 180, 1);
     set(gripper, CLOSED, 1);
     set(wrist_rotate, 0, 1);
  } 
  elbow.write(90);
  wrist.write(0);
  delay(300);
  set(wrist, 95, 1);
  delay(3000);
}

void rtnTest(){
  //moves all motors in a pattern
  int shoulderDown = 140;
  set(elbow, 30,1);
  delay(500);
  set(shoulder, 80,1);
  set(base, 50,1);
  set(shoulder, shoulderDown,1);
  set(wrist_rotate, 0,1);
  set(wrist, 10, 1);
  set(gripper, 10, 1);
  set(shoulder, 80, 1);
  
  delay(1000);
  set(base, 120, 1);
  set(shoulder, shoulderDown, 1);
  set(wrist_rotate, 180, 1);
  set(gripper, 180, 1);
  set(wrist, 170, 1); 
  set(elbow, 20, 1); 
}

void Home(){
  //set the robot to a resting position
  set(shoulder, 80, 1); 
  set(elbow, 100, 1);
  set(base, 90, 1);
  set(wrist, 10, 1);
  set(wrist_rotate, 90, 1);
  set(gripper, OPEN, 1);
}

void close_gripper(){
  int i = gripper.read();
  while(has_object()==false){
    set(gripper, i, 2);
    i+=1; 
  }
}

/////////////////////////////////////////////////////////////////////set() function////////////////////////////////////////////////////////////////////////////////
  // Tell servo to move at a slower speed
void set(Servo motor, int new_position, int spd){
  //spd=0,1,2    0=slow   1=normal   2=max
  int Delay;
  if (spd==0){
    Delay=SLOW_SPD; 
  }else if (spd==2){
    Delay=FAST_SPD; 
  }else{
    Delay=NORM_SPD; 
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
///////////////////////////Drive functions///////////////
void drive(int left, int right){
  if( left > 0){
    digitalWrite(L_DRIVE_1, HIGH);
    digitalWrite(L_DRIVE_2, LOW);
  } 
  else if (left < 0){
    digitalWrite(L_DRIVE_2, HIGH);
    digitalWrite(L_DRIVE_1, LOW);
  }
  else if (left == 0){
    digitalWrite(L_DRIVE_2, LOW);
    digitalWrite(L_DRIVE_1, LOW);
  }
  if( right > 0){
    digitalWrite(R_DRIVE_1, HIGH);
    digitalWrite(R_DRIVE_2, LOW);
  } 
  else if (right < 0){
    digitalWrite(R_DRIVE_2, HIGH);
    digitalWrite(R_DRIVE_1, LOW);
  }
  else if (right == 0){
    digitalWrite(R_DRIVE_2, LOW);
    digitalWrite(R_DRIVE_1, LOW);
  }
}

////////////////////////////////gripper pressure sensing///////////////
boolean has_object(){
  if (analogRead(PRESSURE_PIN)>50){
    return true;
  } else{
    return false; 
  }
}

