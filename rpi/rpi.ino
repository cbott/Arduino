#include <Wire.h>

#define SLAVE_ADDRESS 0x04

static const byte DATA_BITS = 4;//bits used as data (8-id bits)

int number;
int state = 0;
void setup() {
    pinMode(13, OUTPUT);
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
}

// callback for received data
void receiveData(int byteCount){
    while(Wire.available()) {
        number = Wire.read();

        if (number == 1){
 
            if (state == 0){
                digitalWrite(13, LOW); // set the LED on
                state = 1;
            }
            else{
                digitalWrite(13, HIGH); // set the LED off
                state = 0;
            }
         }
     }
     number = byte(number);
     Serial.print("data received: ");
     Serial.print(number);
     Serial.print("     ID:");
     Serial.print((number&byte((256-pow(2,DATA_BITS)))) >> DATA_BITS);//read only the first bits(The id)
     Serial.print("     Data:");
     Serial.println(number & byte((pow(2,DATA_BITS)-1)));
}

// callback for sending data
void sendData(){
    Wire.write(number);
}
