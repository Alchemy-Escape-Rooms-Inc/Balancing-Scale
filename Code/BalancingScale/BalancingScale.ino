//=============================================================
//                        MACROS
//=============================================================
#pragma once

#include <WiFi.h>
#include <PubSubClient.h>
#include <HardwareSerial.h>
#include <ESP32Servo.h>

#define VERSION "1.3.0"

#define GAME_NAME "MermaidsTale"
#define PROP_NAME "BalancingScale"

#define MQTT_TOPIC          "MermaidsTale/BalancingScale"
#define MQTT_TOPIC_COMMAND  "MermaidsTale/BalancingScale/command"
#define MQTT_TOPIC_STATUS   "MermaidsTale/BalancingScale/status"
#define MQTT_TOPIC_LOG      "MermaidsTale/BalancingScale/log"
#define MQTT_TOPIC_MESSAGE  "MermaidsTale/BalancingScale/message"
#define MQTT_TOPIC_SYSTEM   "MermaidsTale/BalancingScale/system"


#define NUM_OF_SPICE_POUCHES 5
#define NUM_OF_COINS 8
#define SERVO_PIN 13

#define RX1 18
#define TX1 19

#define RX2 16
#define TX2 17
#define BAUD_RATE 115200
#define ID_LENGTH 4
#define MAX_READABLE 5                //RFIDs can only read 5 tags at once.

WiFiClient espClient;
PubSubClient mqttClient(espClient);
//=============================================================
//                      DATA STRUCTURE(S)
//=============================================================

/**
 * @brief A struct/container for holding a pair of values,
 *        the RFID(ID) and the represented weight.
 * @tparam N The maximum number of elements to
 *           represent the ID received from the RFID.
 */

template<byte N>
struct IDStorage {
  float weight;       //Represented weight of the RFID object.
  byte id[ID_LENGTH]; //ID of the RFID object.

  /**
   * @brief Default constructor, which initialize all values to 0.
   */
  IDStorage()
    : weight(0.0), id{ 0, 0, 0, 0 } {}
  /**
   * @brief Constructor, which initialize the members using
   *        the arguments.
   * @param vWeight Weight value to be assign to RFID the
   *                object.
   * @param &vID A constant array of char literals of size N.
   */
  IDStorage(float vWeight, const byte (&vID)[N])
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
  /*
     IDStorage(float vWeight,const char vID[]):weight(vWeight) {
     for (int i = 0; i < ID_LENGTH; i++)
     id[i] = vID[i];
     }
     */
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
  Coin(float vWeight, const byte (&vID)[N])
    : IDStorage<N>(vWeight, vID) {}
};

template<byte N>
struct GoldDoubloon : Coin<N> {
  GoldDoubloon(const byte (&vID)[N])
    : Coin<N>(1.0, vID) {}
};
template<byte N>
struct HalfDoubloon : Coin<N> {
  HalfDoubloon(const byte (&vID)[N])
    : Coin<N>(0.5, vID) {}
};
template<byte N>
struct PieceOfEight : Coin<N> {
  PieceOfEight(const byte (&vID)[N])
    : Coin<N>(0.75, vID) {}
};
template<byte N>
struct QuarterDoubloon : Coin<N> {
  QuarterDoubloon(const byte (&vID)[N])
    : Coin<N>(0.25, vID) {}
};

//POUCH
template<byte N>
struct Pouch : IDStorage<N> {
  String spice;
  Pouch(float vWeight, const byte (&vID)[N]):IDStorage<N>(vWeight, vID) {}
  Pouch(const String & vSpice, float vWeight, const byte (&vID)[N]):spice(vSpice),IDStorage<N>(vWeight, vID){}
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
  void addNewStorage(float vWeight,const byte (&vID)[ID_LENGTH]){
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


  void printStorage(){

    //if(index <= 0)
    //  return;
    /*
       for(int i = 0; i < index; i++){

       String uid = "";
       for(int j = 0; j < ID_LENGTH; j++)
       uid += String(storage[i].id[j],HEX) + String((j == ID_LENGTH-1) ? "":"-");

       mqttClient.publish(String(String(MQTT_TOPIC_SYSTEM) + "/storedUID" + String(i)).c_str(),uid.c_str());

       }
       */
    mqttClient.publish(String(String(MQTT_TOPIC_SYSTEM) + "/storedWeight").c_str(),String(plateWeight).c_str());
    mqttClient.publish(String(String(MQTT_TOPIC_SYSTEM) + "/storedIndex").c_str(),String(index).c_str());
  }
};

//=============================================================
//                      GLOBAL VARIABLES
//=============================================================

byte CMD_GET_TAG_COUNT[] = { 0xF5, 0x03, 0x00, 0xFC, 0xFF, 0x02, 0xB2, 0xC1 };
byte CMD_HALT[] = { 0xF5, 0x03, 0x00, 0xFC, 0xFF, 0x05, 0x55, 0xB1 };
byte CMD_REBOOT[] = { 0xF5, 0x03, 0x00, 0xFC, 0xFF, 0x0A, 0xBA, 0x40 };
byte CMD_DUMMY[] = { 0xF5, 0x03, 0x00, 0xFC, 0xFF, 0x01, 0xD1, 0xF1 };
byte CMD_GET_UID[5][9] = {
  { 0xF5, 0x04, 0x00, 0xFB, 0xFF, 0x03, 0x00, 0x5C, 0x48 },
  { 0xF5, 0x04, 0x00, 0xFB, 0xFF, 0x03, 0x01, 0x7D, 0x58 },
  { 0xF5, 0x04, 0x00, 0xFB, 0xFF, 0x03, 0x02, 0x1E, 0x68 },
  { 0xF5, 0x04, 0x00, 0xFB, 0xFF, 0x03, 0x03, 0x3F, 0x78 },
  { 0xF5, 0x04, 0x00, 0xFB, 0xFF, 0x03, 0x04, 0xD8, 0x08 }
};

Servo mServo;
HardwareSerial rfid1(1);
HardwareSerial rfid2(2);



byte uid[ID_LENGTH * MAX_READABLE];


const char* WIFI_SSID = "AlchemyGuest";
const char* WIFI_PASS = "VoodooVacation5601";

// MQTT broker
const char* MQTT_SERVER = "10.1.10.115";
const int MQTT_PORT = 1883;

bool servoState = false;    //true = yay, false = nay

float weights[5];     //array to hold the spices weight
bool success[5] = {false,false,false,false,false}; //array for tracking the correct spice pouch solved

bool puzzleSolved = false;

String incoming = "";


const unsigned long heartBeatPulse = 300 * 1000UL;  //heart beat for MQTT, every 5 minutes (WatchTower standard)
const unsigned long scanPeriod = 1000UL;      //scanning of the RFIDs and other associated operations, every second

unsigned long hLastTime = 0;
unsigned long sLastTime = 0;

//Known IDs for the pouches and their represented weights.
const Pouch<ID_LENGTH> sPouches[NUM_OF_SPICE_POUCHES] = {

  Pouch<ID_LENGTH>("Yeast",2.5f, (byte []){0xC1, 0x27, 0xFB,0x19}),
  Pouch<ID_LENGTH>("SugarCane",2.25f,  (byte []){0x60, 0x74, 0x15,0x21}),
  Pouch<ID_LENGTH>("Vanilla",1.75f,  (byte []){0x94, 0xD3, 0xC9,0x1}),
  Pouch<ID_LENGTH>("Molasses",2.75f,  (byte []){0x80, 0x1E, 0x7C,0x21}),
  Pouch<ID_LENGTH>("Cloves",1.5f,  (byte []){0x71, 0xD, 0xEB,0x17})
};

//Known IDs for the coins and their represented weights.
const Coin<ID_LENGTH> coins[NUM_OF_COINS] = {
  //example of initialization of coins
  GoldDoubloon<ID_LENGTH>({0x80, 0xD5, 0xCF,0x21}),
  GoldDoubloon<ID_LENGTH>({0x31, 0x61, 0x86,0x26}),
  HalfDoubloon<ID_LENGTH>({0x1B,0xF,0x39,0x6}),
  HalfDoubloon<ID_LENGTH>({0x19,0xF0,0x4F,0x6}),
  PieceOfEight<ID_LENGTH>({0x81,0xE8,0x50,0x6}),
  PieceOfEight<ID_LENGTH>({0xF1,0x76,0xC3,0x1}),
  QuarterDoubloon<ID_LENGTH>({0xE2,0x52,0xE3,0x19}),
  QuarterDoubloon<ID_LENGTH>({0x62,0xC7,0x38,0x6})
};

//Objects to store the detected tag IDs based on their type.
ScalePlates<NUM_OF_COINS> coinsPlate;
ScalePlates<NUM_OF_SPICE_POUCHES> pouchesPlate;

//=============================================================
//                    NETWORK & MQTT
//=============================================================

//WIFI NETWORK
void setupWiFi() {
  delay(1000);
  Serial.println("*********** WIFI ***********");
  Serial.print("Connecting to SSID: ");
  Serial.println(WIFI_SSID);

  WiFi.begin(WIFI_SSID,WIFI_PASS);

  while(WiFi.status() != WL_CONNECTED){
    delay(100);
    Serial.print("-");
  }
  Serial.println("\nConnected.");
}

//MQTT SERVER
void connectMQTT() {
  while (!mqttClient.connected()) {
    Serial.print("Connecting to MQTT...");

    String clientId = PROP_NAME;
    clientId += "_";
    clientId += String(random(0xffff), HEX);

    // LWT: broker publishes retained OFFLINE on /status if we drop
    if (mqttClient.connect(clientId.c_str(), MQTT_TOPIC_STATUS, 1, true, "OFFLINE")) {
      Serial.println("connected!");

      // Subscribe to command topic
      mqttClient.subscribe(MQTT_TOPIC_COMMAND);

      // Announce we're online
      mqttClient.publish(MQTT_TOPIC_STATUS, "ONLINE", true);
      mqttLogf("%s v%s online", PROP_NAME, VERSION);

    } else {
      Serial.printf("failed (rc=%d), retrying in 5s\n", mqttClient.state());
      delay(5000);
    }
  }
}

void mqttCallback(char* topic, byte* payload, unsigned int length) {
  char topicBuf[128];
  strncpy(topicBuf,topic,sizeof(topicBuf)-1);
  topicBuf[sizeof(topicBuf)-1] = '\0';

  char message[128];
  if(length >= sizeof(message)){
    length = sizeof(message) - 1;
  }

  memcpy(message,payload,length);
  message[length] = '\0';

  char * msg = message;
  while(*msg == ' ' || *msg == '\r' || *msg == '\n')
    msg++;
  char * end = msg + strlen(msg) -1;
  while(end > msg && (*end == ' ' || *end == '\t' || *end == '\r' || *end == '\n')){
    *end = '\0';
    end--;
  }

  //Serial.printf("[MQTT] Received on %s: %s\n", topicBuf,msg);

  if(strcmp(topicBuf,MQTT_TOPIC_COMMAND) != 0){
    return;
  }

  // Our own PONG/OK replies arrive back on /command (we subscribe to it).
  // Ignore them silently so they don't register as unknown commands.
  if(strcmp(msg,"PONG") == 0 || strcmp(msg,"OK") == 0 || strlen(msg) == 0){
    return;
  }

  if(strcmp(msg,"PING") == 0){
    // WatchTower standard: PONG on the topic the command arrived on
    mqttClient.publish(MQTT_TOPIC_COMMAND,"PONG");
    return;
  }
  if(strcmp(msg,"STATUS") == 0){
    publishDiagnostics();
    return;
  }
  if(strcmp(msg,"RESET") == 0){
    // WatchTower standard: respond OK, then software reboot
    mqttLogf("RESET: rebooting");
    mqttClient.publish(MQTT_TOPIC_COMMAND,"OK");
    mqttClient.loop();
    delay(250);
    ESP.restart();
    return;
  }
  if (strcmp(msg, "PUZZLE_RESET") == 0) {
    // Reset game state without rebooting
    puzzleReset();
    mqttClient.publish(MQTT_TOPIC_COMMAND, "OK");
    return;
  }
  if (strcmp(msg, "SOLVE") == 0) {
    // GM override: mark the puzzle solved
    puzzleSolved = true;
    updateServo(true);
    publishSolved();
    return;
  }

  if (strcmp(msg, "HALTCOIN") == 0) {
    haltReader(rfid2);
    mqttClient.publish(MQTT_TOPIC_MESSAGE, "Halting coins' plate rfid field.");
    return;
  }
  if (strcmp(msg, "HALTSPICE") == 0) {
    haltReader(rfid1);
    mqttClient.publish(MQTT_TOPIC_MESSAGE, "Halting spices' plate rfid field.");
    return;
  }
  if (strcmp(msg, "REBOOTCOIN") == 0) {
    rebootReader(rfid2);
    mqttClient.publish(MQTT_TOPIC_MESSAGE, "Rebooting coins' plate rfid.");
    delay(1000); //needs minimum 1 second boot time
    return;
  }
  if (strcmp(msg, "REBOOTSPICE") == 0) {
    rebootReader(rfid1);
    mqttClient.publish(MQTT_TOPIC_MESSAGE, "Rebooting spices' plate rfid.");
    delay(1000); //needs minimum 1 second boot time
    return;
  }
  mqttLogf("Unknown command: %s", msg);
}

/**
 * @brief Publishes SOLVED on /status. M3's "Balancing Scale Solved" event
 *        matches this exact payload and forwards MermaidsTale/ScaleSolved.
 *        Not retained: a retained SOLVED would replay into fresh sessions.
 */
void publishSolved(){
  mqttClient.publish(MQTT_TOPIC_STATUS, "SOLVED");
  mqttLogf("PUZZLE SOLVED");
}

/**
 * @brief Resets all game state without rebooting (PUZZLE_RESET).
 */
void puzzleReset(){
  puzzleSolved = false;
  for(int i = 0; i < 5; i++)
    success[i] = false;
  clearAllParameters();
  updateServo(false);
  mqttClient.publish(MQTT_TOPIC_STATUS, "ONLINE", true);
  mqttLogf("PUZZLE_RESET complete");
}

/**
 * @brief Publishes a full diagnostics string on /status (STATUS command).
 */
void publishDiagnostics(){
  int count = 0;
  for(int i = 0; i < 5; i++)
    if(success[i])
      count++;
  char diag[192];
  snprintf(diag, sizeof(diag),
    "%s v%s | State=%s | Spice=%.2f | Coins=%.2f | Matched=%d/5 | IP=%s | RSSI=%d | UP%lus",
    PROP_NAME, VERSION,
    puzzleSolved ? "SOLVED" : "ONLINE",
    pouchesPlate.plateWeight, coinsPlate.plateWeight, count,
    WiFi.localIP().toString().c_str(), WiFi.RSSI(), millis() / 1000UL);
  mqttClient.publish(MQTT_TOPIC_STATUS, diag);
}

void setupMQTT() {
  mqttClient.setServer(MQTT_SERVER, MQTT_PORT);
  mqttClient.setCallback(mqttCallback);
  mqttClient.setBufferSize(512);  // Increase if needed
}

void heartBeat(){
  unsigned long currentTime = millis();
  if(!(currentTime - hLastTime > heartBeatPulse))
    return;
  hLastTime = currentTime;
  // WatchTower standard heartbeat format on /status
  char hb[64];
  snprintf(hb, sizeof(hb), "HEARTBEAT:%s:UP%lus:RSSI%d",
           puzzleSolved ? "SOLVED" : "ONLINE", millis() / 1000UL, WiFi.RSSI());
  mqttClient.publish(MQTT_TOPIC_STATUS, hb);
  // While solved, re-assert the plain SOLVED payload M3 matches on, in case
  // M3 restarted mid-game and missed the live solve event.
  if(puzzleSolved)
    mqttClient.publish(MQTT_TOPIC_STATUS, "SOLVED");
}

void mqttLogf(const char* format, ...) {
  char buffer[256];
  va_list args;
  va_start(args, format);
  vsnprintf(buffer, sizeof(buffer), format, args);
  va_end(args);

  mqttClient.publish(MQTT_TOPIC_LOG, buffer);
  //Serial.println(buffer);
}

//=============================================================
//                  MAIN FUNCTIONS
//=============================================================
//=============================================================
//                  MISC FUNCTIONS
//=============================================================

void setupServo(){
  mServo.setPeriodHertz(100);
  mServo.attach(SERVO_PIN, 500, 2400); 
}

/**
 * @brief This function rotates the angle to
 *        0 degrees. Representing Yay.
 */
void servoYay(){
  mqttClient.publish(String(String(MQTT_TOPIC_MESSAGE) + "/servo").c_str(),"yay");
  mServo.write(155);
}
/**
 * @brief This function rotates the angle to
 *        90 degrees. Representing Nay.
 */
void servoNay(){
  mqttClient.publish(String(String(MQTT_TOPIC_MESSAGE) + "/servo").c_str(),"nay");
  mServo.write(30);
}



void updateServo(bool state){
  if(state){
    if(state == servoState){
      return;
    }
    else{
      servoYay();
      servoState = state;
    }

  } else {

    if(state == servoState){
      return;
    }
    else{
      servoNay();
      servoState = state;
    }
  }
 mqttClient.publish(String(String(MQTT_TOPIC_MESSAGE) + "/ServoState").c_str(),(state) ? "true":"false");
}
void servoMid(){
   mqttClient.publish(String(String(MQTT_TOPIC_SYSTEM) + "/servo").c_str(),"center");
   mServo.write(70);
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
  //no tags on a plate, or weights don't match: show nay and leave.
  //(only touch the servo here on an actual mismatch — writing nay
  //unconditionally made the servo flap nay->yay every scan while balanced)
  if(pouchesPlate.plateWeight == 0 || coinsPlate.plateWeight == 0
     || pouchesPlate.plateWeight != coinsPlate.plateWeight){
    updateServo(false);
    return;
  }

  //weights match, update servo to yay
  updateServo(true);

  //get the matching weight and update the success tracking array
  //(publish the per-pouch topic once, on the transition - not every scan)
  bool newSuccess = false;
  for(int i = 0; i < 5; i++){
    if(weights[i] == pouchesPlate.plateWeight && !success[i]){
      success[i] = true;
      newSuccess = true;
      printSuccessToMQTT(i);
    }
  }

  //count the successes
  int count = 0;
  for(int i = 0; i < 5; i++)
    if(success[i])
      count++;

  if(newSuccess)
    mqttClient.publish(String(String(MQTT_TOPIC_MESSAGE) + "/SuccessCount").c_str(),String(count).c_str());
  //if success count is 5, the prop puzzle is solved, and finished
  if(count >= 5 && !puzzleSolved){
    puzzleSolved = true;
    publishSolved();
  }
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
bool isAMatchingID(const byte id1[], const byte id2[]) {
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
bool idValidation(const byte id[], const IDStorage<N> & storage){
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
bool isAValidID(const byte id[], bool isPlateForPouches) {
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
bool storageVerification(const byte id[], const ScalePlates<N> & plateStorage){
  for(int i = 0; i < plateStorage.index; i++)
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
bool isAlreadyStored(const byte id[], bool isPlateForPouches) {
  if(isPlateForPouches)
    return storageVerification(id,pouchesPlate);
  else
    return storageVerification(id,coinsPlate);
}
/**
 * @brief Finds the location of an ID in a plate's storage.
 *
 * This function searches for the passed in "id" within the
 * passed in plate's object.
 *
 * @tparam N The maximum number of IDs the plate can store.
 * @param id An array of char representing the RFID(ID).
 * @param isPlateForPouches
 * @return Location/index of the "id" in the plate's storage.
 *           Returns -1, if "id" wasn't found.
 */
int findID(byte id[],bool isPlateForPouches){
  if(isPlateForPouches){
    for(int i = 0; i < NUM_OF_SPICE_POUCHES; i++)
      if(isAMatchingID(id,sPouches[i].id))
        return i;
  }
  else
  {

    for(int i = 0; i < NUM_OF_COINS; i++)
      if(isAMatchingID(id,coins[i].id))
        return i;
  }
  return -1;
}

void storeUID(int index, bool isPlateForPouches){
  if(isPlateForPouches){
    // mqttClient.publish(String(String(MQTT_TOPIC_MESSAGE) +"/pouchesStorage").c_str(),"Adding to storage.");
    pouchesPlate.addNewStorage(sPouches[index].weight,sPouches[index].id);
  }
  else
  {
    // mqttClient.publish(String(String(MQTT_TOPIC_MESSAGE) +"/coinsStorage").c_str(),"Adding to storage.");
    coinsPlate.addNewStorage(coins[index].weight,coins[index].id);
  }
}


void checkForRemoval(int tagCount,bool isPlateForPouches){
  //check to see if the same tag count matches the IDs stored on the plates
  if(isPlateForPouches){
    if(tagCount >= pouchesPlate.index)
      return;
  } else{
    if(tagCount >= coinsPlate.index)
      return;
  }


  //if the count is less than what is stored in the plates,
  //find the correct tag, and remove it from the plate storage

  int count = 0;  //keep track of unmatched ID

  if(isPlateForPouches){
    for(int i = 0; i < pouchesPlate.index; i++){
      for(int j = 0; j < tagCount; j++){
        if(isAMatchingID(pouchesPlate.storage[i].id,&uid[j*ID_LENGTH])){
          count = 0; //reset
          continue;
        }
        else {
          count++;
        }
      }
      //remove unmatching ID from plate
      // mqttClient.publish(String(String(MQTT_TOPIC_MESSAGE) +"/pouchesStorage").c_str(),"Removing from storage.");
      if(count >= tagCount)
        pouchesPlate.removeStorage(i);


      mqttClient.publish(String(String(MQTT_TOPIC_MESSAGE) + "/Spice").c_str(),"");
      if(pouchesPlate.plateWeight == 0)
        mqttClient.publish(String(String(MQTT_TOPIC_SYSTEM) + "/Spice/weight").c_str(),"");
    }
  }
  else{

    for(int i = 0; i < coinsPlate.index; i++){
      for(int j = 0; j < tagCount; j++){
        if(isAMatchingID(coinsPlate.storage[i].id,&uid[j*ID_LENGTH])){
          count = 0; //reset
          continue;
        }
        else {
          count++;
        }
      }
      //remove unmatching ID from plate
      //mqttClient.publish(String(String(MQTT_TOPIC_MESSAGE) +"/coinsStorage").c_str(),"Removing from storage.");
      if(count >= tagCount)
        coinsPlate.removeStorage(i);

      if(coinsPlate.plateWeight == 0)
        mqttClient.publish(String(String(MQTT_TOPIC_SYSTEM) + "/Coins/weight").c_str(),"");
    }
  }
}

void clearUID(){
  for(int i = 0; i < (ID_LENGTH * MAX_READABLE); i++)
    uid[i] = 0;
}
//=============================================================
//            RFID FUNCTIONS
//=============================================================
void sendCommand(Stream& serial,byte* cmd, int len) {
  serial.write(cmd, len);
}

void haltReader(Stream& serial) {
  sendCommand(serial,CMD_HALT, sizeof(CMD_HALT));
}

void rebootReader(Stream& serial) {
  sendCommand(serial,CMD_REBOOT, sizeof(CMD_REBOOT));
}

void flushRx(Stream& serial){
  while(serial.available())
    serial.read();
}

int readResponse(Stream& serial, byte* buffer, int maxLen, int timeout = 100) {
  int index = 0;
  unsigned long start = millis();
  while ((millis() - start) < timeout) {
    while (serial.available()) {
      if (index < maxLen) {
        buffer[index++] = serial.read();
      }
    }
  }
  return index;
}

int getTagCount(Stream& serial) {
  flushRx(serial);  //flush out buffer
  byte response[32];

  sendCommand(serial,CMD_GET_TAG_COUNT, sizeof(CMD_GET_TAG_COUNT));
  delay(50);
  int len = readResponse(serial,response, sizeof(response));

  //check if length of response is long enough
  if (len < 8)
    return -1;
  //check if response starts with appropriate frame header
  if(response[0] != 0xF5){
    mqttClient.publish(MQTT_TOPIC_MESSAGE,"Bad frame.");
    return -1;
  }

  int count = response[7];  // 8th byte
  if (count < 0)
    return 0;
  if (count > MAX_READABLE)  // CMD_GET_UID only has MAX_READABLE entries
    count = MAX_READABLE;
  return count;
}

bool getUID(Stream& serial,byte * cmd, byte* uid) {
  flushRx(serial);    //flush out buffer
  byte response[32];
  //get uid size is always 9 bytes
  //cannot use sizeof on a pointer
  sendCommand(serial,cmd,9);
  delay(50);
  int len = readResponse(serial,response, sizeof(response));

  if (len < 15)
    return false;

  // UID = bytes 8–11 (0-based index)
  for (int i = 0; i < 4; i++)
    uid[i] = response[9 + i];

  return true;
}

void printUID(byte* uid) {
  for (int i = 0; i < 4; i++) {
    if (uid[i] < 0x10)
      Serial.print("0");
    Serial.print(uid[i], HEX);
  }
  Serial.println();
}

void printToMQTT(){

  if(pouchesPlate.index > 0){

    String topic = String(MQTT_TOPIC_SYSTEM) + "/Spice";
    mqttClient.publish(String(topic + String("/weight")).c_str(), String(pouchesPlate.plateWeight).c_str());
  }

  if(coinsPlate.index > 0 ){
    String topic = String(MQTT_TOPIC_SYSTEM) + "/Coins";
    mqttClient.publish(String(topic + String("/weight")).c_str(), String(coinsPlate.plateWeight).c_str());
  }

}

void printCapturedUIDs(int count){

  for(int i = 0; i < count; i++){

    String id = "";
    for(int j = 0; j < ID_LENGTH; j++){
      id += "0x" + String(uid[(i*ID_LENGTH)+j],HEX) + ((j == ID_LENGTH-1)?"":"-");
    }

    mqttClient.publish(String(String(MQTT_TOPIC_SYSTEM) + "/UID" + String(i)).c_str(),id.c_str());
  }

}


void printSuccessToMQTT(int successIndex){
  switch(successIndex){
    case 0: 
      mqttClient.publish(String(String(MQTT_TOPIC) + "/Yeast").c_str(),"true");
      break;
    case 1: 
      mqttClient.publish(String(String(MQTT_TOPIC) + "/SugarCane").c_str(),"true");
      break;
    case 2: 
      mqttClient.publish(String(String(MQTT_TOPIC) + "/Vanilla").c_str(),"true");
      break;
    case 3: 
      mqttClient.publish(String(String(MQTT_TOPIC) + "/Molasses").c_str(),"true");
      break;
    case 4: 
      mqttClient.publish(String(String(MQTT_TOPIC) + "/Cloves").c_str(),"true");
      break;
  }
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
void listen(Stream& serial, bool isPlateForPouches) {
  int count = getTagCount(serial);
  delay(10);

  //bad frame / no response: don't treat it as "all tags removed"
  if(count < 0){
    clearUID();
    return;
  }

  for(int i = 0; i < count; i++){
    //get each UID of each tags detected
    if (getUID(serial,CMD_GET_UID[i],&uid[i*ID_LENGTH])){
      //print uid as you receive them
      /*
         String tID = "";
         for(int j=0; j < ID_LENGTH; j++)
         tID += String(uid[(i*ID_LENGTH) +j],HEX) + ((j == ID_LENGTH-1)?"":"-");
         mqttClient.publish(String(String(MQTT_TOPIC_MESSAGE) +"/UID"+ String(i)).c_str(),tID.c_str());
         */
      //-------------------------------------------------------
      //check if it is a valid ID (it is known and on the correct scale)
      if(isAValidID(&uid[i*ID_LENGTH],isPlateForPouches)){
        // mqttClient.publish(String(String(MQTT_TOPIC_MESSAGE) +"/valid"+ String(i)).c_str(),"is a valid UID.");
      } else {

        // mqttClient.publish(String(String(MQTT_TOPIC_MESSAGE) +"/valid"+ String(i)).c_str(),"is not a valid UID.");
        continue;
      }
      //-------------------------------------------------------


      //check if it is already stored in the scales storage
      if(isAlreadyStored(&uid[i*ID_LENGTH],isPlateForPouches)){
        //mqttClient.publish(String(String(MQTT_TOPIC_MESSAGE) +"/stored" +  String(i)).c_str(),"is already stored.");
      } else {
        //mqttClient.publish(String(String(MQTT_TOPIC_MESSAGE) +"/stored"+ String(i)).c_str(),"is not already stored.");
        //first, find the matching UID with its weight
        //then store the new UID
        int index = findID(&uid[i*ID_LENGTH],isPlateForPouches);
        storeUID(index,isPlateForPouches);
        if(isPlateForPouches)
          mqttClient.publish(String(String(MQTT_TOPIC_MESSAGE) + "/Spice").c_str(),sPouches[index].spice.c_str());

      }
    }
  }
  //check if any tags was removed
  checkForRemoval(count,isPlateForPouches);
  //clear uid array
  clearUID();
  delay(250);
}

void scanPouchesPlate(){
  listen(rfid1,true);
}

void scanCoinsPlate(){
  listen(rfid2,false);
}


//=============================================================
//            GENERAL FUNCTIONS
//=============================================================
void initParam(){
  for(int i = 0; i < 5; i++ )
    weights[i] = sPouches[i].weight;
}

void _init(){
  //Serial setup
  Serial.begin(BAUD_RATE);
  rfid1.begin(BAUD_RATE, SERIAL_8N1, RX1,TX1);
  rfid2.begin(BAUD_RATE, SERIAL_8N1, RX2,TX2);
  //Servo setup
  setupServo();
  //network setup
  setupWiFi();
  //mqtt setup
  setupMQTT();
  //initialize BalancingScale params
  initParam();
  //set servo to center
  servoMid();
  //add delay for rfids to fully boot
  delay(1000); //1 sec
}

/**
 * @brief The main components of this program.
 *
 * This functions contains all the instructions in the
 * program that will run in the main loop.
 */
void program() {
  if (!mqttClient.connected()) {
    connectMQTT();
  }
  mqttClient.loop();

  heartBeat();

  if(puzzleSolved)
    return;

  unsigned long currentTime = millis();
  if(!(currentTime - sLastTime >= scanPeriod))
    return;

  sLastTime = currentTime;
  //polling pouches plate
  scanPouchesPlate();
  //polling coins plate
  scanCoinsPlate();
  //log to MQTT the scale status
  printToMQTT();

  checkSuccess();
}

//=============================================================
//               MAIN SETUP
//=============================================================
void setup() {
  _init();
}
//=============================================================
//                   LOOP
//=============================================================
void loop() {
  program();
}



