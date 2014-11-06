#include <Servo.h>
class RobotServo
{
  Servo self;
public:
  void init(int PWM_PIN){
    self.attach(PWM_PIN);
  }
  void set(float angle){
    this->self.write(angle);
  }
};

RobotServo gripper;
void setup(){
  gripper.init(11);
}
void loop(){
  gripper.set(0);
  delay(1000);
  gripper.set(180);
}
