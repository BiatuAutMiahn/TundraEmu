#pragma once
// Header (.h) for font: b'Consolas' b'Regular' 10

#include <stdint.h>

extern const int TALLEST_CHAR_PIXELS;

extern const uint8_t consolas10_pixels[];

struct font_char {
    int offset;
    int w;
    int h;
    int left;
    int top;
    int advance;
};

extern const struct font_char consolas10_lookup[];
