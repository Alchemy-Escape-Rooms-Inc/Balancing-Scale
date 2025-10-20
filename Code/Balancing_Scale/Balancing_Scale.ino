#include <SoftwareSerial.h>

#define NUM_VALID_IDS 15
#define RED_LED 6
#define GREEN_LED 7


//---------DATA STRUCTURES---------------
struct IDStorage {
  char ids[][];
  int index = 0;
}

//-----------GLOBAL VARIABLES--------------
const int tagLen = 16;
const int idLen = 13;
const char validIDs[NUM_VALID_IDS] = {};

SoftwareSerial rSerial1(2,3);
SoftwareSerial rSerial2(4,5);

IDStorage plate1, plate2;

//-----------FUNCTIONS PROTOTYPES------------
void _init();
void gpio_init();

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
void _init(){
  gpio_init();
}
void gpio_init(){
  pinMode(GREEN_LED,OUTPUT);
  pinMode(RED_LED,OUTPUT);
  digitalWrite(GREEN_LED,LOW);
  digitalWrite(RED_LED,LOW);
}

void run(){
  listen(rSerial1, plate1);
  listen(rSerial2, plate2);
  checkForSuccess();
}

void listen(const SoftwareSerial & rSerial, IDStorage & storage){
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
  
  if(!isAValidID(newTag))
    return;
  if(isAlreadyStored(newTag, storage))
    return;
  storeID(newTag,storage);
}

void checkForSuccess(){
  
}

void storeID(char id[], IDStorage & storage){
  int index = storage.index;
  for(int i = 0; i < tagLen; i++)
    storage.ids[index][i] = id[i];
  storage.index++;
}

void removeID(char id[], IDStorage & storage){
  storage.index--;
}

bool isAMatchingID(char id1[], char id2[]){
  for(int i = 0; i < idLen; i++)
    if(id1[i] != id2[i])
      return false;
  return true;
}

bool isAValidID(char id[]){
  for(int i = 0; i < NUM_VALID_IDS; i++ )
    if(isAMatchingID(id,validIDs[i]))
      return true;
  return false;
}

bool isAlreadyStored(char id[], const IDStorage & storage){
  for(int i  = 0; i < storage.index; i++)
    if(isAMatchingID(id,storage.ids[i]))
      return true;
  return false;
}


int findID(char id[], const IDStorage & storage){
  for(int i = 0; i < storage.index; i++)
      if(isAMatchingID(id,storage.ids[i]))
        return i;
  return -1;
}
