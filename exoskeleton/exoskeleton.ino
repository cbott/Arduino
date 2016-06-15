const int upButtonPin = 6;
const int downButtonPin = 7;

const int relayUp = 8;
const int relayDown = 9;

void setup() {
	pinMode(relayUp, OUTPUT);
	pinMode(relayDown, OUTPUT);

	pinMode(upButtonPin, INPUT);
	pinMode(downButtonPin, INPUT);

	Serial.begin(9600);
}

void loop() {
	bool upState   = !digitalRead(upButtonPin);
	bool downState = !digitalRead(downButtonPin);

	if(!(upState^downState)){
		halt();
	} else if(upState){
		moveUp();
	} else {
		moveDown();
	}
	delay(1);
}

void moveUp(){
	digitalWrite(relayUp, HIGH);
	digitalWrite(relayDown, LOW);
}
void moveDown(){
	digitalWrite(relayUp, LOW);
	digitalWrite(relayDown, HIGH);
}
void halt(){
	digitalWrite(relayUp, LOW);
	digitalWrite(relayDown, LOW);
}