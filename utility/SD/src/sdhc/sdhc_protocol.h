#ifndef SDHC_PROTOCOL_H
#define SDHC_PROTOCOL_H

//! \name Flags used to define a SD/MMC/SDIO command
//! @{
#define SDMMC_CMD_GET_INDEX(cmd) (cmd & 0x3F)

#define SDMMC_RESP_Msk (1lu << 8)
//! Have response (MCI only)
#define SDMMC_RESP_PRESENT  (1lu << 8)
//! 8 bit response (SPI only)
#define SDMMC_RESP_8 (1lu << 9)
//! 32 bit response (SPI only)
#define SDMMC_RESP_32 (1lu << 10)
//! 136 bit response (MCI only)
#define SDMMC_RESP_136 (1lu << 11)
//! Expect valid crc (MCI only)
#define SDMMC_RESP_CRC (1lu << 12)
//! Card may send busy
#define SDMMC_RESP_BUSY (1lu << 13)

//! To signal a data write operation
#define SDMMC_CMD_WRITE (1lu << 15)
//! To signal a SDIO tranfer in multi byte mode
#define SDMMC_CMD_SDIO_BYTE (1lu << 16)
//! To signal a SDIO tranfer in block mode
#define SDMMC_CMD_SDIO_BLOCK (1lu << 17)
//! To signal a data transfer in stream mode
#define SDMMC_CMD_STREAM (1lu << 18)
//! To signal a data transfer in single block mode
#define SDMMC_CMD_SINGLE_BLOCK (1lu << 19)
//! To signal a data transfer in multi block mode
#define SDMMC_CMD_MULTI_BLOCK (1lu << 20)
//! @}

//! \name SD Switch Status Fields
//! @{
#define SD_SW_STATUS_BIT_SIZE 512                      //!< 512 bits
#define SD_SW_STATUS_BSIZE (SD_SW_STATUS_BIT_SIZE / 8) //!< 64 bytes
//! @}

//! \name Set of flags to define a reponse type
//! @{
#define SDMMC_CMD_NO_RESP   (0)
#define SDMMC_CMD_R1        (SDMMC_RESP_PRESENT | SDMMC_RESP_CRC)
#define SDMMC_CMD_R1B       (SDMMC_RESP_PRESENT | SDMMC_RESP_CRC | SDMMC_RESP_BUSY)
#define SDMMC_CMD_R2        (SDMMC_RESP_PRESENT | SDMMC_RESP_8 | SDMMC_RESP_136 | SDMMC_RESP_CRC)
#define SDMMC_CMD_R3        (SDMMC_RESP_PRESENT | SDMMC_RESP_32)
#define SDMMC_CMD_R4        (SDMMC_RESP_PRESENT | SDMMC_RESP_32)
#define SDMMC_CMD_R5        (SDMMC_RESP_PRESENT | SDMMC_RESP_8 | SDMMC_RESP_CRC)
#define SDMMC_CMD_R6        (SDMMC_RESP_PRESENT | SDMMC_RESP_CRC)
#define SDMMC_CMD_R7        (SDMMC_RESP_PRESENT | SDMMC_RESP_32 | SDMMC_RESP_CRC)
//! @}

/** Cmd0(bc): Reset all cards to idle state */
#define SDMMC_MCI_CMD0_GO_IDLE_STATE        (0 | SDMMC_CMD_NO_RESP)
/** Cmd2(bcr, R2): Ask the card to send its CID number (stuff but arg 0 used) */
#define SDMMC_CMD2_ALL_SEND_CID             (2 | SDMMC_CMD_R2)
/** SD Cmd3(bcr, R6): Ask the card to publish a new relative address (RCA) */
#define SD_CMD3_SEND_RELATIVE_ADDR          (3 | SDMMC_CMD_R6)
/** SDIO Cmd5(R4): Send operation condition */
#define SDIO_CMD5_SEND_OP_COND              (5 | SDMMC_CMD_R4)
/** Cmd7(ac, R1/R1b): Select/Deselect card
 * For SD: R1b only from the selected card.
 * For MMC: R1 while selecting from Stand-By State to Transfer State;
 *          R1b while selecting from Disconnected State to Programming State.
 */
#define SDMMC_CMD7_SELECT_CARD_CMD          (7 | SDMMC_CMD_R1B)
#define SDMMC_CMD7_DESELECT_CARD_CMD        (7 | SDMMC_CMD_R1)
/** SD Cmd8(bcr, R7) : Send SD Memory Card interface condition */
#define SD_CMD8_SEND_IF_COND                (8 | SDMMC_CMD_R7)
/** Cmd9 MCI (ac, R2): Addressed card sends its card-specific data (CSD) */
#define SDMMC_MCI_CMD9_SEND_CSD             (9 | SDMMC_CMD_R2)
/** Cmd13(ac, R1): Addressed card sends its status register. */
#define SDMMC_MCI_CMD13_SEND_STATUS         (13 | SDMMC_CMD_R1)

/*
 * --- Block-oriented read commands (class 2) ---
 */
/** Cmd16(ac, R1): Set the block length (in bytes) */
#define SDMMC_CMD16_SET_BLOCKLEN            (16 | SDMMC_CMD_R1)
/** Cmd17(adtc, R1): Read single block */
#define SDMMC_CMD17_READ_SINGLE_BLOCK       (17 | SDMMC_CMD_R1 | SDMMC_CMD_SINGLE_BLOCK)
/** Cmd18(adtc, R1): Read multiple block */
#define SDMMC_CMD18_READ_MULTIPLE_BLOCK     (18 | SDMMC_CMD_R1 | SDMMC_CMD_MULTI_BLOCK)

/*
 * --- Block-oriented write commands (class 4) ---
 */
/** MMC Cmd23(ac, R1): Set block count */
#define MMC_CMD23_SET_BLOCK_COUNT (23 | SDMMC_CMD_R1)
/** Cmd24(adtc, R1): Write block */
#define SDMMC_CMD24_WRITE_BLOCK (24 | SDMMC_CMD_R1 | SDMMC_CMD_WRITE | SDMMC_CMD_SINGLE_BLOCK)
/** Cmd25(adtc, R1): Write multiple block */
#define SDMMC_CMD25_WRITE_MULTIPLE_BLOCK (25 | SDMMC_CMD_R1 | SDMMC_CMD_WRITE | SDMMC_CMD_MULTI_BLOCK)

/** SD Cmd32(ac, R1): */
#define SD_CMD32_ERASE_WR_BLK_START (32 | SDMMC_CMD_R1)
/** SD Cmd33(ac, R1): */
#define SD_CMD33_ERASE_WR_BLK_END (33 | SDMMC_CMD_R1)
/** Cmd38(ac, R1B): */
#define SDMMC_CMD38_ERASE (38 | SDMMC_CMD_R1B)

/*
 * --- Application-specific commands (class 8) ---
 */
/**
 * Cmd55(ac, R1): Indicate to the card that the next command is an application
 * specific command rather than a standard command.
 */
#define SDMMC_CMD55_APP_CMD                 (55 | SDMMC_CMD_R1)

/**
 * ACMD41(bcr, R3): Send host capacity support information (HCS) and asks the
 * accessed card to send its operating condition register (OCR) content
 * in the response
 */
#define SD_MCI_ACMD41_SD_SEND_OP_COND       (41 | SDMMC_CMD_R3)
/**
 * SD Cmd6(adtc, R1) : Check switchable function (mode 0)
 * and switch card function (mode 1).
 */
#define SD_CMD6_SWITCH_FUNC (6 | SDMMC_CMD_R1 | SDMMC_CMD_SINGLE_BLOCK)
/** ACMD6(ac, R1): Define the data bus width */
#define SD_ACMD6_SET_BUS_WIDTH              (6 | SDMMC_CMD_R1)

#endif /* SDHC_PROTOCOL_H */