#include "tftp2sd.h"

#define TFTP_MAX_PAYLOAD_SIZE 512
#define MAX_FILE_SIZE 4096
#define PUT 1
#define GET 0

/* Define a base directory for TFTP access
 * ATTENTION: This code does NOT check for sandboxing,
 * i.e. '..' in paths is not checked! */
#ifndef LWIP_TFTP_EXAMPLE_BASE_DIR
#define LWIP_TFTP_EXAMPLE_BASE_DIR ""
#endif

/* Define this to a file to get via tftp client */
#ifndef LWIP_TFTP_EXAMPLE_CLIENT_FILENAME
#define LWIP_TFTP_EXAMPLE_CLIENT_FILENAME "test.bin"
#endif

/* Define this to a server IP string */
#ifndef LWIP_TFTP_EXAMPLE_CLIENT_REMOTEIP
#define LWIP_TFTP_EXAMPLE_CLIENT_REMOTEIP "192.168.0.233"
#endif

static char full_filename[256];
FILE* tftp_file = nullptr;
File tftp_sd_file;

int tftp_read_index, tftp_write_index;
uint8_t sd_file_buffer[MAX_FILE_SIZE];

void *tftp_open_file(const char* fname, u8_t is_write)
{
  int result;

  snprintf(full_filename, sizeof(full_filename), "%s%s", LWIP_TFTP_EXAMPLE_BASE_DIR, fname);
  full_filename[sizeof(full_filename)-1] = 0;

  if (!SD.begin(true)) 
  {
    //Serial.println("SD card initialization failed!");        
    return 0;
  }
  //Serial.println("SD card initialization successful!");

  memset(sd_file_buffer, 0, sizeof(sd_file_buffer));

  switch (is_write) {
  case PUT: // tftp put
    if (SD.exists(full_filename)) {
      SD.remove(full_filename);
    }
    tftp_sd_file = SD.open(full_filename, FILE_WRITE);
    tftp_sd_file.seek(0);
    tftp_write_index = 0;

    tftp_file = fmemopen((char*)sd_file_buffer, sizeof(sd_file_buffer), "w+");
    break;

  case GET: // tftp get
    if (!SD.exists(full_filename)) {
      //Serial.println("File doesn't exist in SD card !");
      return 0;
    } else {
      tftp_read_index = 0;
    }
    tftp_sd_file = SD.open(full_filename);
    tftp_sd_file.seek(0);

    result = tftp_sd_file.read(&sd_file_buffer[0], MAX_FILE_SIZE);

    tftp_file = fmemopen((char*)sd_file_buffer, result, "r+");
    if (!tftp_file) {
      //Serial.println("File cannot be created!");
      return 0;
    }
    tftp_sd_file.close();
    break;

  default:
    // Handle unexpected cases here
    break;
  }
  
  fflush(tftp_file);

  return (void*)tftp_file;
}

void* tftp_open(const char* fname, const char* mode, u8_t is_write)
{
  //Serial.println("tftp_open called");
  LWIP_UNUSED_ARG(mode);
  
  return tftp_open_file(fname, is_write);
}

void tftp_close(void* file_handler)
{
  //Serial.println("tftp_close called");
  fclose((FILE*)file_handler);
}

int tftp_read(void* file_handler, void* buf, int bytes)
{
  //Serial.println("tftp_read called");

  int ret = fread(buf, 1, bytes, (FILE*)file_handler);
  //fseek((FILE*)handle, 512, SEEK_CUR);

  //Serial.println(ftell((FILE*)handle));
  //Serial.println(ret);

  tftp_read_index += ret;

  // if (bytes >= tftp_read_index)
  //   bytes = tftp_read_index;
  // else
  //   tftp_read_index -= bytes;

  if(tftp_read_index == 0)
  {
    //Serial.println("No data in " + String(full_filename));
    return 0;
  }
  
  //Serial.println(tftp_read_index);

  return ret;
}

int tftp_write(void* file_handler, struct pbuf* p)
{
  //Serial.println("tftp_write called");
  fclose((FILE*)file_handler);
  
  int ret = tftp_sd_file.write((uint8_t*)p->payload, p->len);
  tftp_write_index += ret;
  //tftp_sd_file.seek(ret);

  p = p->next;
  //Serial.println(ret);
  //Serial.println(tftp_write_index);

  
  if(ret < TFTP_MAX_PAYLOAD_SIZE || tftp_write_index == MAX_FILE_SIZE)
  {
    tftp_sd_file.close();
  }

  return ret;
}

/* For TFTP client only */
void tftp_error(void* handle, int err, const char* msg, int size)
{
  char message[100];

  LWIP_UNUSED_ARG(handle);

  memset(message, 0, sizeof(message));
  MEMCPY(message, msg, LWIP_MIN(sizeof(message)-1, (size_t)size));

  //Serial.print("TFTP error:");
  //Serial.print(err);
  //Serial.println(message);
}

void ot2it_tftp_example_init_server(void)
{
  tftp_init_server(&tftp);
}