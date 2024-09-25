/*
  SD card basic file example

  This example shows how to create and destroy an SD card file.
  The circuit. Pin numbers reflect the default
  SPI pins for Uno and Nano models:
   SD card attached to SPI bus as follows:
 ** SDO - pin 11
 ** SDI - pin 12
 ** CLK - pin 13
 ** CS - depends on your SD card shield or module.
 		Pin 10 used here for consistency with other Arduino examples
    (for MKR Zero SD: SDCARD_SS_PIN)

  created   Nov 2010
  by David A. Mellis
  modified 24 July 2020
  by Tom Igoe

  This example code is in the public domain.
*/
#include <SD.h>

File2 myFile;

void setup() {
  // Open serial communications and wait for port to open:
//  Serial.begin(115200);
  Serial1.begin(57600);
  // wait for Serial Monitor to connect. Needed for native USB port boards only:
while (!Serial1);

  Serial1.print("Initializing SD card...");

  if (!SD.begin(true)) {
    Serial1.println("initialization failed.");
    while (1);
  }
  Serial1.println("initialization done.");

  if (SD.exists("example.txt")) {
    Serial1.println("example.txt exists.");
  } else {
    Serial1.println("example.txt doesn't exist.");
  }

  // open a new file and immediately close it:
  Serial1.println("Creating example.txt...");
  myFile = SD.open("example.txt", FILE_WRITE);
  myFile.close();

  // Check to see if the file exists:
  if (SD.exists("example.txt")) {
    Serial1.println("example.txt exists.");
  } else {
    Serial1.println("example.txt doesn't exist.");
  }

  // delete the file:
  Serial1.println("Removing example.txt...");
  SD.remove("example.txt");

  if (SD.exists("example.txt")) {
    Serial1.println("example.txt exists.");
  } else {
    Serial1.println("example.txt doesn't exist.");
  }
}

void loop() {
  // nothing happens after setup finishes.
}
