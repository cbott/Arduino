static const int LED1PINS[] = {22, 23, 24, 25, 26, 27, 28, 29};
static const int LED2PINS[] = {30, 31, 32, 33, 34, 35, 36, 37};
static const int SWITCHPIN = 13;

boolean prevBtnState;
boolean btnState;
unsigned long time;
unsigned long prevtime;

void setup() {
  for( int i = 0; i < 8; i++){
    pinMode(LED1PINS[i], OUTPUT);
    pinMode(LED2PINS[i], OUTPUT);
  } 
  pinMode(SWITCHPIN, INPUT);
  Serial.begin(9600);
  
  prevBtnState = 0;
  prevtime = 0;
}

void loop() {
  time = millis();
  btnState = digitalRead(SWITCHPIN);
  
  if(btnState && !prevBtnState){
    float timeChange = (time - prevtime)/1000.0;
    float spm = secTospm(timeChange);
    Serial.println(spm);
    dispNumber(spm, int(spm) != spm);
    prevtime = time;
  }
  prevBtnState = btnState;
  delay(10);
}

float secTospm(float seconds){
  float spm = 60 / seconds;
  float decimal = spm - int(spm);
  spm = int(spm);
  if(decimal > 0.25 && decimal < 0.75){
    spm += 0.5; 
  } else if (decimal > 0.75) {
    spm += 1; 
  }
  return spm;
}

void dispNumber(int number, boolean decimal){
  if(number > 99){
    number = 99;
  } 
  if(number < 0){
    number = 0; 
  }
  int firstDigit = int(number / 10);
  int secondDigit = number - 10 * firstDigit;
  if(firstDigit){
    dispDigit(firstDigit, false, LED1PINS); 
  } else {
    dispDigit(-1, false, LED1PINS); 
  }
  
  dispDigit(secondDigit, decimal, LED2PINS); 
}

void dispDigit(int digit, boolean decimal, const int *PinArray){
  //display 1 digit, if digit is not between 0 and 9, display nothing
  int digitCodes[10] = {B1110111, B0010010, B1011101, B1011011, B0111010, B1101011, B1101111, B1010010, B1111111, B1111010};
  byte states;
  if(digit >= 0 && digit <= 9){
    states = digitCodes[digit];
  } else {
    states = B0000000; 
  }
  
  for(int i = 0; i < 7; i++){
    digitalWrite(PinArray[6-i], bitRead(states, i));
  }
  if(decimal){
    digitalWrite(PinArray[7], HIGH);  
  } else {
    digitalWrite(PinArray[7], LOW); 
  }
}
