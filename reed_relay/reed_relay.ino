const int SWITCHPIN = 3;

void setup() {
  pinMode(SWITCHPIN, OUTPUT);
}

void loop() {
  digitalWrite(SWITCHPIN, HIGH);
  delay(2000);
  digitalWrite(SWITCHPIN, LOW);
  delay(2000);
}
