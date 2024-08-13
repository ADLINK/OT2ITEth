#include <Arduino.h>
#include <math.h>
#include <stdlib.h>
#include <tftp2sd.h>
#define	MAX_LENGTH		256
// For SD Card
#define SD_Block_Size 512
#define BUFF_SIZE 4096
uint8_t buffer[BUFF_SIZE];
int buff_index, sd_write_index, sd_read_index, sd_dir_index;
char filename[256];
char t_filename[256];

//Ethernet
#include "src/OT2ITEth.h"

#define MAC_ADDRESS "\x02\x00\x00\x00\x00\x12"

uint8_t mac[6] = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06};
OT2ITEth ot2itEth;

// #ifdef __cplusplus
// extern "C" {
// #endif

#include <string.h>

/* Define a base directory for TFTP access
 * ATTENTION: This code does NOT check for sandboxing,
 * i.e. '..' in paths is not checked! */

/* Define this to a server IP string */
#ifndef LWIP_TFTP_EXAMPLE_CLIENT_REMOTEIP
#define LWIP_TFTP_EXAMPLE_CLIENT_REMOTEIP "192.168.0.233"
#endif

// #ifdef __cplusplus
// }
// #endif
void setup()
{

  Serial.begin(9600);
  Serial.println("USB Virtual Serial Port Initialized");
  ot2itEth.begin();
  ot2itEth.ip_stack_init((uint8_t *)MAC_ADDRESS);

  ot2it_tftp_example_init_server(); 

  Serial1.begin(57600); // This is for debug port

  Serial1.print("\r\n");
  Serial1.println(version);

  //SD Card initialization
  if (!SD.begin(true)) {
    Serial1.println("SD card initialization failed!");
  }
  else{
    Serial1.println("SD card initialized!");
    Serial1.printf("SD card size = %d MB\r\n",SD.cardsize()/2048);
    //Serial1.printf("SD card size*block = %d\n",SD.cardsize()*SD_Block_Size);
  }
}

void loop()
{
  ot2itEth.get_link_sts();
  ot2itEth.receive();
}
