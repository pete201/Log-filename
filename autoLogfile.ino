/*
  Listfiles
  This example shows how print out the files in a
  directory on a SD card.

  from file created   Nov 2010   by David A. Mellis
  modified 9 Apr 2012     by Tom Igoe
  modified 2 Feb 2014     by Scott Fitzgerald
  modified 24 July 2020   by Tom Igoe
*/
#include <SPI.h>
#include <SD.h>

const int chipSelect = 15;  // Chip select pin on SD card shield

File root;

void setup() {
 // Open serial communications and wait for port to open:
  Serial.begin(115200);
  // wait for Serial Monitor to connect. Needed for native USB port boards only:
  while (!Serial);
  Serial.println("");

  Serial.println("Initializing SD card...");
  if (!SD.begin(chipSelect)) {
    Serial.println("initialization failed. Things to check:");
    Serial.println("1. is a card inserted?");
    Serial.println("2. is your wiring correct?");
    Serial.println("3. did you change the chipSelect pin to match your shield or module?");
    Serial.println("Note: press reset or reopen this serial monitor after fixing your issue!");
    while (true);
  }

  Serial.println("initialization done.");
  root = SD.open("/");

  // first, we create a directory if it does not already exist:
  if (!SD.exists("mpu6050log")){
    Serial.println("Creating new directory mpu6050log");
    SD.mkdir("mpu6050log");
  } else {
    Serial.println("dir mpu6050log already exists");
  }

  printDirectory(root, 0);
  Serial.println("done!");
}

void loop() {
  // nothing happens after setup finishes.
}

void printDirectory(File dir, int numTabs) {
  while (true) {
    File entry =  dir.openNextFile();
    if (! entry) {
      // no more files
      break;
    }

    for (uint8_t i = 0; i < numTabs; i++) {
      Serial.print('\t');
    }

    Serial.print(entry.name());

    if (entry.isDirectory()) {
      Serial.println("/");
      printDirectory(entry, numTabs + 1);
    } else {
      // files have sizes, directories do not
      Serial.print("\t\t");
      Serial.println(entry.size(), DEC);
    }

    entry.close();
  }
}