
int PIN = 8;

void setup() {
  pinMode(PIN, OUTPUT);
  randomSeed(analogRead(A4));
  Serial.begin(9600);
}

long del = 500;
void loop() {

	digitalWrite(PIN, HIGH);
	delay(del);
	digitalWrite(PIN, LOW);
	delay(del);

	int rec = 0;
	if(Serial.available() > 0){
		rec = Serial.parseInt();
		Serial.println(rec);
	}
	if(rec != 0)
		del = rec;
}
