#include <windows.h>
#include <gdiplus.h>
#include <stdint.h>

double map(double a1, double a2, double b1, double b2, double s) {
  return b1 + (s - a1) * (b2 - b1) / (a2 - a1);
}

Gdiplus::Color gr42col(uint8_t c) {
  BYTE col = c | c << 4;
  return RGB(col, col, col);
}
