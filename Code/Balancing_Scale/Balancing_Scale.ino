#include <SPI.h>
#include <MFRC522.h>


//MACROS
#define RST_PIN 8
#define R1_SSA_PIN 10
#define R2_SSA_PIN 9

#define RED_LED 6
#define GREEN_LED 5

#define MAX_NUM_ID 6
#define UID_BYTE_SIZE 4


struct IDStorage {
  byte ids [MAX_NUM_ID][UID_BYTE_SIZE]; //instead of 2D array, 1D array
  uint8_t index;

  IDStorage() {
    //clear everything
    index = 0;
    //clear the array values
    for (uint8_t i = 0; i < MAX_NUM_ID; i++)
      for (uint8_t j = 0; j < UID_BYTE_SIZE; j++)
        ids[i][j] = 0;
  }
};


//GLOBAL VARIABLES
MFRC522 rc1(R1_SSA_PIN, RST_PIN);
MFRC522 rc2(R2_SSA_PIN, RST_PIN);

IDStorage plate1, plate2;
byte readID1[4], readID2[4];

/*
   D1553C3
   D3E0272E
   B91B283
   1BEC81
   8EC6C81
   1B83326
*/
const byte validID[MAX_NUM_ID][UID_BYTE_SIZE] = {
  {0x0D, 0x15, 0x53, 0xC3},
  {0xD3, 0xE0, 0x27, 0x2E},
  {0x0B, 0x91, 0xB2, 0x83},
  {0x1B, 0xEC, 0x81},
  {0x08, 0xEC, 0x6C, 0x81},
  {0x01, 0xB8, 0x33, 0x26}
};

byte defcard[][4] = {{0xC6, 0xFD, 0xC5, 0x32}, {0x32, 0xD7, 0x0F, 0xB}}; //authorized cards/ufid
int N = 2;




//PROTOTYPES
bool matchID(byte id1[], byte id2[]);
bool isValidID(byte id);
void storeID(byte id[], byte storage[]);
void checkForSuccess(IDStorage & s1, IDStorage & s2);
void printAuthorizedUFID();
void gpio_init();

int reader1(IDStorage & storage);
int reader2(IDStorage & storage);


void setup() {

  Serial.begin(9600);
  SPI.begin();

  rc1.PCD_Init(); //initialize reader 1
  rc2.PCD_Init(); //initialize reader 2
  rc1.PCD_DumpVersionToSerial(); //show details of reader 1
  rc2.PCD_DumpVersionToSerial(); //show details of reader 2
  gpio_init();

  Serial.println(F("Scan Access Card to see Details"));
}


//MAIN LOOP
void loop() {
  int a =  reader1(plate1);
  int b = reader2(plate2);
  checkForSuccess(plate1, plate2);

}


//PROTOTYPES DEFINITION
void gpio_init() {
  pinMode(GREEN_LED, OUTPUT); //led indicator for in valid weight
  pinMode(RED_LED, OUTPUT); //led indicator for valid weight
}

void printAuthorizedUFID() {
  Serial.println("All authorized ID:");
  for (int i = 0; i < N; i++) {
    Serial.print(i + 1);
    Serial.print("  ");
    for (int j = 0; j < UID_BYTE_SIZE; j++) {
      Serial.print(defcard[i][j], HEX);
    }
    Serial.println("");
  }
}

bool matchID(byte id1[], byte id2[]) {
  for (uint8_t i = 0; i < UID_BYTE_SIZE; i++) {
    if (id1[i] != id2[i]) {
      return false; //if any of the 4 corresponding byte index value doesn't match return false;
    }
  }
  return true;  //otherwise return true, we have a match
}

bool isValidID(byte id[]) {
  for (uint8_t i = 0; i < MAX_NUM_ID; i++) {
    if (matchID(id, validID[i])) {
      return true;
    }
  }
  return false;
}

void storeID(byte id[], byte storage[]) {
  for (uint8_t i = 0; i < UID_BYTE_SIZE; i++) {
    storage[i] = id[i];
  }
}

void checkForSuccess(IDStorage & s1, IDStorage & s2) {
  bool success = false;
  if ((s1.index > 2) && (s2.index > 2)) {
    success = true;
  }
  if (!success) {
    digitalWrite(GREEN_LED, LOW);
    digitalWrite(RED_LED, HIGH);
  } else {
    digitalWrite(GREEN_LED, HIGH);
    digitalWrite(RED_LED, LOW);

  }
}
int reader1(IDStorage & storage) {

  if (!rc1.PICC_IsNewCardPresent()) {
    return 0;
  }
  if (!rc1.PICC_ReadCardSerial()) {
    return 0;
  }



  Serial.println("THE UID FROM THE FIRST READER:");

  for (int i = 0; i < UID_BYTE_SIZE; i++) {
    readID1[i] = rc1.uid.uidByte[i]; //storing the UID of the tag in readcard
    Serial.print(readID1[i], HEX);
  }

  /*
    if (isValidID(readID1)) {
    storeID(readID1, storage.ids[storage.index]);
    storage.index++;
    }
  */
  storeID(readID1, storage.ids[storage.index]);
  storage.index++;
  Serial.println("");
  rc1.PICC_HaltA();
  return 1;
}

int reader2(IDStorage & storage) {

  if (!rc2.PICC_IsNewCardPresent()) {
    return 0;
  }
  if (!rc2.PICC_ReadCardSerial()) {
    return 0;
  }


  Serial.println("THE UID FROM THE SECOND READER:");

  for (int i = 0; i < UID_BYTE_SIZE; i++) {
    readID2[i] = rc2.uid.uidByte[i]; //storing the UID of the tag in readcard
    Serial.print(readID2[i], HEX);
  }

  /*
    if (isValidID(readID2)) {
      storeID(readID1, storage.ids[storage.index]);
      storage.index++;
    }
  */
  storeID(readID1, storage.ids[storage.index]);
  storage.index++;
  Serial.println("");
  rc2.PICC_HaltA();
  return 1;
}
