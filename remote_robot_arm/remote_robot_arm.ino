
/*
elbow - up = 15, down = 130
base - left = 0, right = 180
shoulder - back = 0, forward = 180
wrist - down = 0, up = 180
wrist rotate - left = 0, right = 180
gripper - open = 10, closed = 180
*/


#include <Servo.h>  // servo library
#include <Wire.h>

//Communication setup
#define SLAVE_ADDRESS 0x04
static const byte DATA_BITS = 6;//bits used as data (8-id bits)
boolean commands[64];//make sure this is equal to 2**DATA_BITS

int number;//value received over i2c
int id = 0;    //first bits in number ( 11110000 )
int data = 0;  //later bits in number ( 00001111 )

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

//Motor declarations
Servo base;
Servo shoulder;
Servo elbow;
Servo wrist;
Servo wrist_rotate;
Servo gripper;


void setup(){ 
  // initialize i2c as slave
  Wire.begin(SLAVE_ADDRESS);

  // define callbacks for i2c communication
  Wire.onReceive(receiveData);
  Wire.onRequest(sendData);
  
  //Set up digital outputs
  pinMode(indicLED, OUTPUT);
  pinMode(L_DRIVE_1, OUTPUT);
  pinMode(L_DRIVE_2, OUTPUT);
  pinMode(R_DRIVE_1, OUTPUT);
  pinMode(R_DRIVE_2, OUTPUT);

  //seed random
  randomSeed(analogRead(5));
  
  //enable servos
  base.attach(BASE_PWM);
  shoulder.attach(SHOULDER_PWM);
  elbow.attach(ELBOW_PWM);
  gripper.attach(GRIPPER_PWM);
  wrist_rotate.attach(W_ROTATE_PWM);
  wrist.attach(WRIST_PWM);
  
  //begin serial transmission
  Serial.begin(9600);
  
  //optional
  Home();
}

void loop(){  
  digitalWrite(indicLED, HIGH);
  
  commands[id] = data;
  
  //driving//
  if (commands[1]==1){//forward
    drive(1,1) ;
  }else if (commands[1]==2){//backward
    drive(-1,-1);
  }else if (commands[1]==3){//left
    drive(-1,1); 
  }else if (commands[1]==4){//right
    drive(1,-1);
  }else{//stopped
    drive(0,0); 
  }
  //gripper//
  float current_position = gripper.read();
  float new_position = current_position;
  float spd = 1;
  //Serial.println(current_position);
  if(commands[2]==1){
    new_position -= spd;
  }
  if (commands[2]==2){
    new_position += spd;
  }
  //Serial.println(new_position);
  delay(10);
  gripper.write(constrain(new_position, 0.0, 180.0));
  
}

// callback for received data
void receiveData(int byteCount){
  while(Wire.available()) {
    number = Wire.read();
  }
   
  number = byte(number);
  id = (number&byte((256-pow(2,DATA_BITS)))) >> DATA_BITS;//read only the first bits(The id)
  data = number & byte((pow(2,DATA_BITS)-1));
  /*
  Serial.print("data received: ");
  Serial.print(number);
  Serial.print("     ID:");
  Serial.print(id);
  Serial.print("     Data:");
  Serial.println(data);
  */
}

// callback for sending data
void sendData(){
    Wire.write(has_object());
}

void Home(){
  //set the robot to a resting position
  set(shoulder, 80, 1); 
  set(elbow, 100, 1);
  set(base, 100, 1);
  set(wrist, 10, 1);
  set(wrist_rotate, 90, 1);
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

