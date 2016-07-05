#include <IRLib.h>

#define PROTOCOL NEC
#define BTN_SETUP 0x1FE48B7
#define BTN_UP 0x1FE58A7
#define BTN_POWER 0x1FE7887
#define BTN_LEFT 0x1FE807F
#define BTN_ENTER 0x1FE40BF
#define BTN_RIGHT 0x1FEC03F
#define BTN_EXIT 0x1FE20DF
#define BTN_DOWN 0x1FEA05F
#define BTN_MUTE 0x1FE609F
#define BTN_VOL_DOWN 0x1FEE01F
#define BTN_VOL_UP 0x1FE906F
#define BTN_ZOOM 0x1FE50AF
#define BTN_ROTATE 0x1FED827
#define BTN_SLIDE 0x1FEF807
#define BTN_HOLD 0xFFFFFFFF

const int SIGNAL_PIN = 11;
const int RELAY_PIN = 9;

IRrecv receiver(SIGNAL_PIN);
IRdecode decoder;

int state = 0;
void setup() {
	Serial.begin(9600);
	receiver.enableIRIn();
	pinMode(RELAY_PIN, OUTPUT);
}

void loop() {
  if (receiver.GetResults(&decoder)) {
    decoder.decode();		//Decode the data
    if(decoder.decode_type == NEC){
    	switch(decoder.value){
    		case BTN_POWER: //power
    		  Serial.println("power pressed");
    		  state = !state;
    		  break;
    		case BTN_UP:
    		  Serial.println("Up");
    		  break;
    		case BTN_DOWN:
    		  Serial.println("Down");
    		  break;
    		case BTN_LEFT:
    		  Serial.println("Left");
    		  break;
    		case BTN_RIGHT:
    		  Serial.println("Right");
    		  break;
    		case BTN_HOLD:
    		  Serial.println("Holding button");
    		  break;
    		default:
    		  decoder.DumpResults();
    	}
    } else {
    	decoder.DumpResults();	//Show the results on serial monitor
    }
    receiver.resume(); 		//Restart the receiver
  }
  digitalWrite(RELAY_PIN, state);
}
