#include <Servo.h>
#include "RobotArm.h"
#include "sn754410.h"

#include <Wire.h>
#define SLAVE_ADDRESS 0x04

RobotBase robot;

int command;

void setup()
{
  Serial.begin(9600);
  
  Wire.begin(SLAVE_ADDRESS);
 
  // define callbacks for i2c communication
  Wire.onReceive(receiveData);
  Wire.onRequest(sendData);
 
  Serial.println("Ready!");
  
  command = 0;

  robot.begin();
  robot.home();
}


void loop(){
  if(command == 0){
    robot.tankDrive(0,0);
  } else if(command == 1){
    robot.tankDrive(1,1);
  } else if(command == 2){
    robot.tankDrive(-1,-1);
  } else if(command == 3){
    robot.tankDrive(-1,1);
  } else if(command == 4){
    robot.tankDrive(1,-1);
  } else if(command >= 15 && command <= 170){
    robot.setGripper(command); 
  }else if(command == 255){
    robot.home();
  }
  delay(1);
  
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
