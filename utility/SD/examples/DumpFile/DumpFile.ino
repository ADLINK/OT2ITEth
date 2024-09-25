/*
  SD card file dump

  This example shows how to read a file from the SD card using the
  SD library and send it over the serial port.
  Pin numbers reflect the default SPI pins for Uno and Nano models.

  The circuit:
   SD card attached to SPI bus as follows:
 ** SDO - pin 11
 ** SDI - pin 12
 ** CLK - pin 13
 ** CS - depends on your SD card shield or module.
 		Pin 10 used here for consistency with other Arduino examples
   (for MKR Zero SD: SDCARD_SS_PIN)

  created  22 December 2010
  by Limor Fried
  modified 9 Apr 2012
  by Tom Igoe

  This example code is in the public domain.
*/
#include <SD.h>

void setup() {
  // Open serial communications and wait for port to open:
  //Serial1.begin(115200);
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
  File2 dataFile = SD.open("datalog.txt");

  // if the file is available, write to it:
  if (dataFile) {
    while (dataFile.available()) {
      Serial1.write(dataFile.read());
    }
    dataFile.close();
  }
  // if the file isn't open, pop up an error:
  else {
    Serial1.println("error opening datalog.txt");
  }
}

void loop() {
}
