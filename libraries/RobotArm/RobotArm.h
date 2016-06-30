/*
* Library for handling basic control functions of servo-based robot arm
*/

#ifndef RobotArm_h
#define RobotArm_h

#include "Arduino.h"
#include <Servo.h>
#include "sn754410.h" //for drive motors

/*
elbow - up = 15, down = 130
base - left = 0, right = 180
shoulder - back = 0, forward = 180
wrist - down = 0, up = 180
wrist rotate - left = 0, right = 180
gripper - open = 10, closed = 180
*/
class RobotBase{
  public:
    RobotBase();
    void begin();
    boolean set(float x, float y, float z, float wrist_angle = 90, float wrist_rotate_angle = 90);
    boolean moveTo(float x, float y, float z, float w, float wr, float cms=15); //speed in cm/s
    void setGripper(int angle);
    void openGripper();
    void closeGripper();
    float getPressure();

    void tankDrive(float left, float right);
    void arcadeDrive(float forward, float turn);

    void home();

    //accessor methods
    float getX();float getY();float getZ();
    int getW(); int getWR();
  private:
    //Length Constants of Robot Arm Segments
    static const float A = 11.7;
    static const float B = 12.3;
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
    const int GRIP_OPEN = 15;
    const int GRIP_CLOSED = 170;
    
    Servo base;
    Servo shoulder;
    Servo elbow;
    Servo wrist;
    Servo wrist_rotate;
    Servo gripper;
    
    sn754410 drive;

    //Position Values
    float robotx,roboty,robotz;
    int robotw,robotwr;
};

#endif
