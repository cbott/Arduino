//keyboardEmulator.ino
#include "Keyboard.h"

const int LED_PIN = 13;

void setup(){
  pinMode(LED_PIN, OUTPUT);
  Keyboard.begin();
}

const char *message = "Hello World";
void loop(){
  delay(500);
  Keyboard.press(KEY_LEFT_GUI);
  delay(1);
  Keyboard.press('r');
  Keyboard.releaseAll();
  delay(200);
  Keyboard.println("notepad");
  delay(500);

  for(int i=0; i<11; ++i){
    Keyboard.print(message[i]);
    delay(50);
  }
  Keyboard.println();

  while(1){
    digitalWrite(LED_PIN, HIGH);
    delay(1000);
    digitalWrite(LED_PIN, LOW);
    delay(1000);
  }
}
// const int BTN_PIN = 14;

// const int LINES = 3;
// const char *text[] = {"Line 1", "Line 2", "Line 3"};

// void setup() {
//   // put your setup code here, to run once:
//   pinMode(LED_PIN, OUTPUT);
//   pinMode(BTN_PIN, INPUT);
//   Keyboard.begin();
// }

// bool prev_state = false;
// void loop() {
//   bool cur_state = digitalRead(BTN_PIN);
//   if(cur_state && !prev_state){
//     digitalWrite(LED_PIN, HIGH);
//     hack();  
//   } else {
//     digitalWrite(LED_PIN, LOW);
//   }
//   prev_state = cur_state;
//   delay(1);
// }

// void hack(){
//   Keyboard.press(KEY_LEFT_CTRL);
//   Keyboard.press(KEY_LEFT_ALT);
//   Keyboard.press('t');
//   delay(10);
//   Keyboard.releaseAll();
//   delay(500);
//   Keyboard.print("Hello World");
// }

// void printLines(){
//   for(int i = 0; i < LINES; ++i){
//     Keyboard.print(text[i]);
//     Keyboard.press(KEY_LEFT_CTRL);
//     delay(10);
//     Keyboard.press(KEY_RETURN);
//     Keyboard.releaseAll();
//   }
//   Keyboard.println();
// }
