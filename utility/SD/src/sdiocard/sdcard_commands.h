#ifndef SDCARD_COMMANDS_H
#define SDCARD_COMMANDS_H

#include <stdbool.h>
#include "sdcard_defines.h"

#ifdef __cplusplus
extern "C" {
#endif

bool sdcard_cmd0_go_idle_state(sdcard_t * sdcard);

bool sdcard_cmd2_all_send_cid(sdcard_t * sdcard);

bool sdcard_cmd3_send_relative_addr(sdcard_t * sdcard);

bool sdcard_cmd6_switch_func(sdcard_t * sdcard, sdcard_bus_speed_t bus_speed);

bool sdcard_cmd7_select_card(sdcard_t * sdcard);

bool sdcard_cmd7_deselect_card(sdcard_t * sdcard);

bool sdcard_cmd8_send_if_cond(sdcard_t * sdcard);

bool sdcard_cmd9_send_csd(sdcard_t * sdcard);

bool sdcard_cmd13_send_status(sdcard_t * sdcard);

bool sdcard_cmd16_set_blocklen(sdcard_t * sdcard, uint16_t block_len);

bool sdcard_cmd17_read_single_block(sdcard_t * sdcard, uint32_t address, bool use_dma);

bool sdcard_cmd18_read_multiple_block(sdcard_t * sdcard, uint32_t address, uint16_t n_blocks, bool use_dma);

bool sdcard_cmd24_write_block(sdcard_t * sdcard, uint32_t address, bool use_dma);

bool sdcard_cmd25_write_multiple_block(sdcard_t * sdcard, uint32_t address, uint16_t n_blocks, bool use_dma);

bool sdcard_cmd32_erase_wr_blk_start(sdcard_t * sdcard, uint32_t lba);

bool sdcard_cmd33_erase_wr_blk_end(sdcard_t * sdcard, uint32_t lba);

bool sdcard_cmd38_erase(sdcard_t * sdcard, sdcard_erase_mode_t erase_mode);

bool sdcard_acmd6_set_bus_width(sdcard_t * sdcard, sdcard_bus_width_t bus_width);

bool sdcard_acmd41_sd_send_op_cond(sdcard_t * sdcard);

#ifdef __cplusplus
}
#endif

#endif /* SDCARD_COMMANDS_H */