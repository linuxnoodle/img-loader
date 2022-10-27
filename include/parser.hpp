#pragma once

#include "common.hpp"
#include <fstream>
#include <cinttypes>

// critical -> IHDR, PLTE, IDAT, IEND
// corresponding number determined by sum of ascii values of characters in type
enum CHTYPE {
    IHDR = 295,
    PLTE = 309,
    IDAT = 290,
    IEND = 288
};

struct chunk {
    CHTYPE chunk_type;
    uint8_t *chunk_data;
    uint8_t chunk_crc[4];
};

struct image {
    // unfortunately there is no uint4_t or uint1_t
    uint8_t width, height, bit_depth, color_type, compression_method, filter_method, interlace_method;
    std::vector<chunk> chunks;
};

image parse_png(std::string name);
