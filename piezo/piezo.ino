static const int BTNPIN = 8;
static const int BUZZPIN = 9;
static const int POTPIN = 0;

static const int STEP = 5;

void setup() {
  pinMode(BTNPIN, INPUT);
  pinMode(BUZZPIN, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  boolean val = digitalRead(BTNPIN);
  int pot = analogRead(POTPIN);
  int freq = map(pot, 150, 1023, 40, 2000);
  freq = freq / STEP * STEP;
  Serial.print(pot);
  Serial.print(" --> ");
  Serial.println(freq);
  if (!val){
    tone(BUZZPIN, freq);
  } else {
    noTone(BUZZPIN);
  }
}
