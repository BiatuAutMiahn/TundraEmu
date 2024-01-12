#include "gfx.h"
#include "util.h"
#include <stdint.h>
#include <fstream>
#include <streambuf>
#include <iostream>

void setPixel(uint8_t buf[], uint16_t x, uint16_t y, uint8_t b, uint8_t m) {
  uint16_t off = map(x, 1, 319, 0, 159);
  uint8_t newp = 0;
  if (y > 1) off += 160 * (y - 1);
  uint8_t p = buf[off];
  uint16_t hi = (p >> 4) & 0xf;
  uint16_t lo = p & 0xf;
  if (x % 2 == 0) {
    if (m == 1) {  // Add
      lo += b;
    } else if (m == 2) {  // Subtract
      lo -= b;
    } else if (m == 3) {  // AND
      lo &= b;
    } else if (m == 4) {  // OR
      lo |= b;
    } else if (m == 5) {  // XOR
      lo ^= b;
    } else if (m == 6) {  // Divide
      lo /= b;
    } else if (m == 7) {  // Multiply
      lo *= b;
    } else if (m == 8) {  // Greater Than
      if (lo > b) lo = b;
    } else if (m == 9) {  // Less Than
      if (lo < b) lo = b;
    } else if (m == 10) {  // Greater Than Or Equal
      if (lo >= b) lo = b;
    } else if (m == 11) {  // Less Than Or Equal
      if (lo <= b) lo = b;
    } else {
      lo = b;
    }
  } else {
    if (m == 1) {
      hi += b;
    } else if (m == 2) {
      hi -= b;
    } else if (m == 3) {
      hi &= b;
    } else if (m == 4) {
      hi |= b;
    } else if (m == 5) {
      hi ^= b;
    } else if (m == 6) {
      hi /= b;
    } else if (m == 7) {
      hi *= b;
    } else if (m == 8) {  // Greater Than
      if (hi > b) hi = b;
    } else if (m == 9) {  // Less Than
      if (hi < b) hi = b;
    } else if (m == 10) {  // Greater Than Or Equal
      if (hi >= b) hi = b;
    } else if (m == 11) {  // Less Than Or Equal
      if (hi <= b) hi = b;
    } else {
      hi = b;
    }
  }
  if (hi >= UINT8_MAX) hi = 0x0;
  if (lo >= UINT8_MAX) lo = 0x0;
  if (hi < 0) hi = 0x0;
  if (lo < 0) lo = 0x0;
  if (hi > 0xf) hi = 0xf;
  if (lo > 0xf) lo = 0xf;
  newp = (lo | (hi << 4));
  memset(&buf[off], newp, 1);
}

void drawLine(uint8_t buf[], uint16_t x0, uint16_t y0, uint16_t x1,
              uint16_t y1, uint8_t c, uint8_t m) {
  uint16_t steep = abs(y1 - y0) > abs(x1 - x0);
  if (steep) {
    swapit(x0, y0);
    swapit(x1, y1);
  }

  if (x0 > x1) {
    swapit(x0, x1);
    swapit(y0, y1);
  }

  uint16_t dx, dy;
  dx = x1 - x0;
  dy = abs(y1 - y0);

  int16_t err = dx / 2;
  int16_t ystep;

  if (y0 < y1) {
    ystep = 1;
  } else {
    ystep = -1;
  }

  for (; x0 < x1; x0++) {
    if (steep) {
      setPixel(buf, y0, x0, c, m);
    } else {
      setPixel(buf, x0, y0, c, m);
    }
    err -= dy;
    if (err < 0) {
      y0 += ystep;
      err += dx;
    }
  }
}

void drawLineH(uint8_t buf[], uint16_t x, uint16_t y, uint16_t w,
               uint8_t c, uint8_t m) {
  drawLine(buf, x, y, x + w, y, c, m);
}
void drawLineV(uint8_t buf[], uint16_t x, uint16_t y, uint16_t h,
               uint8_t c, uint8_t m) {
  drawLine(buf, x, y, x, y + h, c, m);
}
void drawRect(uint8_t buf[], uint16_t x, uint16_t y, uint16_t w,
              uint16_t h, uint8_t c, uint8_t m) {
  uint16_t tempHeight;

  drawLineH(buf, x, y, w, c, m);
  drawLineH(buf, x, y + h - 1, w, c, m);

  tempHeight = h - 2;
  if (tempHeight < 1) return;

  drawLineV(buf, x, y + 1, tempHeight, c, m);
  drawLineV(buf, x + w - 1, y + 1, tempHeight, c, m);
}

void drawRectFill(uint8_t buf[], uint16_t x, uint16_t y, uint16_t width,
                  uint16_t height, uint8_t color, uint8_t mode) {
  // TODO - need to optimise the memory map draw so that this function will not
  // call pixel one by one
  for (int i = x; i < x + width; i++) {
    drawLineV(buf, i, y, height, color, mode);
  }
}

void drawTriangle(uint8_t buf[], int16_t x0, int16_t y0, int16_t x1, int16_t y1,
                  int16_t x2, int16_t y2, uint8_t c, uint8_t m) {
  drawLine(buf, x0, y0, x1, y1, c, m);
  drawLine(buf, x1, y1, x2, y2, c, m);
  drawLine(buf, x2, y2, x0, y0, c, m);
}

void drawFillTriangle(uint8_t buf[], int16_t x0, int16_t y0, int16_t x1,
                      int16_t y1, int16_t x2, int16_t y2, uint8_t c,
                      uint8_t m) {
  int16_t a, b, y, last;

  // Sort coordinates by Y order (y2 >= y1 >= y0)
  if (y0 > y1) {
    swapit(y0, y1);
    swapit(x0, x1);
  }
  if (y1 > y2) {
    swapit(y2, y1);
    swapit(x2, x1);
  }
  if (y0 > y1) {
    swapit(y0, y1);
    swapit(x0, x1);
  }

  if (y0 == y2) {  // Handle awkward all-on-same-line case as its own thing
    a = b = x0;
    if (x1 < a)
      a = x1;
    else if (x1 > b)
      b = x1;
    if (x2 < a)
      a = x2;
    else if (x2 > b)
      b = x2;
    drawLineH(buf, a, y0, b - a + 1, c, m);
    return;
  }

  int16_t dx01 = x1 - x0, dy01 = y1 - y0, dx02 = x2 - x0, dy02 = y2 - y0,
          dx12 = x2 - x1, dy12 = y2 - y1;
  int32_t sa = 0, sb = 0;

  // For upper part of triangle, find scanline crossings for segments
  // 0-1 and 0-2.  If y1=y2 (flat-bottomed triangle), the scanline y1
  // is included here (and second loop will be skipped, avoiding a /0
  // error there), otherwise scanline y1 is skipped here and handled
  // in the second loop...which also avoids a /0 error here if y0=y1
  // (flat-topped triangle).
  if (y1 == y2)
    last = y1;  // Include y1 scanline
  else
    last = y1 - 1;  // Skip it

  for (y = y0; y <= last; y++) {
    a = x0 + sa / dy01;
    b = x0 + sb / dy02;
    sa += dx01;
    sb += dx02;
    /* longhand:
    a = x0 + (x1 - x0) * (y - y0) / (y1 - y0);
    b = x0 + (x2 - x0) * (y - y0) / (y2 - y0);
    */
    if (a > b) swapit(a, b);
    drawLineH(buf, a, y, b - a + 1, c, m);
  }

  // For lower part of triangle, find scanline crossings for segments
  // 0-2 and 1-2.  This loop is skipped if y1=y2.
  sa = (int32_t)dx12 * (y - y1);
  sb = (int32_t)dx02 * (y - y0);
  for (; y <= y2; y++) {
    a = x1 + sa / dy12;
    b = x0 + sb / dy02;
    sa += dx12;
    sb += dx02;
    /* longhand:
    a = x1 + (x2 - x1) * (y - y1) / (y2 - y1);
    b = x0 + (x2 - x0) * (y - y0) / (y2 - y0);
    */
    if (a > b) swapit(a, b);
    drawLineH(buf, a, y, b - a + 1, c, m);
  }
}

void loadImg(uint8_t buf[], wchar_t* fPath) {
  std::ifstream in(fPath, std::ios::binary);
  in.read(reinterpret_cast<char*>(&buf), sizeof(uint8_t) * 160 * 132);
}

void drawWnd(uint8_t buf[], uint16_t x, uint16_t y, uint16_t w, uint16_t h) {
  memset(oBgM, 0x00, oled_buf_size);
  drawRectFill(oBgM, x, y, w, h, 0xf, 0);
  uint8_t p;
  for (int o = 0; o < oled_buf_size; ++o) {
    p = oBgB[o] & oBgM[o];
    if (p) {
      buf[o] = p;
    }
  }
  drawRectFill(buf, x, y + 12, w, h - 12, 0x2, 2);
  // drawRectFill(buf, x, y, w, h, 0x2, 8);
  drawRaisedBox(buf, x, y, w, h);
}

void drawRaisedBox(uint8_t buf[], uint16_t x, uint16_t y, uint16_t w,
                   uint16_t h) {
  // drawRectFill(buf, x, y, w, h, 0x2, 8);
  drawLineH(buf, x - 1, y + h, w + 1, 0x4, 2);
  drawLineV(buf, x + w, y, h + 1, 0x4, 2);
  drawLineH(buf, x - 1, y, w + 1, 0x8, 1);
  drawLineV(buf, x - 1, y, h + 1, 0x8, 1);
}
void drawSunkenBox(uint8_t buf[], uint16_t x, uint16_t y, uint16_t w,
                   uint16_t h) {
  // drawRectFill(buf, x, y, w, h, 0x2, 8);
  drawLineH(buf, x - 1, y + h, w + 1, 0x8, 1);
  drawLineV(buf, x + w, y, h + 1, 0x8, 1);
  drawLineH(buf, x - 1, y, w + 1, 0x4, 2);
  drawLineV(buf, x - 1, y, h + 1, 0x4, 2);
}

void loadBg() {
  wsprintf(fNameOledBg, L"/img/bg/%s", oBgL.at(oiBg).c_str());
  wsprintf(fNameOledBgBlr, L"/img/bg/blur/%s", oBgL.at(oiBg).c_str());
  loadImg(oBg, fNameOledBg);
  loadImg(oBgB, fNameOledBgBlr);
}

void bufSetPixel(int32_t x, int32_t y, uint16_t c) {
  setPixel(oUI, x, y, c, 0);
}
