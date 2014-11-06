
void setup(){
  pinMode(8, INPUT);
  pinMode(9, INPUT);
  pinMode(10, INPUT);
  pinMode(11, INPUT);
  Serial.begin(9600);
}
void loop(){
 if (digitalRead(8)){
   Serial.println("8"); 
 } 
 if (digitalRead(9)){
   Serial.println("9");
 } 
 if (digitalRead(10)){
   Serial.println("10"); 
 }
 if (digitalRead(11)){
   Serial.println("11"); 
 }
 delay(1000);
}
