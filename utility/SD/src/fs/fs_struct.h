#ifndef FS_STRUCT_H
#define FS_STRUCT_H

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct block_dev {
    void * _hw;
    uint32_t block_len;
    bool (*read_blocks)(struct block_dev * dev, void * destination, uint32_t block_address, uint32_t length);
    bool (*write_blocks)(struct block_dev * dev, const void * source, uint32_t block_address, uint32_t length);
    bool (*is_ready)(struct block_dev * dev);
} block_device_t;

#ifdef __cplusplus
}
#endif

#endif /* FS_STRUCT_H */