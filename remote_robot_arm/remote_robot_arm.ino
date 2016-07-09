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

  robot.home();
}


void loop(){
  switch(command){
    case 0:
      robot.tankDrive(0,0);
      break;
    case 1:
      robot.tankDrive(1,1);
      break;
    case 2:
      robot.tankDrive(-1,-1);
      break;
    case 3:
      robot.tankDrive(1,-1);
      break;
    case 4:
      robot.tankDrive(-1,1);
      break;
    case 255:
      robot.home();
      break;
  }
  
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
