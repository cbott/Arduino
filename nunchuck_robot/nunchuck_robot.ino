#include <Servo.h>
#include "RobotArm.h"
#include "sn754410.h"

#include "Wire.h"
#include "WiiChuck.h"
WiiChuck chuck;
RobotBase robot;

static const float spd = 0.15;

float robotx,roboty,robotz;
int robotw,robotwr;
void setup(){
  //nunchuck
  chuck.begin();
  chuck.update();
  robot.begin();
  Serial.begin(9600);
  robot.openGripper();
  set_home_values();
}

void set_home_values(){
  //postition values
  robotx = 0; roboty = 13.5; robotz = 10;
  robotw = 90; robotwr = 90;
}

void loop(){
  delay(20);
  chuck.update();
  float y = chuck.readJoyY();
  float x = chuck.readJoyX();
  bool z = chuck.buttonZ;
  bool c = chuck.buttonC;
  float pitch = chuck.readPitch();
  float roll = chuck.readRoll();
  
  //Clamp joystick when close to zero
  if(abs(y) < 0.1){
    y=0; 
  }
  if(abs(x) < 0.1){
    x=0; 
  }  

  //Tilted Up Controls
  if(pitch > 110 && pitch < 150){
    robot.arcadeDrive(0.0,0.0);
    if(c && z){
      set_home_values();
    } else {
      if(x > 0.9){
         robotwr+=1;
      } else if(x < -0.9) {
         robotwr-=1; 
      }
      if(y > 0.9){
         robotw+=1;
      } else if(y < -0.9) {
         robotw-=1; 
      }
      if(z){
        robot.closeGripper();  
      } else if(c){
        robot.openGripper(); 
      }
    }
  }
  //Tilted Down Controls
  else {
    if(c && z){
      //Drive using joystick if c+z are pressed
      robot.arcadeDrive(y,x);
    }
    else {
      robot.arcadeDrive(0.0,0.0);
      if(x > 0.9){
         robotx+=spd;
      } else if(x < -0.9) {
         robotx-=spd; 
      }
      if(y > 0.9){
         roboty+=spd;
      } else if(y < -0.9) {
         roboty-=spd; 
      }
      if(c){
        robotz+=spd;
      }else if(z){
        robotz-=spd; 
      }
    }  
  }

  //Serial.println(robot.getPressure());

  bool set = robot.moveTo(robotx,roboty,robotz,robotw,robotwr);
  if(!set)
    Serial.println("No solution found");
  
}

void printPos(){
  Serial.print("X:  ");
  Serial.print(robotx);
  Serial.print("    Y:  ");
  Serial.print(roboty);
  Serial.print("    Z:  ");
  Serial.println(robotz);
}

/*
void loop(){
  robot.moveTo(8,15,0,90,180);
  robot.closeGripper();
  delay(1000);
  robot.moveTo(-8,15,4,90,90,25);
  robot.openGripper();
  delay(1000);
}
*/