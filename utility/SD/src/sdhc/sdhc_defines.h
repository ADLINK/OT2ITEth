#ifndef SDHC_DEFINES_H
#define SDHC_DEFINES_H

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    SDHC_E_OK = 0,
    SDHC_E_INVALID_DATA = -1,
    SDHC_E_ERR = -2,
    SDHC_E_TIMEOUT = -3,
    SDHC_E_CMDINHC = -4,
    SDHC_E_CMDINHD = -5,
    SDHC_E_EISTR = -6,
    SDHC_E_CLK_MODE_UNSUPPORTED = -7,
    SHDC_E_PVALEN = -8,
} sdhc_err_t;

typedef enum {
    SDHC_CLOCK_DIVIDED,
    SDHC_CLOCK_PROGRAMMABLE,
} sdhc_clock_mode_t;

typedef enum {
    SDHC_BUS_WIDTH_1 = 0x0,
    SDHC_BUS_WIDTH_4 = 0x2,
} sdhc_bus_width_t;

#ifdef __cplusplus
}
#endif

#endif /* SDHC_DEFINES_H */