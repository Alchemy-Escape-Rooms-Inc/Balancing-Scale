#include <SoftwareSerial.h>
#include <Servo.h>





//**************STRUCTURES ***************
struct storage {
  unsigned char id[13];
  storage():id{0,0,0,0,0,0,0,0,0,0,0,0,0}{}
  storage(char vID[]){
    addID(vID);
  }
  addID(char vID[]){
    for(int i = 0; i < 13; i++)
      id[i]  = vID[i];
  }
};

//************ GLOBAL VARIABLES ************
SoftwareSerial rSerial(9,10);
Servo myServo;

const int tagLen = 16;
const int idLen = 13;

char newTag[idLen];

unsigned long previousTime = 0;
const unsigned long intervalTime = 500;



storage s1;
//************** FUNCTIONS *****************

bool isAMatch(char id1[], char id2[]){
  for(int i = 0; i < 13; i++)
    if(id1[i] != id2[i])
      return false;
  return true;
}
bool isAlreadyStored(char id[], const storage & s){
  if(isAMatch(id, s.id))
    return true;
  return false;
}

void printID(char id[]){
  Serial.print("ID: ");
  for(int i = 0; i < idLen; i++)
    Serial.print(id[i]);
  Serial.println();
}

void clearTag(){
  for(int i = 0; i < idLen; i++)
    newTag[i] = 0;
}
void listen(){
  int readByte;
  int i = 0;
  /*
  bool tag = (rSerial.available() == tagLen) ? true : false;

  if(!tag)
    return;
  */
  while(rSerial.available()){
    readByte = rSerial.read();
    if (readByte != 2 && readByte!= 13 && readByte != 10 && readByte != 3)
      newTag[i++] = readByte;
  }

  printID(newTag);
  clearTag();
}

void program(){


  /*
  unsigned long currentTime = millis();
  if((currentTime - previousTime) >= intervalTime ){
      listen();
    previousTime = currentTime;
  }
  */

  myServo.write(0);
  delay(1000);
  myServo.write(90);
  delay(1000);
  myServo.write(180);
  delay(1000);
}


void setup(){
  Serial.begin(9600);
  rSerial.begin(9600);
  
  Serial.println("Program started.");

  myServo.attach(8,500,2500);

}

void loop(){
    program();
}
