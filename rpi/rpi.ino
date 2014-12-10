#include <Wire.h>

#define SLAVE_ADDRESS 0x04

static const byte DATA_BITS = 4;//bits used as data (8-id bits)

int RED_PIN = 8;
int GREEN_PIN = 9;
int BLUE_PIN = 10;
boolean lights[] = {0,0,0};//store rgb value

int number;//value received over i2c
int id;    //first bits in number ( 11110000 )
int data;  //later bits in number ( 00001111 )

int state = 0;
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
  if (lights[0]){
    digitalWrite(RED_PIN, LOW); 
  }else{
    digitalWrite(RED_PIN, HIGH); 
  }
  if (lights[1]){
    digitalWrite(GREEN_PIN, LOW);
  }else{
    digitalWrite(GREEN_PIN, HIGH);
  }
  if (lights[2]){
    digitalWrite(BLUE_PIN, LOW);
  }else{
    digitalWrite(BLUE_PIN, HIGH);
  }
    
}

// callback for received data
void receiveData(int byteCount){
     while(Wire.available()) {
        number = Wire.read();
     }
     
     number = byte(number);
     id = (number&byte((256-pow(2,DATA_BITS)))) >> DATA_BITS;
     data = number & byte((pow(2,DATA_BITS)-1));
     Serial.print("data received: ");
     Serial.print(number);
     Serial.print("     ID:");
     Serial.print(id);//read only the first bits(The id)
     Serial.print("     Data:");
     Serial.println(data);
     
     if (id == 1){
       lights[0] = data;  
     }else if (id == 2){
       lights[1] = data; 
     }else if (id == 3){
       lights[2] = data; 
     }
}

// callback for sending data
void sendData(){
    Wire.write(number);
}
