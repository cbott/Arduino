#include <stdlib.h>
char number_array[5];

void setup(){
  Serial.begin(9600); 
}
void loop(){
  dtostrf(123.4, 4, 3, number_array);
//  String number = number_array;
//  Serial.println(number.length());
//  Serial.println(number);
//  Serial.println();
  int j=4;
  for(int i=0; i<j; i++){
    int num = number_array[i] - '0';
    Serial.print(num + 1);
    if(number_array[i+1] == '.'){
      Serial.print('.');
      i++;
      j++;
    }
    Serial.println();
  }
  while(true){

  }
}

