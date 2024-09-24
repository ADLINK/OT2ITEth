#include "SD.h"
#include "OT2ITEth.h"
extern "C" {
    #include "lwip/tftp_client.h"
    #include "lwip/tftp_server.h"
}
#define TFTP_MAX_PAYLOAD_SIZE 512
#define LWIP_TFTP_EXAMPLE_BASE_DIR ""
#define MAX_FILE_SIZE 4096
#define PUT 1
#define GET 0

void* tftp_open(const char* fname, const char* mode, u8_t is_write);
void tftp_close(void* handle);
int tftp_read(void* handle, void* buf, int bytes);
int tftp_write(void* handle, struct pbuf* p);
void tftp_error(void* handle, int err, const char* msg, int size);
void ot2it_tftp_example_init_server(void);

extern FILE* tftp_file;
extern File2 tftp_sd_file;

extern uint8_t sd_file_buffer[MAX_FILE_SIZE];
extern int tftp_read_index;
extern int tftp_write_index;

const struct tftp_context tftp =
{
    tftp_open,
    tftp_close,
    tftp_read,
    tftp_write,
    tftp_error
};

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __cplusplus
}
#endif

void *tftp_open_file(const char* fname, u8_t is_write);

