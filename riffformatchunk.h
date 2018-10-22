#pragma once

#include <stdint.h>

namespace format_tag {
    const uint16_t PCM = 0x0001;
    }

struct RiffFormatChunk
    {
    union {
        char bytes[16];
        struct {
            uint16_t format_tag;
            uint16_t channels;
            uint32_t samples_per_second;
            uint32_t avg_bytes_per_second;
            uint16_t block_alignment;
            uint16_t bits_per_sample;
            } data;
        };
    };
