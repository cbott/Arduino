#include "Arduino.h"
#include "AnalogJoystick.h"


AnalogJoystick::AnalogJoystick(int horizPin, int vertPin, int buttonPin){
	xpin = horizPin;
	ypin = vertPin;
	bpin = buttonPin;
        pinMode(bpin, INPUT);
        digitalWrite(bpin, HIGH);
}
float AnalogJoystick::GetX(){
	return -map(analogRead(xpin), 1, 1021, -100, 100)/100.0;
}
float AnalogJoystick::GetY(){
	return map(analogRead(ypin), 1, 1021, -100, 100)/100.0;
}
boolean AnalogJoystick::GetPressed(){
        return digitalRead(bpin) == false;
}
