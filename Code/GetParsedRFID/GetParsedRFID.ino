#include <SoftwareSerial.h>

SoftwareSerial rSerial(9,10);

const int tagLen = 16;
const int idLen = 13;

char newTag[idLen];



void idPrintOut(char id[]){
  Serial.print("{ ");
  for(int i = 0; i < idLen; i++){
    Serial.print("0x");
    Serial.print((int)id[i]);
    if(i < (idLen-1))
      Serial.print(", ");
  }
  Serial.println(" }");
}


void listen(){
  int readByte;
  int i = 0;
  bool tag = (rSerial.available() == tagLen) ? true : false;

  if(!tag)
    return;

  while(rSerial.available()){
    readByte = rSerial.read();
    if (readByte != 2 && readByte!= 13 && readByte != 10 && readByte != 3)
      newTag[i++] = readByte;
  }


  Serial.print("Tag #: ");
  idPrintOut(newTag);

  for(int i = 0; i < idLen; i++)
    newTag[i] = 0;
}

void setup(){
  Serial.begin(9600);
  rSerial.begin(9600);

  Serial.println("Program started.");
}

void loop(){
  listen();
}

