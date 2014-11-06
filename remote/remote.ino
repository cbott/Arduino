
#include <IRremote.h>
const int numberOfKeys = 5;
const int firstKey = 4;

boolean buttonState[numberOfKeys];
boolean lastButtonState[numberOfKeys];
long irKeyCodes[numberOfKeys]={
  0x18E758A7,
  0x18E708F7,
  0x18E78877,
  0x18E748B7,
  0x18E7C837
};

IRsend irsend;

void setup()
{
  for (int i = 0; i<numberOfKeys; i++){
    buttonState[i] = true;
    lastButtonState[i] = true;
    int physicalPin = i+firstKey;
    pinMode(physicalPin, INPUT);
    digitalWrite(physicalPin, HIGH);
  }
  Serial.begin(9600);
}

void loop() {
  if (Serial.read() != -1) {
    for (int i = 0; i < 3; i++) {
      irsend.sendSony(0xa90, 12); // Sony TV power code
      delay(40);
    }
  }
}

