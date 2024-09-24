#include "ot2it_tftp.h"


//Ethernet
#include <OT2ITEth.h>

#define MAC_ADDRESS "\x02\x00\x00\x00\x00\x12"

uint8_t mac[6] = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06};
OT2ITEth ot2itEth;

#include <stdio.h>

#include "lwip/tftp_client.h"
#include "lwip/tftp_server.h"

#include <string.h>

/* Define a base directory for TFTP access
 * ATTENTION: This code does NOT check for sandboxing,
 * i.e. '..' in paths is not checked! */
#ifndef LWIP_TFTP_EXAMPLE_BASE_DIR
#define LWIP_TFTP_EXAMPLE_BASE_DIR ""
#endif


#define I_PI_OT2IT 1

void setup()
{
  ot2itEth.begin();
  ot2itEth.ip_stack_init((uint8_t *)MAC_ADDRESS);

  ot2it_tftp_example_init_server(); 
  
  Serial1.begin(57600); // This is for debug port
}
/*==============================================================================
 * LOOP()
 *============================================================================*/

void loop()
{
  ot2itEth.get_link_sts();
  ot2itEth.receive();
  /*Initialize IP stack interface with MAC Address*/
  
}