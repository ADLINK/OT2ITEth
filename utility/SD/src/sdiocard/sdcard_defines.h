#ifndef SDCARD_DEFINES_H
#define SDCARD_DEFINES_H

#include <sam.h>
#include <stdint.h>
#include <stdbool.h>
#include "sdhc/sdhc_registers.h"

#define SDCARD_CLK_INIT 428000
#define SDCARD_CHECK_PATTERN 0xAA
#define SDCARD_DEFAULT_BLOCK_LEN 512

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    SDCARD_STATE_IDLE     = 0x00,
    SDCARD_STATE_READY    = 0x01,
    SDCARD_STATE_IDENT    = 0x02,
    SDCARD_STATE_STBY     = 0x03,
    SDCARD_STATE_TRAN     = 0x04,
    SDCARD_STATE_DATA     = 0x05,
    SDCARD_STATE_RCV      = 0x06,
    SDCARD_STATE_PRG      = 0x07,
    SDCARD_STATE_DIS      = 0x08,
} sdcard_state_t;

typedef enum {
    SDCARD_VER_UNKNOWN,
    SDCARD_VER_1_X_SC,
    SDCARD_VER_2_0_UNKNOWN,
    SDCARD_VER_2_0_SC,
    SDCARD_VER_2_0_HC,
} sdcard_version_t;

typedef enum {
    SDCARD_ERASE_MODE_NORMAL = 0,
    SDCARD_ERASE_MODE_DISCARD = 1,
    SDCARD_ERASE_MODE_FULE = 2,
} sdcard_erase_mode_t;

typedef enum {
    SDCARD_BUS_SPEED_DEFAULT_SPEED = 0,
    SDCARD_BUS_SPEED_HIGH_SPEED = 1,
} sdcard_bus_speed_t;

typedef enum {
    SDCARD_BUS_1LANE,
    SDCARD_BUS_4LANE,
} sdcard_bus_width_t;

typedef struct {
    sdhc_reg_csd_t csd;
    sdhc_reg_cid_t cid;
} sdcard_registers_t;

typedef struct {
    Sdhc * device;
    uint16_t rca;
    uint32_t clock_hz;
    uint32_t num_blocks;
    uint16_t block_len;
    sdcard_bus_speed_t bus_speed;
    sdcard_bus_width_t bus_width;
    sdcard_version_t version;
    sdcard_state_t current_state;
    sdcard_registers_t regs;
} sdcard_t;

#ifdef __cplusplus
}
#endif

#endif /* SDCARD_DEFINES_H */