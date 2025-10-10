#include <SoftwareSerial.h>

SoftwareSerial rfid1(2,3);
SoftwareSerial rfid2(4,5);


//-----------FUNCTIONS PROTOTYPES------------
void listen(const SoftwareSerial & rfid);

bool isAMatchingID(unsigned int id1, unsigned int id2);

void setup(){
  rfid1.begin(9600);
  rfid2.begin(9600);
}

void loop(){
  listen(rfid1);
  listen(rfid2);
}

void listen(const SoftwareSerial & rfid){
  if(rfid.available()){
  }
}

bool isAMatchingID(unsigned int id1, unsigned int id2){

}
