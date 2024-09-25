#ifndef SDHC_H
#define SDHC_H

#include <sam.h>
//#include <same54/include/same54.h>
#include <stdint.h>
#include "sdhc_defines.h"
#include "sdhc_protocol.h"
#include "sdhc_registers.h"

#ifdef __cplusplus
extern "C" {
#endif

sdhc_err_t sdhc_set_clk(Sdhc * sdhc, uint32_t freq_hz, sdhc_clock_mode_t clock_mode);

void sdhc_set_bus_width(Sdhc * sdhc, sdhc_bus_width_t width);

uint8_t sdhc_has_high_speed_support(Sdhc * sdhc);

void sdhc_set_high_speed(Sdhc * sdhc, uint8_t high_speed);

void sdhc_reset_dat(Sdhc * sdhc);

void sdhc_reset_cmd(Sdhc * sdhc);

void sdhc_reset_all(Sdhc * sdhc);

void sdhc_driver_init(Sdhc * sdhc);

sdhc_err_t sdhc_cmd(Sdhc * sdhc, uint32_t cmd, uint32_t arg);

sdhc_err_t sdhc_adtc_start(Sdhc * sdhc, uint32_t cmd, uint32_t arg, uint16_t block_len, uint16_t n_blocks, uint8_t use_dma);

void * sdhc_get_response_ptr(Sdhc * sdhc);

uint32_t sdhc_get_response_32bit(Sdhc * sdhc);

uint32_t sdhc_get_response_auto_status(Sdhc * sdhc);

void sdhc_get_response_120bit(Sdhc * sdhc, void * response);

sdhc_err_t sdhc_read_block(Sdhc * sdhc, void * destination, uint16_t block_len);

sdhc_err_t sdhc_read_blocks(Sdhc * sdhc, void * destination, uint16_t block_len, uint16_t n_blocks);

sdhc_err_t sdhc_write_block(Sdhc * sdhc, const void * source, uint16_t block_len);

sdhc_err_t sdhc_write_blocks(Sdhc * sdhc, const void * source, uint16_t block_len, uint16_t n_blocks);

sdhc_err_t sdhc_wait_dma(Sdhc * sdhc);

#ifdef __cplusplus
}
#endif

#endif /* SDHC_H */