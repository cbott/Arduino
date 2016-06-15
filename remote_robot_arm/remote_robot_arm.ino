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

#include <Wire.h>
#define SLAVE_ADDRESS 0x04


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

int command;

void setup()
{
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
  
  //begin serial transmission
  Serial.begin(9600);
  
  Wire.begin(SLAVE_ADDRESS);
 
  // define callbacks for i2c communication
  Wire.onReceive(receiveData);
  Wire.onRequest(sendData);
 
  Serial.println("Ready!");
  
  command = 0;

  Home();
}


void loop(){
  switch(command){
    case 0:
      drive.set(0,0);
      break;
    case 1:
      drive.set(1,1);
      break;
    case 2:
      drive.set(-1,-1);
      break;
    case 3:
      drive.set(1,-1);
      break;
    case 4:
      drive.set(-1,1);
      break;
    case 255:
      Home();
      break;
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

// callback for received data
void receiveData(int byteCount){
     while(Wire.available()) {
        command = Wire.read();
     }
     command = byte(command);
     Serial.print("data received: ");
     Serial.println(command);
}

// callback for sending data
void sendData(){
    Wire.write(command);
}
