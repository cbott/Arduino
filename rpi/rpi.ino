/*
*
*Simple send/receive communication with raspberry pi
*
*/

#include <Wire.h>

#define SLAVE_ADDRESS 0x04

int RED_PIN = 9;
int GREEN_PIN = 10;
int BLUE_PIN = 11;
int lights[] = {0,0,0};//store rgb value

int number;  //value received

void setup() {
    pinMode(RED_PIN, OUTPUT);
    pinMode(GREEN_PIN, OUTPUT);
    pinMode(BLUE_PIN, OUTPUT);
    digitalWrite(RED_PIN, HIGH);
    digitalWrite(GREEN_PIN, HIGH);
    digitalWrite(BLUE_PIN, HIGH);
    
    Serial.begin(9600);         // start serial for output
    // initialize i2c as slave
    Wire.begin(SLAVE_ADDRESS);
 
    // define callbacks for i2c communication
    Wire.onReceive(receiveData);
    Wire.onRequest(sendData);
 
    Serial.println("Ready!");
}
 
void loop() {
  delay(10);
  
  analogWrite(RED_PIN, 255-lights[0]*3); 
  analogWrite(GREEN_PIN, 255-lights[1]*3);
  analogWrite(BLUE_PIN, 255-lights[2]*3);    
}

// callback for received data
void receiveData(int byteCount){
     while(Wire.available()) {
        number = Wire.read();
     }
     
     number = byte(number);
     Serial.print("data received: ");
     Serial.println(number);
     
     if(number==0){
       for(int i=0; i<3; i++){
         lights[i]=0; 
       }
     }else if (number <= 85){
       lights[0] = number;  
     }else if (number <= 170){
       lights[1] = number-85; 
     }else{
       lights[2] = number-170; 
     }
}

// callback for sending data
void sendData(){
    Wire.write(number);
}
