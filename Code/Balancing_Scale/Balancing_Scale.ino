#include <SoftwareSerial.h>

#define NUM_OF_SPICE_POUCHES 5
#define NUM_OF_COINS 20
#define NUM_VALID_IDS NUM_OF_SPICE_POUCHES + NUM_OF_COINS


//---------DATA STRUCTURES---------------
struct IDStorage {
  char id[idLen];
  float weight; 
}

//-----------GLOBAL VARIABLES--------------
const int tagLen = 16;
const int idLen = 13;



SoftwareSerial rSerial1(2,3);
SoftwareSerial rSerial2(4,5);

const IDStorage sPouches[NUM_OF_SPICE_POUCHES] = {
  //example of an initialization of IDs and weights 
  //ids             weights 
  {{0x00,0x11,0x22,0x33,0x44,0x55,0x66,0x77,0x88,0x99,0xAA,0xBB,0xCC}, 1.5},
  {{0xED,0xBC,0xDF,0x43,0x32,0x58,0xAC,0xFE,0xDA,0x9A,0xC8,0xCD,0xFF}, 2.5}
};

const IDStorage coins[NUM_OF_COINS] = {
  //example of an initialization of IDs and weights 
  //ids             weights 
  {{0x00,0x11,0x22,0x33,0x44,0x55,0x66,0x77,0x88,0x99,0xAA,0xBB,0xCC}, 0.75},
  {{0xED,0xBC,0xDF,0x43,0x32,0x58,0xAC,0xFE,0xDA,0x9A,0xC8,0xCD,0xFF}, 1}
};


//-----------FUNCTIONS PROTOTYPES------------

void listen(const SoftwareSerial & rSerial);
void checkForSuccess(const IDStorage & s1, const IDStorage & s2);
void storeID(char id[], IDStorage & storage);
void removeID(char id[], IDStorage & storage);

bool isAMatchingID(char id1[],char id2[]);
bool isAValidID(char id[]);
bool isAlreadyStored(char id[],const IDStorage & storage);

int findID(char id[], const IDStorage & storage);

//---------------SETUP SR------------
void setup(){
  rSerial1.begin(9600);
  rSerial2.begin(9600);
}

//-----------MAIN LOOP SR-----------
void loop(){
  run();
}


//-----------------FUNCTIONS DEFINITIONS --------------------------
void run(){
  listen(rSerial1 , true);
  listen(rSerial2 , false);
  checkForSuccess();
}

void listen(const SoftwareSerial & rSerial, bool isPlateForPouches){
  int rByte;  //capture bytes
  int i  = 0; //scope counter

  bool tag = (rSerial.available() == tagLen) ? true : false;  //Ensures the entire tag data is serial buffer
                                                              //if no tag is detected, leave
  if(!tag)
    return;

  while(rSerial.available()){
    rByte = rSerial.read();
    //Skip the first byte and the last 3, ASCII 2 (STX), ASCII 13 (CR), ASCII 10 (LF), & ASCII 3 (ETX)
    if (rByte != 2 && rByte!= 13 && rByte != 10 && rByte != 3)
      newTag[i++] = readByte;
  }
  
  //get a reference of the appropriate storage
  IDStorage & storage = (isPlateForPouches) ? sPouches : coins;
  
  if(!isAValidID(newTag, storage))
    return;
  if(isAlreadyStored(newTag, storage))
    return;
  storeID();
}

void checkForSuccess(){
  
}

void storeID(){
}
void removeID(char id[], IDStorage & storage){
}

bool isAMatchingID(char id1[], char id2[]){
  for(int i = 0; i < idLen; i++)
    if(id1[i] != id2[i])
      return false;
  return true;
}

bool isAValidID(char id[], const IDStorage & storage){
  for(int i = 0; i < NUM_VALID_IDS; i++ )
    if(isAMatchingID(id,storage.id[i]))
      return true;
  return false;
}

bool isAlreadyStored(char id[], const IDStorage & storage){
  for(int i  = 0; i < storage.index; i++)
    if(isAMatchingID(id,storage[i].id))
      return true;
  return false;
}


int findID(char id[], const IDStorage & storage){
}
