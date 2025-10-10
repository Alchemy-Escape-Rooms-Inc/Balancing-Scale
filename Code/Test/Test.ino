#include <SoftwareSerial.h>

SoftwareSerial rSerial(10,11);

void setup(){
  Serial.begin(9600);
  rSerial.begin(9600);

  Serial.println("Program started.");
}

void loop(){
  if(rSerial.available()){
    Serial.println("Detected card!");
  }
}
