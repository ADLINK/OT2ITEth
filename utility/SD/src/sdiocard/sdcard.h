#ifndef SDCARD_H
#define SDCARD_H

#include <sam.h>
#include <stdbool.h>
#include <stdint.h>
#include "sdcard_defines.h"
#include "sdcard_commands.h"

#ifdef __cplusplus
extern "C" {
#endif

bool sdcard_init(sdcard_t* sdcard, Sdhc * sdhc, bool high_speed);

bool sdcard_read(sdcard_t* sdcard, void* destination, uint32_t address, uint16_t n_blocks, bool use_dma);

bool sdcard_write(sdcard_t* sdcard, const void* source, uint32_t address, uint16_t n_blocks, bool use_dma);

bool sdcard_is_ready(sdcard_t* sdcard);

#ifdef __cplusplus
}
#endif

#endif /* SDCARD_H */