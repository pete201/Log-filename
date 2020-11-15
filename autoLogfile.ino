/*
  auto log file
  On each boot, a new logfile is created on an SD card that is numerically one more than the last 
  the Files are listed to serial out
*/
#include <SPI.h>
#include <SD.h>

const int chipSelect = 15;  // Chip select pin on D1 mini SD card shield

File root;
File logFile;
char logFileDir [20] = "mpu6050log";
char logFilename[20];

int logFilenumber;

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

  // first, we create a logging directory if it does not already exist:
  if (!SD.exists(logFileDir)){
    Serial.print("Creating new directory: "); Serial.println(logFileDir);
    SD.mkdir(logFileDir);
  } else {
    Serial.print("dir "); Serial.print(logFileDir); Serial.println(" already exists");
  }


  logFilenumber = getLogFilename(SD.open(logFileDir));
  //Serial.print("retuned log filenumber integer is: ");
  //Serial.println(logFilenumber);
  //SD.close("mpu6050log");


  // we now need to generate a filename from the int returned from getLogFilename
  sprintf(logFilename, "%s/%d.log", logFileDir, logFilenumber);
  Serial.print("and the log Filename as a string is: ");
  Serial.println(logFilename);
  Serial.println();

  logFile = SD.open(logFilename, FILE_WRITE);
  if (logFile){
    Serial.print("new logfile ");
    Serial.print(logFilename);
    Serial.println(" opened!");
    logFile.close();
  }
  

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

int getLogFilename(File dir){

  int filename = -1;

  Serial.print("passed directory name is ");
  Serial.println(dir);

  // I DONT LIKE THE WHILE(TRUE).  REPLACE WITH while(File entry =  dir.openNextFile())
  while (true) {
    File entry =  dir.openNextFile();
    if (! entry) {
      // no more files

      // INSERT FINAL LOGFILE NAME HERE
      // first convert char filename into an int so that we can i++
      filename++;


      Serial.print("New log filename is: ");
      Serial.println(filename);

      //create the file and return it
      return filename;
    }    
    
    // pass int part of filename into int filename
    filename = atoi(entry.name());

    Serial.print("flename "); Serial.print(filename); Serial.println(" already exists...");

    entry.close();
  }
}