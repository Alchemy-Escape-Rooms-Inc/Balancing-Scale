#include <SoftwareSerial.h>

SoftwareSerial rSerial(10,11);

 const int tagLen = 16; 
 const int idLen = 13;

char newTag[idLen];

void listen();

void setup(){
  Serial.begin(9600);
  rSerial.begin(9600);

  Serial.println("Program started.");
}

void loop(){
  listen();
}

void listen(){
  int readByte;
  int i = 0;
  bool tag = (rSerial.available() == tagLen) ? true : false;

  if(tag){
    while(rSerial.available()){
      readByte = rSerial.read();
      
      if (readByte != 2 && readByte!= 13 && readByte != 10 && readByte != 3) {
        newTag[i] = readByte;
        i++;
      }
      if(readByte == 3) 
        tag = false;
    }
  }
  if(strlen(newTag) == 0)
    return;

  Serial.print("Tag #: ");
  Serial.println(newTag);
  
  for(int i = 0; i < idLen; i++)
    newTag[i] = 0;
}
