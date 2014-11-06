/*wiring:
5v-------pressure sensor--
                          |
                          |
analog in------------------
                          |
                          |
                     10k resistor
                          |
                          |
GND------------------------

*/

const int PIN = 0;

void setup(){ 
  Serial.begin(9600);
}
void loop(){
  int pressure_val = analogRead(PIN);
  Serial.println(pressure_val);
  delay(500); 
}
