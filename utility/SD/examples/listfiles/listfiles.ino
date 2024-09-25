/*
  Listfiles

  This example shows how to print out the files in a
  directory on a SD card. Pin numbers reflect the default
  SPI pins for Uno and Nano models

  The circuit:
   SD card attached to SPI bus as follows:
 ** SDO - pin 11
 ** SDI - pin 12
 ** CLK - pin 13
 ** CS - depends on your SD card shield or module.
 		Pin 10 used here for consistency with other Arduino examples
    (for MKR Zero SD: SDCARD_SS_PIN)

  created   Nov 2010
  by David A. Mellis
  modified 9 Apr 2012
  by Tom Igoe
  modified 2 Feb 2014
  by Scott Fitzgerald
  modified 24 July 2020
  by Tom Igoe
  
  This example code is in the public domain.

*/
#include <SD.h>

File2 root;

void setup() {
 // Open serial communications and wait for port to open:
  //Serial.begin(115200);
  Serial1.begin(57600);
  // wait for Serial Monitor to connect. Needed for native USB port boards only:
  while (!Serial1);

  Serial1.print("Initializing SD card...");

  if (!SD.begin(true)) {
    Serial1.println("initialization failed.");
    while (true);
  }

  Serial1.println("initialization done.");

  root = SD.open("/");

  printDirectory(root, 0);

  Serial1.println("done!");
}

void loop() {
  // nothing happens after setup finishes.
}

void printDirectory(File2 dir, int numTabs) {
  while (true) {

    File2 entry =  dir.openNextFile();
    if (! entry) {
      // no more files
      break;
    }
    for (uint8_t i = 0; i < numTabs; i++) {
      Serial1.print('\t');
    }
    Serial1.print(entry.name());
    if (entry.isDirectory()) {
      Serial1.println("/");
      printDirectory(entry, numTabs + 1);
    } else {
      // files have sizes, directories do not
      Serial1.print("\t\t");
      Serial1.println(entry.size(), DEC);
    }
    entry.close();
  }
}
