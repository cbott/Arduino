#ifndef AnalogJoystick_h
#define AnalogJoystick_h

#include "Arduino.h"
class AnalogJoystick{
	public:
		AnalogJoystick(int horzPin, int vertPin, int buttonPin);
		float GetX();
		float GetY();
                boolean GetPressed();
	private:
		int bpin;
		int xpin;
		int ypin;
};
#endif
