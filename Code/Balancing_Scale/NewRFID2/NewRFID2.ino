#include <SoftwareSerial.h>

// Choose two pins for SoftwareSerial
SoftwareSerial rSerial(10, 11); // RX, TX



const int tagLen = 16;
const int idLen = 13;
const int kTags = 4;

// Put your known tags here!
char knownTags[kTags][idLen] = {
             "111111111111",
             "444444444444",
             "555555555555",
             "7A005B0FF8D6"
};

// Empty array to hold a freshly scanned tag
char newTag[idLen];

void setup() {
  // Starts the hardware and software serial ports
   Serial.begin(9600);
   rSerial.begin(9600);

  Serial.println("Starting program.");
}

void loop() {
  // Counter for the newTag array
  int i = 0;
  // Variable to hold each byte read from the serial buffer
  int readByte;
  // Flag so we know when a tag is over
  boolean tag = false;

  // This makes sure the whole tag is in the serial buffer before
  // reading, the Arduino can read faster than the ID module can deliver!
  if (rSerial.available() == tagLen) {
    tag = true;
  }

  if (tag == true) {
    while (rSerial.available()) {
      // Take each byte out of the serial buffer, one at a time
      readByte = rSerial.read();

      /* This will skip the first byte (2, STX, start of text) and the last three,
      ASCII 13, CR/carriage return, ASCII 10, LF/linefeed, and ASCII 3, ETX/end of 
      text, leaving only the unique part of the tag string. It puts the byte into
      the first space in the array, then steps ahead one spot */
      if (readByte != 2 && readByte!= 13 && readByte != 10 && readByte != 3) {
        newTag[i] = readByte;
        i++;
      }

      // If we see ASCII 3, ETX, the tag is over
      if (readByte == 3) {
        tag = false;
      }

    }
  }


  // don't do anything if the newTag array is full of zeroes
  if (strlen(newTag)== 0) {
    return;
  }

  else {
    int total = 0;

    for (int ct=0; ct < kTags; ct++){
        total += checkTag(newTag, knownTags[ct]);
    }

    // If newTag matched any of the tags
    // we checked against, total will be 1
    if (total > 0) {

      // Put the action of your choice here!

      // I'm going to rotate the servo to symbolize unlocking the lockbox

      Serial.println("Success!");
    }

    else {
        // This prints out unknown cards so you can add them to your knownTags as needed
        Serial.print("Unknown tag! ");
        Serial.print(newTag);
        Serial.println();
    }
  }

  // Once newTag has been checked, fill it with zeroes
  // to get ready for the next tag read
  for (int c=0; c < idLen; c++) {
    newTag[c] = 0;
  }
}

// This function steps through both newTag and one of the known
// tags. If there is a mismatch anywhere in the tag, it will return 0,
// but if every character in the tag is the same, it returns 1
int checkTag(char nTag[], char oTag[]) {
    for (int i = 0; i < idLen; i++) {
      if (nTag[i] != oTag[i]) {
        return 0;
      }
    }
  return 1;
}
