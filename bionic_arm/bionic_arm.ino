const int SIG_A = 8;
const int SIG_B = 9;

const int IN1 = 10;
const int IN2 = 11;

void setup(){
  pinMode(SIG_A, OUTPUT);
  pinMode(SIG_B, OUTPUT);
  
  pinMode(IN1, INPUT);
  pinMode(IN2, INPUT);
  
  Serial.begin(9600);
}

void loop(){
//  if (digitalRead(IN1)){
//    digitalWrite(SIG_A, HIGH);
//    digitalWrite(SIG_B, LOW);
//  } else if (digitalRead(IN2)){
//    digitalWrite(SIG_B, HIGH);
//    digitalWrite(SIG_A, LOW);
//  } else {
//    digitalWrite(SIG_A, LOW);
//    digitalWrite(SIG_B, LOW);
//  }
//  
//  Serial.print(digitalRead(IN1));
//  Serial.print(", ");
//  Serial.println(digitalRead(IN2));
//  delay(100);
  digitalWrite(SIG_A, HIGH);
  digitalWrite(SIG_B, LOW);
  delay(500);
  digitalWrite(SIG_A, LOW);
  digitalWrite(SIG_B, LOW);
  delay(500);
  digitalWrite(SIG_B, HIGH);
  digitalWrite(SIG_A, LOW);
  delay(500);
  digitalWrite(SIG_A, LOW);
  digitalWrite(SIG_B, LOW);
  delay(500);

}
