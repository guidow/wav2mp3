#pragma once

#include <stdint.h>

struct RiffHeader
    {
    union {
        char bytes[12];
        struct {
            char name[4];
            uint32_t chunksize;
            char riff_type[4];
            } data;
        };
    };

struct RiffChunkHeader
    {
    union {
        char bytes[8];
        struct {
            char name[4];
            uint32_t chunksize;
            } data;
        };
    };
