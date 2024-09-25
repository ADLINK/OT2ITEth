#ifndef SD_BLOCK_DEVICE_H
#define SD_BLOCK_DEVICE_H

#include "fs/fs_struct.h"
#include "sdiocard/sdcard.h"

#ifdef __cplusplus
extern "C" {
#endif

block_device_t sd_create_block_device(sdcard_t * sdcard);

#ifdef __cplusplus
}
#endif

#endif /* SD_BLOCK_DEVICE_H */