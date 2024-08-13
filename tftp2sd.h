#ifndef _TFTP2SD_H
#define _TFTP2SD_H

#include <stdio.h>
#include "lwip/tftp_client.h"
#include "lwip/tftp_server.h"
#include "SD-samd5x.h"

void* tftp_open(const char* fname, const char* mode, u8_t is_write);
void tftp_close(void* file_handler);
int tftp_read(void* handle, void* buf, int bytes);
int tftp_write(void* handle, struct pbuf* p);
void tftp_error(void* handle, int err, const char* msg, int size);

const struct tftp_context tftp = {
  tftp_open,
  tftp_close,
  tftp_read,
  tftp_write,
  tftp_error
};

void ot2it_tftp_example_init_server(void);

#endif // _TFTP2SD_H