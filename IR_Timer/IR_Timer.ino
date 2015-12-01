const int THRESHHOLD = 60;

const int START_PIN = A0;
const int END_PIN = A1;

unsigned long starttime = 0;
unsigned long endtime = 0;
bool timing = false;

void setup() {
  Serial.begin(9600);
}

void loop() {
  int read1 = analogRead(START_PIN);
  int read2 = analogRead(END_PIN);
  /*
  for(int i=0; i < val - 30; i++){
    Serial.print("|");
  }*/
  /*
  Serial.print("Detector 1: ");
  Serial.print(read1);
  Serial.print("  Detector 2: ");
  Serial.println(read2);
  */
  
  if(read1 < THRESHHOLD && !timing){
    starttime = millis();
    Serial.println("Starting!!!");
    timing = true;
  }
  else if(read2 < THRESHHOLD && timing){
    endtime = millis();
    Serial.print("FINISH: ");
    Serial.print(endtime - starttime);
    Serial.println("ms");
    timing = false;
  }
  if(timing)
    Serial.println(millis()-starttime);
  
  delay(5);
}
