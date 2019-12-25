#ifndef __CRC32__
#define __CRC32__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

#define CRC32POLY			0x04C11DB7
#define CRC32POLY_REVERSE	0xEDB88320

typedef uint32_t crc32_t;

void crc32_init(void);
crc32_t crc32_compute(const void*, uint32_t);
int crc32_selfcheck(const void*, uint32_t, crc32_t);
int crc32_check(const void*, uint32_t);
uint8_t reflect(uint8_t);

#ifdef __cplusplus
}
#endif

#endif /* __CRC32__ */