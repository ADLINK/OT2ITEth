#include "sdhc.h"

#include <string.h>

static sdhc_err_t _sdhc_wait_busy(Sdhc * sdhc) {
    uint32_t timeout = 0xFFFFFFF;
    do {
        if(timeout-- == 0) {
            sdhc_reset_cmd(sdhc);
            return SDHC_E_TIMEOUT;
        }
    } while(!(sdhc->PSR.bit.DATLL));
    return SDHC_E_OK;
}

static sdhc_err_t _sdhc_cmd_execute(Sdhc * sdhc, uint32_t cr, uint32_t cmd, uint32_t arg) {
    cr |= SDHC_CR_CMDIDX(cmd) | SDHC_CR_CMDTYP_NORMAL;
    
    if(cmd & SDMMC_RESP_PRESENT) {
        if(cmd & SDMMC_RESP_136) {
            cr |= SDHC_CR_RESPTYP_136_BIT;
        } else if(cmd & SDMMC_RESP_BUSY) {
            cr |= SDHC_CR_RESPTYP_48_BIT_BUSY;
        } else {
            cr |= SDHC_CR_RESPTYP_48_BIT;
        }
    }

    sdhc->ARG1R.reg = arg;
    sdhc->CR.reg = cr;

    uint32_t mask =
        SDHC_EISTR_CMDTEO | SDHC_EISTR_CMDEND | 
        SDHC_EISTR_CMDIDX | SDHC_EISTR_DATTEO | 
        SDHC_EISTR_DATEND | SDHC_EISTR_ADMA;
    
    if ((cmd & SDMMC_RESP_CRC) == 0) {
        mask |= (SDHC_EISTR_CMDCRC | SDHC_EISTR_DATCRC);
    }

    volatile uint32_t timeout = 65536;
    do {
        if(sdhc->EISTR.reg & mask || timeout-- == 0) {
            sdhc->EISTR.reg = SDHC_EISTR_MASK;
            sdhc_reset_cmd(sdhc);
            return SDHC_E_EISTR;
        }
    } while (!sdhc->NISTR.bit.CMDC);

    if(!(cr & SDHC_CR_DPSEL_DATA)) {
        sdhc->NISTR.bit.CMDC = 1;
    }
    
    if(cmd & SDMMC_RESP_BUSY) {
        return _sdhc_wait_busy(sdhc);
    }

    return SDHC_E_OK;
}

sdhc_err_t sdhc_set_clk(Sdhc * sdhc, uint32_t freq_hz, sdhc_clock_mode_t clock_mode) {
    /* Do not set clock if preset value is enabled */
    if(sdhc->HC2R.bit.PVALEN)
        return SHDC_E_PVALEN;

    if(clock_mode == SDHC_CLOCK_PROGRAMMABLE && sdhc->CA1R.bit.CLKMULT == 0)
        return SDHC_E_CLK_MODE_UNSUPPORTED;

    uint32_t clk_base = sdhc->CA0R.bit.BASECLKF;
    if(clk_base == 0)
        clk_base = F_CPU;
    uint32_t clk_mul  = sdhc->CA1R.bit.CLKMULT;
    uint32_t clk_div;

    /* If programmable clock mode is enabled, baseclk is divided by 2 */
    if(clk_mul > 0) {
        clk_base = clk_base >> 1;
    }

    /* If clock is enabled, wait until lines are not inhibited
    before turning it off */
    if (sdhc->CCR.bit.SDCLKEN) {
        while(sdhc->PSR.bit.CMDINHC || sdhc->PSR.bit.CMDINHD) {};
        sdhc->CCR.bit.SDCLKEN = 0;
    }

    switch(clock_mode) {
        default:
        case SDHC_CLOCK_DIVIDED:
            sdhc->CCR.bit.CLKGSEL = SDHC_CCR_CLKGSEL_DIV_Val;

            clk_div = (clk_base / freq_hz) >> 1;
        break;
        case SDHC_CLOCK_PROGRAMMABLE:
            sdhc->CCR.bit.CLKGSEL = SDHC_CCR_CLKGSEL_PROG_Val;

            clk_div = (clk_base * (clk_mul + 1)) / freq_hz;
            if(clk_div > 0)
                clk_div--;
        break;
    }

	/* The clock divider (DIV) in SDMMC_CCR must be set 
    to a value different from 0 when HSEN is 1. */
    if(sdhc->HC1R.bit.HSEN && (clk_div == 0)) {
        clk_div = 1;
    }

    sdhc->CCR.bit.SDCLKFSEL  = clk_div & 0xFF;
    sdhc->CCR.bit.USDCLKFSEL = clk_div >> 8;

    /* Enable internal clock and wait until it is stable 
    before turning the clock line on. */
    sdhc->CCR.bit.INTCLKEN   = 1;
    while(sdhc->CCR.bit.INTCLKS == 0) {}

    sdhc->CCR.bit.SDCLKEN    = 1;
    return SDHC_E_OK;
}

void sdhc_set_bus_width(Sdhc * sdhc, sdhc_bus_width_t width) {
    switch(width) {
        case SDHC_BUS_WIDTH_1:
            sdhc->HC1R.bit.DW = SDHC_HC1R_DW_1BIT_Val;
            break;
        case SDHC_BUS_WIDTH_4:
            sdhc->HC1R.bit.DW = SDHC_HC1R_DW_4BIT_Val;
            break;
    }
}

uint8_t sdhc_has_high_speed_support(Sdhc * sdhc) {
    return sdhc->CA0R.bit.HSSUP;
}

void sdhc_set_high_speed(Sdhc * sdhc, uint8_t high_speed) {
    sdhc->HC1R.bit.HSEN = (high_speed & 1);
}

void sdhc_reset_dat(Sdhc * sdhc) {
    sdhc->SRR.bit.SWRSTDAT = 1;
}

void sdhc_reset_cmd(Sdhc * sdhc) {
    sdhc->SRR.bit.SWRSTCMD = 1;
}

void sdhc_reset_all(Sdhc * sdhc) {
    sdhc->SRR.bit.SWRSTALL = 1;
    while(sdhc->SRR.bit.SWRSTALL) {}
}

void sdhc_driver_init(Sdhc * sdhc) {
    sdhc_reset_all(sdhc);

    /* Timeout(us) = 2^(13 + dtcval) / Baseclock(MHz)
    For a base clock of 120MHz, the timeout will be around 1us. */
    sdhc->TCR.reg = SDHC_TCR_DTCVAL(0xE);

    /* Set power supply to 3.3 volts */
    sdhc->PCR.reg = SDHC_PCR_SDBPWR | SDHC_PCR_SDBVSEL_3V3;

    /* Turn all status register bits on */
    sdhc->NISTER.reg = SDHC_NISTER_MASK;
    sdhc->EISTER.reg = SDHC_EISTER_MASK;
}

sdhc_err_t sdhc_cmd(Sdhc * sdhc, uint32_t cmd, uint32_t arg) {
    if(sdhc->PSR.bit.CMDINHC) {
        return SDHC_E_CMDINHC;
    }

    sdhc->TMR.bit.DMAEN = 0;
    sdhc->BCR.reg       = 0;

    return _sdhc_cmd_execute(sdhc, 0, cmd, arg);
}

sdhc_err_t sdhc_adtc_start(Sdhc * sdhc, uint32_t cmd, uint32_t arg, uint16_t block_len, uint16_t n_blocks, uint8_t use_dma) {
    uint32_t tmr;

	if (sdhc->PSR.bit.CMDINHC) {
		return SDHC_E_CMDINHC;
	}
    if (sdhc->PSR.bit.CMDINHD) {
        return SDHC_E_CMDINHD;
    }

    if(cmd & SDMMC_CMD_WRITE) {
        tmr = SDHC_TMR_DTDSEL_WRITE;
    } else {
        tmr = SDHC_TMR_DTDSEL_READ;
    }

    if(cmd & SDMMC_CMD_SDIO_BYTE) {
        tmr |= SDHC_TMR_MSBSEL_SINGLE;
    } else if(cmd & SDMMC_CMD_SDIO_BLOCK) {
        tmr |= SDHC_TMR_MSBSEL_MULTIPLE;
    } else if (cmd & SDMMC_CMD_SINGLE_BLOCK) {
		tmr |= SDHC_TMR_MSBSEL_SINGLE;
	} else if (cmd & SDMMC_CMD_MULTI_BLOCK) {
		tmr |= SDHC_TMR_BCEN_ENABLE;
        tmr |= SDHC_TMR_MSBSEL_MULTIPLE;
	} else {
		return SDHC_E_INVALID_DATA;
	}

    sdhc->TMR.reg = tmr;
    if(n_blocks > 1) {
        sdhc->TMR.bit.ACMDEN = SDHC_TMR_ACMDEN_CMD12_Val;
    }
    
    if(use_dma) {
        sdhc->TMR.bit.DMAEN = 1;
        sdhc->BSR.bit.BOUNDARY = SDHC_BSR_BOUNDARY_4K_Val;
    }
    sdhc->BSR.bit.BLOCKSIZE = block_len;
    sdhc->BCR.bit.BCNT = n_blocks;

    return _sdhc_cmd_execute(sdhc, SDHC_CR_DPSEL_DATA, cmd, arg);
}

void * sdhc_get_response_ptr(Sdhc * sdhc) {
    return (void*)sdhc->RR;
}

uint32_t sdhc_get_response_32bit(Sdhc * sdhc) {
    return sdhc->RR[0].reg;
}

uint32_t sdhc_get_response_auto_status(Sdhc * sdhc) {
    return sdhc->RR[3].reg;
}

void sdhc_get_response_120bit(Sdhc * sdhc, void * response) {
    memcpy((uint8_t*)response, (uint8_t*)sdhc->RR, 15);
}

sdhc_err_t sdhc_read_block(Sdhc * sdhc, void * destination, uint16_t block_len) {
    uint32_t * p_dst = (uint32_t*)destination;
    do {
        if(sdhc->EISTR.reg)
            return SDHC_E_EISTR;
    } while(!sdhc->NISTR.bit.BRDRDY);
    sdhc->NISTR.bit.BRDRDY = 1;

    for(int i = 0; i < block_len; i+=4) {
        p_dst[i>>2] = sdhc->BDPR.reg;
    }
    return SDHC_E_OK;
}

sdhc_err_t sdhc_read_blocks(Sdhc * sdhc, void * destination, uint16_t block_len, uint16_t n_blocks) {
    sdhc_err_t err;
    uint16_t position = 0;
    uint16_t blocks_remaining = n_blocks;

    uint8_t * p_dst = (uint8_t*)destination;

    while(blocks_remaining) {
        err = sdhc_read_block(sdhc, p_dst+position, block_len);
        if(err)
            return err;
        position+=block_len;
        blocks_remaining--;
    }
    do {
        if(sdhc->EISTR.reg)
            return SDHC_E_EISTR;
    } while(!sdhc->NISTR.bit.TRFC);
    sdhc->NISTR.bit.TRFC = 1;

    return SDHC_E_OK;
}

sdhc_err_t sdhc_write_block(Sdhc * sdhc, const void * source, uint16_t block_len) {
    const uint32_t * p_src = (const uint32_t*)source;
    do {
        if(sdhc->EISTR.reg)
            return SDHC_E_EISTR;
    } while(!sdhc->NISTR.bit.BWRRDY);
    sdhc->NISTR.bit.BWRRDY = 1;

    for(int i = 0; i < block_len; i+=4) {
        sdhc->BDPR.reg = p_src[i>>2];
    }
    return SDHC_E_OK;
}

sdhc_err_t sdhc_write_blocks(Sdhc * sdhc, const void * source, uint16_t block_len, uint16_t n_blocks) {
    sdhc_err_t err;
    uint16_t position = 0;
    uint16_t blocks_remaining = n_blocks;

    const uint8_t * p_src = (const uint8_t*)source;

    while(blocks_remaining) {
        err = sdhc_write_block(sdhc, p_src+position, block_len);
        if(err)
            return err;
        position+=block_len;
        blocks_remaining--;
    }
    do {
        if(sdhc->EISTR.reg)
            return SDHC_E_EISTR;
    } while(!sdhc->NISTR.bit.TRFC);
    sdhc->NISTR.bit.TRFC = 1;

    return SDHC_E_OK;
}

sdhc_err_t sdhc_wait_dma(Sdhc * sdhc) {
    while(!sdhc->NISTR.bit.TRFC) {
        if(sdhc->NISTR.bit.DMAINT) {
            sdhc->SSAR.reg = sdhc->SSAR.reg;
            sdhc->NISTR.bit.DMAINT = 1;
        }
    }
    sdhc->NISTR.bit.TRFC = 1;
    sdhc->TMR.bit.DMAEN = 0;
    return SDHC_E_OK;
}