#include <SoftwareSerial.h>

#define NUM_VALID_IDS 
#define RED_LED 6
#define GREEN_LED 7


//---------DATA STRUCTURES---------------
struct IDStorage {
  char IDs[];
  int index = 0;
}

//-----------GLOBAL VARIABLES--------------
const int tagLen = 16;
const int idLen = 13;
const char validIDs[] = {};

SoftwareSerial rSerial1(2,3);
SoftwareSerial rSerial2(4,5);


//-----------FUNCTIONS PROTOTYPES------------
void _init();
void gpio_init();


void listen(const SoftwareSerial & rSerial);
void checkForSuccess(const IDStorage & s1, const IDStorage & s2);




bool isAMatchingID(char id1[],char id2[]);
bool isAValidID(char id[]);
bool storeID(char id[], char  storage[]);





//---------------SETUP SR------------
void setup(){
  rSerial1.begin(9600);
  rSerial2.begin(9600);
}

//-----------MAIN LOOP SR-----------
void loop(){
  listen(rSerial1);
  listen(rSerial2);
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
void listen(const SoftwareSerial & rSerial){
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
}

bool isAMatchingID(char id1[], char id2[]){
  for(int i = 0; i < idLen; i++)
    if(id1[i] != id2[i]) 
      return false;
  return true;
}

bool isAValidID(char id[]){
    
}
bool storeID(char id[], char  storage[]){
  for(int i = 0; i < tagLen; i++)
    storage[i] = id[i];
}

