/*
  auto log file
  On each boot, a new logfile is created on an SD card that is numerically one more than the last 
  the Files are listed to serial out
*/
#include <SPI.h>
#include <SD.h>
#include <cstring>

const int chipSelect = 15;  // Chip select pin on D1 mini SD card shield

File root;
File logFile;
String logFileDir = "mpu6050log";
String logFilename;


void setup() {
 // Open serial communications and wait for port to open:
  Serial.begin(115200);
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

  // first, create a logging directory if it does not already exist:
  if (!SD.exists(logFileDir)){
    Serial.print("Creating new directory: "); Serial.println(logFileDir);
    SD.mkdir(logFileDir);
  } else {
    Serial.print("dir "); Serial.print(logFileDir); Serial.println(" already exists");
  }

  logFilename = getLogFilename(SD.open(logFileDir));

  // now we have the new logFileName, create the logfile
  logFile = SD.open(logFilename, FILE_WRITE);
  if (logFile){
    Serial.print("new logfile ");
    Serial.print(logFilename);
    Serial.println(" opened!");
    logFile.close();
  }
  
  // print out SD card directory to Serial
  root = SD.open("/");
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


String getLogFilename(File dir){

  int logFilenumber = -1;
  String logfile;
  
  while (File entry = dir.openNextFile()) {   
    
    // if a logfile already exists, pass integer part of filename into int logFilenumber
    logFilenumber = atoi(entry.name());

    Serial.print("file "); Serial.print(logFilenumber); Serial.println(" already exists...");

    entry.close();
  }

  // no more files, so inc logFilenumber by 1 to get next filename
  logFilenumber++;

  //create the filename and return it
  logFilename =  logFileDir + "/" + logFilenumber + ".log";

  return logFilename;
}