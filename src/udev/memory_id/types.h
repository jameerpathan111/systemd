#pragma once

#include <inttypes.h>
#include "dmidecode-config.h"

struct dmi_header
{
        uint8_t type;
        uint8_t length;
        uint16_t handle;
        uint8_t *data;
};

/*
 * You may use the following defines to adjust the type definitions
 * depending on the architecture:
 * - Define ALIGNMENT_WORKAROUND if your system doesn't support
 *   non-aligned memory access. In this case, we use a slower, but safer,
 *   memory access method. This should be done automatically in config.h
 *   for architectures which need it.
 */

#include <endian.h>

#if __BYTE_ORDER == __BIG_ENDIAN
typedef struct {
        uint32_t h;
        uint32_t l;
} u64;
#else
typedef struct {
        uint32_t l;
        uint32_t h;
} u64;
#endif

#if defined(ALIGNMENT_WORKAROUND) || __BYTE_ORDER == __BIG_ENDIAN
static inline u64 U64(uint32_t low, uint32_t high)
{
        u64 self;

        self.l = low;
        self.h = high;

        return self;
}
#endif

/*
 * Per SMBIOS v2.8.0 and later, all structures assume a little-endian
 * ordering convention.
 */
#if defined(ALIGNMENT_WORKAROUND) || __BYTE_ORDER == __BIG_ENDIAN
#define WORD(x) (uint16_t)((x)[0] + ((x)[1] << 8))
#define DWORD(x) (uint32_t)((x)[0] + ((x)[1] << 8) + ((x)[2] << 16) + ((x)[3] << 24))
#define QWORD(x) (U64(DWORD(x), DWORD(x + 4)))
#else /* ALIGNMENT_WORKAROUND || BIGENDIAN */
#define WORD(x) (uint16_t)(*(const uint16_t *)(x))
#define DWORD(x) (uint32_t)(*(const uint32_t *)(x))
#define QWORD(x) (*(const u64 *)(x))
#endif /* ALIGNMENT_WORKAROUND || BIGENDIAN */
