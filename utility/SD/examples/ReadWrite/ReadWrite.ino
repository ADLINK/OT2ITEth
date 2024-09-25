/*
  SD card read/write

  This example shows how to read and write data to and from an SD card file
  The circuit. Pin numbers reflect the default
  SPI pins for Uno and Nano models:
   SD card attached to SPI bus as follows:
 ** SDO - pin 11
 ** SDI - pin 12
 ** CLK - pin 13
 ** CS - pin 4 (For For Uno, Nano: pin 10. For MKR Zero SD: SDCARD_SS_PIN)

  created   Nov 2010
  by David A. Mellis
  modified  24 July 2020
  by Tom Igoe

  This example code is in the public domain.

*/
#include <SD.h>

File2 myFile;

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

  // open the file. note that only one file can be open at a time,
  // so you have to close this one before opening another.
  myFile = SD.open("test2.txt", FILE_WRITE);

  // if the file opened okay, write to it:
  if (myFile) {
    Serial1.print("Writing to test2.txt...");
    myFile.println("testing 1, 2, 3.");
    // close the file:
    myFile.close();
    Serial1.println("done.");
  } else {
    // if the file didn't open, print an error:
    Serial1.println("error opening test2.txt");
  }

  // re-open the file for reading:
  myFile = SD.open("test2.txt");
  if (myFile) {
    Serial1.println("test2.txt:");

    // read from the file until there's nothing else in it:
    while (myFile.available()) {
      Serial1.write(myFile.read());
    }
    // close the file:
    myFile.close();
  } else {
    // if the file didn't open, print an error:
    Serial1.println("error opening test2.txt");
  }
}

void loop() {
  // nothing happens after setup
}
