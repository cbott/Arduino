const int EMIT_PIN = 10;
const int DETECT_PIN = A0;

void setup() {
  pinMode(EMIT_PIN, OUTPUT);
  
  Serial.begin(9600);
  analogWrite(EMIT_PIN, 150);
}

void loop() {
  int total = 0;
  int readings;
  for(readings=0; readings < 3; readings++){
    total += analogRead(DETECT_PIN);
    delay(50);
  }
  int val = int(total / readings);
  /*
  for(int i=0; i < val - 30; i++){
    Serial.print("|");
  }*/
  Serial.println(val);
  //delay(100);

}
