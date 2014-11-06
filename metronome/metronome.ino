const int button1pin = 2;
const int button2pin = 3;
const int ledPin = 13;
const int buzzer = 9;

int b1State, b2State;

float bpm = 100;

void setup()
{
  pinMode(button1pin, INPUT);
  pinMode(button2pin, INPUT);
  pinMode(ledPin, OUTPUT);
  
  Serial.begin(9600);
}
void loop(){
  b1State = digitalRead(button1pin);
  b2State = digitalRead(button2pin);
  
  if(b1State == LOW){//b1 pressed
    bpm-=1; 
  }
  else if(b2State == LOW){
    bpm+=1;
  }
  
  Serial.println(round(bpm));
  
  digitalWrite(ledPin, HIGH); 
  tone(buzzer, 400, 50);
  digitalWrite(ledPin, LOW); 
  delay(round(60000 / round(bpm)));
  
}
