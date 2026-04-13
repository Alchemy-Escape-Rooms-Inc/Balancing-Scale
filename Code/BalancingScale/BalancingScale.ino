#include <SoftwareSerial.h>
#include <Servo.h>


#define TAG_LENGTH 16
#define ID_LENGTH 13
#define NUM_OF_SPICE_POUCHES 5
#define NUM_OF_COINS 20
#define RST_BT 4
#define CHK_BT 5
#define SERVO_PIN


//**************** DATA STRUCTURES ***********************

/**
 * @brief A struct/container for holding a pair of values, 
 *        the RFID(ID) and the represented weight.
 * @tparam N The maximum number of elements to 
 *           represent the ID received from the RFID. 
 */

template<byte N>
struct IDStorage {
  float weight;       //Represented weight of the RFID object.
  char id[ID_LENGTH]; //ID of the RFID object. 

  /**
   * @brief Default constructor, which initialize all values to 0.
   */
  IDStorage()
    : weight(0.0), id{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 } {}
  /**
   * @brief Constructor, which initialize the members using  
   *        the arguments.
   * @param vWeight Weight value to be assign to RFID the 
   *                object.
   * @param &vID A constant array of char literals of size N. 
   */
  IDStorage(float vWeight, const char (&vID)[N])
    : weight(vWeight) {
      for (int i = 0; i < ID_LENGTH; i++)
        id[i] = vID[i];
    }

  /**
   * @brief Constructor, which initialize the members using  
   *        the arguments.
   * @param vWeight Weight value to be assign to RFID object.
   * @param vID An array of char. 
   */
  IDStorage(float vWeight, char vID[]):weight(vWeight) {
    for (int i = 0; i < ID_LENGTH; i++)
      id[i] = vID[i];
  }
};

//COINS
/**
 * @brief A derived struct of the IDStorage struct to
 *        hold values specifically for the RFID coins
 *        object demonations.
 * @tparam N The maximum number of elements to 
 *           represent the ID received from the RFID. 
 */
template<byte N>
struct Coin : IDStorage<N> {
  /**
   * @brief Constructor, which initialize the members using 
   *        the arguments.
   * @param vWeight Weight value to be assign to the RFID 
   *                object.
   * @param &vID A constant array of char literals of size N.
   */
  Coin(float vWeight, const char (&vID)[N])
    : IDStorage<N>(vWeight, vID) {}
};

template<byte N>
struct GoldDoubloon : Coin<N> {
  GoldDoubloon(const char (&vID)[N])
    : Coin<N>(1.0, vID) {}
};
template<byte N>
struct HalfDoubloon : Coin<N> {
  HalfDoubloon(const char (&vID)[N])
    : Coin<N>(0.5, vID) {}
};
template<byte N>
struct PieceOfEight : Coin<N> {
  PieceOfEight(const char (&vID)[N])
    : Coin<N>(0.75, vID) {}
};
template<byte N>
struct QuarterDoubloon : Coin<N> {
  QuarterDoubloon(const char (&vID)[N])
    : Coin<N>(0.25, vID) {}
};

//POUCH
template<byte N>
struct Pouch : IDStorage<N> {
  Pouch(float vWeight, const char (&vID)[N])
    : IDStorage<N>(vWeight, vID) {}
};

//PLATES

/**
 * @brief A struct/container to hold a few values, for 
 *        keeping track of the RFID objects placed on the 
 *        scale's plates. 
 * @tparam N The maximum number of IDs that can stored for 
 *           this struct's object.
 */
template<byte N>
struct ScalePlates {
  IDStorage<ID_LENGTH> storage[N];  //An array of the IDs stored 
  byte index = 0;                   //Value to keep track of the amount of stored IDs
  float plateWeight = 0.0;          //Represents the total amount of RFID objects placed on the scale's plate.

  /**
   * @brief Adds a new RFID object to the storage.
   * @param vWeight Weight value of the RFID object.
   * @param vID The ID of the RFID object.
   */
  void addNewStorage(float vWeight, char vID){
    if((index + 1) > N)
      return;
    storage[index] = IDStorage<ID_LENGTH>(vWeight,vID);
    plateWeight += vWeight;
    index++;
  }
  
  /**
   * @brief Removes a RFID object from the storage. 
   * @param rIndex Index of RFID object that is to be 
   *               removed from the storage.
   */
  void removeStorage(int rIndex){
    if((index-1) < 0)
      return;
    float weight = storage[rIndex].weight;
    shiftStorageLeft(rIndex);
    clearStorage(index-1);
    plateWeight -= weight;
    index--;
  }

  /**
   * @brief Shifts all the RFID object information in 
   *        the array storage to the left overriding 
   *        one of stored objects.
   * @param sIndex Index of the stored object to be 
   *               overriden by the left shift.
   */
  void shiftStorageLeft(int sIndex){
    for(int i = sIndex; i < index; i++){
      for(int j = 0; j < ID_LENGTH; j++){
        if((i + 1) != index)
          storage[i].id[j] = storage[i+1].id[j];
      }
      if((i + 1) != index)
        storage[i].weight = storage[i+1].weight;
    }
  }

  /**
   * @brief Clears/Sets all the RFID object information
   *        to 0. 
   * @param cIndex Index of the stored object information 
   *               to be cleared.
   */
  void clearStorage(int cIndex){
    for(int i  = 0; i < ID_LENGTH; i++)
      storage[cIndex].id[i] = 0;
    storage[cIndex].weight = 0.0;
  }
};

//*************gg***** GLOBAL VARIABLES ****************************
char newTag[ID_LENGTH];
SoftwareSerial rSerial1(9, 10);
SoftwareSerial rSerial2(11, 12);
Servo mServo;

// const Pouch<ID_LENGTH> sPouches[NUM_OF_SPICE_POUCHES] = {
//   Pouch<ID_LENGTH>({ 0xED, 0xBC, 0xDF, 0x43, 0x32, 0x58, 0xAC, 0xFE, 0xDA, 0x9A, 0xC8, 0xCD, 0xFF }),
//   Pouch<ID_LENGTH>({ 0xED, 0xCD, 0xDF, 0x43, 0x32, 0x45, 0xAC, 0xFE, 0x23, 0x9A, 0xC8, 0x45, 0xFF })
// };


// const Coin<ID_LENGTH> coins[NUM_OF_COINS] = {
//   //example of initialization of coins
//   GoldDoubloon<ID_LENGTH>({ 0xED, 0xBC, 0xDF, 0x43, 0x32, 0x58, 0xAC, 0xFE, 0xDA, 0x9A, 0xC8, 0xCD, 0xFF }),
//   HalfDoubloon<ID_LENGTH>({ 0xED, 0xCD, 0xDF, 0x43, 0x32, 0x45, 0xAC, 0xFE, 0x23, 0x9A, 0xC8, 0x45, 0xFF })

// };

//Known IDs for the pouches and their represented weights. 
const Pouch<ID_LENGTH> sPouches[2] = {
  Pouch<ID_LENGTH>(3.0, { 0xED, 0xBC, 0xDF, 0x43, 0x32, 0x58, 0xAC, 0xFE, 0xDA, 0x9A, 0xC8, 0xCD, 0xFF }),
  Pouch<ID_LENGTH>(4.5, { 0xED, 0xCD, 0xDF, 0x43, 0x32, 0x45, 0xAC, 0xFE, 0x23, 0x9A, 0xC8, 0x45, 0xFF })
};

//Known IDs for the coins and their represented weights. 
const Coin<ID_LENGTH> coins[2] = {
  //example of initialization of coins
  GoldDoubloon<ID_LENGTH>({ 0xED, 0xBC, 0xDF, 0x43, 0x32, 0x58, 0xAC, 0xFE, 0xDA, 0x9A, 0xC8, 0xCD, 0xFF }),
  HalfDoubloon<ID_LENGTH>({ 0xED, 0xCD, 0xDF, 0x43, 0x32, 0x45, 0xAC, 0xFE, 0x23, 0x9A, 0xC8, 0x45, 0xFF })

};

//Objects to store the detected tag IDs based on their type.
ScalePlates<NUM_OF_COINS> coinsPlate;
ScalePlates<NUM_OF_SPICE_POUCHES> pouchesPlate;



//*************************  FUNCTIONS *******************************
/**
 * @brief This function rotates the angle to 
 *        0 degrees. Representing Yay. 
 */
void servoYay(){
  mServo.write(0);
}
/**
 * @brief This function rotates the angle to 
 *        90 degrees. Representing Nay.
 */
void servoNay(){
  mServo.write(90);
}

/**
 * @brief This function rotates the angle to 
 *        45 degrees. Representing Nay.
 */
void servoMid(){
  mServo.write(45);
}
/**
 * @brief This function checks to see if the reset 
 *        button has been pressed. 
 */
void rst_bt(){
  if(!digitalRead(RST_BT))
    return; 
  clearAllParameters();

}
/**
 * @brief This function checks to see if the reset 
 *        button has been pressed. 
 */
void chk_bt(){
  if(!digitalRead(CHK_BT))
    return; 
  checkSuccess();

}
/**
 * @brief This function resets all the scale's stored 
 *        data. 
 */
void clearAllParameters(){
  for(int i = 0; i < coinsPlate.index; i++)
    coinsPlate.clearStorage(i);
  for(int i = 0; i < pouchesPlate.index; i++)
    pouchesPlate.clearStorage(i);
  
  coinsPlate.plateWeight = 0.0;
  pouchesPlate.plateWeight = 0.0;

  coinsPlate.index = 0;
  pouchesPlate.index = 0;
}
/**
 * @brief This function checks if the user has success 
 *        balanced the weight with the items. 
 */
void checkSuccess(){


}
/**
 * @brief Verify if the array of chars are a match.
 *
 * This function verifies if the passed in IDs are 
 * identical.
 *
 * @param id1 An array of char representing the RFID(ID).
 * @param id2 An array of char representing the RFID(ID).
 * @return A boolean value, whether both IDs are identical.
 */
bool isAMatchingID(char id1[], char id2[]) {
  for (int i = 0; i < ID_LENGTH; i++)
    if (id1[i] != id2[i])
      return false;
  return true;
}
/**
 * @brief Validates an ID is a known ID.
 *
 * This function validates the passed in "id", by 
 * finding a match in one of the structures with 
 * the known IDs.
 *
 * @tparam N The maximum number of IDs that is stored
 *           in the IDStorage structure.
 * @param id An array of char representing the RFID(ID).
 * @param storage An object that holds the known RFID 
 *                IDs. 
 * @return A boolean value, whether the "id" is a
 *           matched to any of the known IDs.
 */
template<byte N>
bool idValidation(char id[], const IDStorage<N> & storage){
  return (isAMatchingID(id,storage.id)) ? true : false;
}
/**
 * @brief Checks to make sure an ID is one of the
 *        known IDs.
 *
 * This function checks if the passed in "id" is found
 * within one of the structures holding the known IDs.
 *
 * @param id An array of char representing the RFID(ID).
 * @param isPlateForPouches A boolean value representing
 *           which plate on the scale is to be used.
 * @return A boolean value, whether the "id" is a valid 
 *           /known ID. 
 */
bool isAValidID(char id[], bool isPlateForPouches) {
  if(isPlateForPouches){
    for(int i = 0; i < NUM_OF_SPICE_POUCHES; i++)
      if(idValidation(id,sPouches[i]))
        return true;
  }
  else
  {
    for(int i = 0; i < NUM_OF_COINS; i++)
      if(idValidation(id,coins[i]))
        return true;
  }
  return false;
}
/**
 * @brief Verifies if an ID is found in a plate object.
 *
 * This function verifies if the passed in "id" is within
 * the passed in plate object.
 *
 * @tparam N The maximum number of IDs the plate can store.
 * @param id An array of char representing the RFID(ID).
 * @param plateStorage An object that was created to store
 *           the detected IDs. It will be used to verify
 *           passed in "id".
 * @return A boolean value, whether the "id" is in the 
 *           plate object.
 */
template<byte N>
bool storageVerification(char id[], const ScalePlates<N> & plateStorage){
  for(int i = 0; plateStorage.index; i++)
    if(isAMatchingID(id,plateStorage.storage[i].id))
      return true;
  return false;
}
/**
 * @brief Checks if an ID was already stored.
 *
 * This function checks in the passed in "id" is already
 * stored within one of the plate's storage.
 *
 * @param id An array of char representing the RFID(ID).
 * @param isPlateForPouches A boolean value representing
 *           which plate on the scale is to be used.
 * @return A boolean value, whether "id" was already
 *           stored.
 */
bool isAlreadyStored(char id[], bool isPlateForPouches) {
  if(isPlateForPouches)
    storageVerification(id,pouchesPlate);
  else
    storageVerification(id,coinsPlate);
  return false;
}
/**
 * @brief Finds the location of an ID in a plate's storage.
 *
 * This function searches for the passed in "id" within the
 * passed in plate's object.
 *
 * @tparam N The maximum number of IDs the plate can store.
 * @param id An array of char representing the RFID(ID).
 * @param plateStorage The structure created to store the
 *           detected IDs. It will be used to searched for
 *           passed in "id".
 * @return Location/index of the "id" in the plate's storage.
 *           Returns -1, if "id" wasn't found.
 */
template<byte N>
int findID(char id[], const ScalePlates<N> & plateStorage){
  for(int i = 0; i < plateStorage.index; i++)
    if(isAMatchingID(id,plateStorage.storage[i].id))
      return i;
  return -1;
}
/**
 * @brief Retrieves the data from RFID reader.
 *
 * This function captures the data from the RFID readers,
 * when a RFID transmitter is in range or detected.
 *
 * @param rSerial An object of the SoftwareSerial data type,
 *                which listens to serial data on the
 *                instantiated hardware pins.
 * @param isPlateForPouches A boolean value representing
 *           which plate on the scale is to be used.
 */
void listen(const SoftwareSerial& rSerial, bool isPlateForPouches) {
  int rByte;  //capture bytes
  int i = 0;  //scope counter

  bool tag = (rSerial.available() == TAG_LENGTH) ? true : false;  //Ensures the entire tag data is serial buffer.

  //If there isn't a tag is detected, leave
  if (!tag)
    return;

  while (rSerial.available()) {
    rByte = rSerial.read();
    //Skip the first byte and the last 3, ASCII 2 (STX), ASCII 13 (CR), ASCII 10 (LF), & ASCII 3 (ETX)
    if (rByte != 2 && rByte != 13 && rByte != 10 && rByte != 3)
      newTag[i++] = rByte;
  }

  if (!isAValidID(newTag, isPlateForPouches))
    return;
  if (isAlreadyStored(newTag, isPlateForPouches))
    return;
  //auto& storage = (isPlateForPouches) ? pouchesPlate : coinsPlate;
}
/**
 * @brief The main components of this program.
 *
 * This functions contains all the instructions in the
 * program that will run in the main loop.
 */
void program() {
  listen(rSerial1 , true);
  listen(rSerial2 , false);
  rst_bt();
  chk_bt();
}

void setup() {
  //Serial setup
  rSerial1.begin(9600);
  rSerial2.begin(9600);
  //Servo setup
  mServo.attach(SERVO_PIN);
  //GPIO setup
  pinMode(RST_BT,INPUT);
  pinMode(CHK_BT,INPUT);
  digitalWrite(RST_BT,LOW);
  digitalWrite(CHK_BT,LOW);
}
void loop() {
  program();
}
