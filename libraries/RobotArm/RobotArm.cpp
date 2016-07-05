#include "Arduino.h"
#include "sn754410.h" //for drive motors
#include <Servo.h>
#include "RobotArm.h"

RobotBase::RobotBase():
  drive(L_DRIVE_1, L_DRIVE_2, R_DRIVE_1, R_DRIVE_2){

}
void RobotBase::begin(){
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

  home();
}
boolean RobotBase::set(float x, float y, float z, float wrist_angle, float wrist_rotate_angle){
    float r = sqrt(x*x+y*y); // radius
    float val_acos_e = (A*A+B*B-r*r-z*z)/(2*A*B);
    float val_acos_s = (A*A - B*B + r*r) / (2*A*r);
    if(abs(val_acos_e)<1 && abs(val_acos_s)<1){
      //Update robot position
      robotx = x; roboty = y; robotz = z; robotw = wrist_angle; robotwr = wrist_rotate_angle;
      int elbow_a = acos(val_acos_e) * 180.0 / PI;
      int shoulder_a = (atan2(z, r) + acos(val_acos_s)) * 180.0 / PI;
      int base_a = (PI-atan2(y,x)) * 180.0 / PI;
      int wrist_a = wrist_angle + 90 - shoulder_a - elbow_a;
      int wrist_rotate_a = 90 - base_a + wrist_rotate_angle;
      
      base.write(base_a);
      shoulder.write(180-shoulder_a);
      elbow.write(180-elbow_a);
      wrist.write(wrist_a);
      wrist_rotate.write(wrist_rotate_a);
      return true;
    }
    return false; //No valid solution to the equations
}
boolean RobotBase::moveTo(float x, float y, float z, float w, float wr, float cms){
  int wait = 10; //ms gap between movements

  float dx = x-robotx;
  float dy = y-roboty;
  float dz = z-robotz;
  float dw = w-robotw;
  float dwr = wr-robotwr;
  float startx = robotx; float starty = roboty; float startz = robotz; //store values that will be overwritten by set()
  int startw = robotw; int startwr = robotwr;

  float dist = sqrt(dx*dx+dy*dy+dz*dz); //distance from current position to new position
  float t = dist / cms; //time it should take to reach the new coordinate
  int inc =  t / wait * 1000 + 0.99; // the number of increments with delay time wait needed to take time t
  
  if(inc == 0  && (dwr || dw)){
    inc = 1; //if the position is unchanged, but the wrist angles change, use 1 loop to set them
  }
  for(int j=1; j<=inc; j++){
    float i = j; //allows intermediate steps so that (i/inc) != 1 or 0 only
    if(!set(startx+dx*(i/inc),
        starty+dy*(i/inc),
        startz+dz*(i/inc),
        startw+dw*(i/inc),
        startwr+dwr*(i/inc))
      )
      return false; //stop if the robot was unable to move to a coordinate
    delay(wait);
  }
  return true;
}
void RobotBase::setGripper(int angle){
  gripper.write(angle); 
}
void RobotBase::openGripper(){
  setGripper(GRIP_OPEN); 
}
void RobotBase::closeGripper(){
  setGripper(GRIP_CLOSED); 
}
float RobotBase::getPressure(){
  //read from the pressure sensor in the gripper
  float val = analogRead(PRESSURE_PIN)/950.0;
  return constrain(val, 0.0, 1.0); //actual reads may range up to 960
}
void RobotBase::tankDrive(float left, float right){
  drive.set(left, right);
}
void RobotBase::arcadeDrive(float forward, float turn){
  float leftOutput, rightOutput;
  if (forward > 0.0){
    if (turn > 0.0){
      leftOutput = forward + turn;
      rightOutput = max(forward, -turn);
    } else {
      leftOutput = max(forward, turn);
      rightOutput = forward - turn;
    }
  } else {
    if (turn > 0.0){
      leftOutput = - max(-forward, -turn);
      rightOutput = forward - turn;
    } else {
      leftOutput = forward + turn;
      rightOutput = - max(-forward, turn);
    }
  }
  drive.set(leftOutput, rightOutput);
}
void RobotBase::home(){
  set(0,13.5,10,90,90);
}
float RobotBase::getX(){
  return robotx;
}
float RobotBase::getY(){
  return roboty;
}
float RobotBase::getZ(){
  return robotz;
}
int RobotBase::getW(){
  return robotw;
}
int RobotBase::getWR(){
  return robotwr;
}