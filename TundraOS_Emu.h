
#include <windows.h>
#include <objidl.h>
#include <gdiplus.h>
#include <vector>
//#include <OpenFontRender.h>
//#include "font-consolas.h"

#define OLED_WIDTH 320
#define OLED_HEIGHT 132
#define OLED_BUF_SIZE (OLED_WIDTH / 2) * OLED_HEIGHT

HDC hdc;
PAINTSTRUCT ps;
SIZE wndSize;
RECT wndRect;


char fNameOledBg[255];
char fNameOledBgBlr[255];
std::vector<std::string> oBgL;
uint8_t oiBg;
uint16_t yOffs;
//OpenFontRender ofr;

std::vector<std::vector<const char*>> uiOptions = {
    {"Settings", "Return", "-", "Logout", "Power", "Radio", "Background"},
    {"Admin", "Return", "Save", "Reset", "-", "Users", "Display", "Lights",
     "Motor"},
    {"Users", "Return", "Add", "Remove", "-"},
    {"Display", "Return", "Save", "Reset", "-"},
    {"Lights", "Return", "Save", "Reset", "-"},
    {"Motor", "Return", "Save", "Reset", "-"}
};

uint8_t oBg[OLED_BUF_SIZE];
uint8_t oBgM[OLED_BUF_SIZE];
uint8_t oBgB[OLED_BUF_SIZE];
uint8_t oUI[OLED_BUF_SIZE];
uint8_t oBuf[OLED_BUF_SIZE];
uint8_t oIconLg[118 * 118];
uint8_t oiScale = 6;

Gdiplus::Color gr42col(uint8_t c);

#define swapit(a, b) \
  {                  \
    uint16_t t = a;  \
    a = b;           \
    b = t;           \
  }

#define mapRange(a1, a2, b1, b2, s) (b1 + (s - a1) * (b2 - b1) / (a2 - a1))

void loadImg(uint8_t buf[], const wchar_t* fPath);
void loadImg(uint8_t buf[], const char* fPath);

void setPixel(uint8_t buf[], uint16_t x, uint16_t y, uint8_t b, uint8_t m);

void drawLine(uint8_t buf[], uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1,
              uint8_t c, uint8_t m);
void drawLineH(uint8_t buf[], uint16_t x, uint16_t y, uint16_t w, uint8_t c,
               uint8_t m);
void drawLineV(uint8_t buf[], uint16_t x, uint16_t y, uint16_t h, uint8_t c,
               uint8_t m);
void drawRect(uint8_t buf[], uint16_t x, uint16_t y, uint16_t w, uint16_t h,
              uint8_t c, uint8_t m);
void drawRectFill(uint8_t buf[], uint16_t x, uint16_t y, uint16_t width,
                  uint16_t height, uint8_t color, uint8_t mode);
void drawTriangle(uint8_t buf[], int16_t x0, int16_t y0, int16_t x1, int16_t y1,
                  int16_t x2, int16_t y2, uint8_t c, uint8_t m);
void drawFillTriangle(uint8_t buf[], int16_t x0, int16_t y0, int16_t x1,
                      int16_t y1, int16_t x2, int16_t y2, uint8_t c, uint8_t m);

void drawFillTriangle(uint8_t buf[], int16_t cx, int16_t cy, int16_t width, int16_t height, float rotation, uint8_t c, uint8_t m);

void drawRaisedBox(uint8_t buf[], uint16_t x, uint16_t y, uint16_t w,
    uint16_t h);

void drawRaisedBoxFill(uint8_t buf[], uint16_t x, uint16_t y, uint16_t w,
    uint16_t h);

void drawSunkenBox(uint8_t buf[], uint16_t x, uint16_t y, uint16_t w,
                   uint16_t h);

void drawWnd(uint8_t buf[], uint16_t x, uint16_t y, uint16_t w, uint16_t h);

void loadBg();

void bufSetPixel(int32_t x, int32_t y, uint16_t c);

void RedirectIOToConsole();

    //void OLED_ShowPicture_gray(uint16_t x, uint16_t y, uint16_t sizex,
//                           uint16_t sizey, const uint8_t bmp[]);
//

class gfxSurface {
public:
    uint16_t w;
    uint16_t h;
    uint16_t bufSize;
    uint16_t bitW;
    uint16_t bitH;
    uint8_t* buf;
    gfxSurface(uint16_t w, uint16_t h);
    uint16_t map(uint16_t a1, uint16_t a2, uint16_t b1, uint16_t b2, uint16_t s);
    //void setPixel(uint16_t w, uint16_t h, uint16_t x, uint16_t y, uint8_t b, uint8_t m);
    void setPixel(uint16_t x, uint16_t y, uint8_t b, uint8_t m);
    void drawLine(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint8_t c, uint8_t m);
    void drawLineH(uint16_t x, uint16_t y, uint16_t w, uint8_t c, uint8_t m);
    void drawLineV(uint16_t x, uint16_t y, uint16_t h, uint8_t c, uint8_t m);
    void drawRect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint8_t c, uint8_t m);
    void drawRectFill(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint8_t color, uint8_t mode);
    void drawTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint8_t c, uint8_t m);
    void drawFillTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint8_t c, uint8_t m);
    void drawFillTriangle(int16_t cx, int16_t cy, int16_t width, int16_t height, float rotation, uint8_t c, uint8_t m);
    void drawWnd(uint16_t x, uint16_t y, uint16_t w, uint16_t h, const char* title="");
    void drawRaisedBox(uint16_t x, uint16_t y, uint16_t w, uint16_t h);
    void drawRaisedBoxFill(uint16_t x, uint16_t y, uint16_t w, uint16_t h);
    void drawSunkenBox(uint16_t x, uint16_t y, uint16_t w, uint16_t h);
    void loadImg(const char* fPath, uint16_t xDst = 0, uint16_t yDst = 0, uint16_t wSrc = 0, uint16_t hSrc = 0, uint16_t xSrc = 0, uint16_t ySrc = 0);
    void clear(uint8_t c);
    uint8_t getPixel(uint16_t x, uint16_t y);
    uint8_t getPixel(uint8_t buf[], uint16_t x, uint16_t y);
    uint8_t getPixel(gfxSurface buf, uint16_t x, uint16_t y);
    void drawText(uint16_t pos_x, uint16_t pos_y, const char* s, uint8_t m=0);
};

void render();

gfxSurface::gfxSurface(uint16_t w, uint16_t h) {
    this->w = w;
    this->h = h;
    this->bitW = (w / 2) + this->w % 2;
    this->bitH = h;
    this->bufSize = this->bitW * this->bitH;
    this->buf = new uint8_t[this->bufSize];
    memset(this->buf, 0, this->bufSize);
};

uint16_t gfxSurface::map(uint16_t a1, uint16_t a2, uint16_t b1, uint16_t b2, uint16_t s) {
    return (b1 + (s - a1) * (b2 - b1) / (a2 - a1));
};
// uint8_t blendPixel(uint8_t c)
void gfxSurface::setPixel(uint16_t x, uint16_t y, uint8_t b, uint8_t m = 0) {
    if (x < 1 || y < 1 || y > this->h || x > this->w) return;
    uint16_t off = this->map(1, this->w - 1, 0, this->bitW - 1, x);
    uint16_t hi = 0;
    uint16_t lo = 0;
    uint8_t newp = 0;
    if (y > 1) off += this->bitW * (y - 1);
    uint8_t p = this->buf[off];
    if (p > 0) {
        hi = (p >> 4) & 0xf;
        lo = p & 0xf;
    }
    if (m > 0) {
        if (x % 2 == 0) {
            switch (m) {
              case 1:  // Add
                lo += b;
                break;
              case 2:  // Subtract
                lo -= b;
                break;
              case 3:  // AND
                lo &= b;
                break;
              case 4:  // OR
                lo |= b;
                break;
              case 5:  // XOR
                lo ^= b;
                break;
              case 6:  // Divide
                lo /= b;
                break;
              case 7:  // Multiply
                lo *= b;
                break;
              case 8:  // Greater Than
                if (lo > b) lo = b;
                break;
              case 9:  // Less Than
                if (lo < b) lo = b;
                break;
              case 10:  // Greater Than Or Equal
                if (lo >= b) lo = b;
                break;
              case 11:  // Less Than Or Equal
                if (lo <= b) lo = b;
                break;
              case 12:  // p25% , b75%
                lo = (lo + 3 * b) / 4;
                break;
              case 13:  // p75% , b25%
                lo = (3 * lo + b) / 4;
                break;
              case 14:  // inverted p25% , b75%
                lo = (lo + 3 * (b ^ 0xF)) / 4;
                break;
              case 15:  // invert
                lo = (b ^ 0xF);
                break;
              default:
                lo = b;
            }
        } else {
            switch (m) {
              case 1:  // Add
                hi += b;
                break;
              case 2:  // Subtract
                hi -= b;
                break;
              case 3:  // AND
                hi &= b;
                break;
              case 4:  // OR
                hi |= b;
                break;
              case 5:  // XOR
                hi ^= b;
                break;
              case 6:  // Divide
                hi /= b;
                break;
              case 7:  // Multiply
                hi *= b;
                break;
              case 8:  // Greater Than
                if (hi > b) hi = b;
                break;
              case 9:  // Less Than
                if (hi < b) hi = b;
                break;
              case 10:  // Greater Than Or Equal
                if (hi >= b) hi = b;
                break;
              case 11:  // Less Than Or Equal
                if (hi <= b) hi = b;
                break;
              case 12:  // p25% , b75%
                hi = (hi + 3 * b) / 4;
                break;
              case 13:  // p75% , b25%
                hi = (3 * hi + b) / 4;
                break;
              case 14:  // inverted p25% , b75%
                hi = (hi + 3 * (b ^ 0xF)) / 4;
                break;
              case 15:  // invert
                hi = (b ^ 0xF);
                break;
              default:
                hi = b;
            }
        }
    } else {
        if (x % 2 == 0) {
            lo = b;
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
    memset(&this->buf[off], newp, 1);
};

void gfxSurface::drawLine(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1,
                          uint8_t c, uint8_t m) {
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
            this->setPixel(y0, x0, c, m);
        } else {
            gfxSurface::setPixel(x0, y0, c, m);
        }
        err -= dy;
        if (err < 0) {
            y0 += ystep;
            err += dx;
        }
    }
}

void gfxSurface::drawLineH(uint16_t x, uint16_t y, uint16_t w, uint8_t c,
                           uint8_t m) {
    this->drawLine(x, y, x + w, y, c, m);
}
void gfxSurface::drawLineV(uint16_t x, uint16_t y, uint16_t h, uint8_t c,
                           uint8_t m) {
    this->drawLine(x, y, x, y + h, c, m);
}
void gfxSurface::drawRect(uint16_t x, uint16_t y, uint16_t w, uint16_t h,
                          uint8_t c, uint8_t m) {
    uint16_t tempHeight;

    this->drawLineH(x, y, w, c, m);
    this->drawLineH(x, y + h - 1, w, c, m);

    tempHeight = h - 2;
    if (tempHeight < 1) return;

    this->drawLineV(x, y + 1, tempHeight, c, m);
    this->drawLineV(x + w - 1, y + 1, tempHeight, c, m);
}

void gfxSurface::drawRectFill(uint16_t x, uint16_t y, uint16_t width,
                              uint16_t height, uint8_t color, uint8_t mode) {
    // TODO - need to optimise the memory map draw so that this function will
    // not call pixel one by one
    for (int i = y; i < y + height; i++) {
        this->drawLineH(x, i, width, color, mode);
    }
}

void gfxSurface::drawTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1,
                              int16_t x2, int16_t y2, uint8_t c, uint8_t m) {
    this->drawLine(x0, y0, x1, y1, c, m);
    this->drawLine(x1, y1, x2, y2, c, m);
    this->drawLine(x2, y2, x0, y0, c, m);
}

void gfxSurface::drawFillTriangle(int16_t x0, int16_t y0, int16_t x1,
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
        this->drawLineH(a, y0, b - a + 1, c, m);
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
        drawLineH(a, y, b - a + 1, c, m);
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
        this->drawLineH(a, y, b - a + 1, c, m);
    }
}

void gfxSurface::drawFillTriangle(int16_t cx, int16_t cy, int16_t width,
                                  int16_t height, float rotation, uint8_t c,
                                  uint8_t m) {
    // Calculate the center of the triangle
    int16_t centerX = cx;
    int16_t centerY = cy;

    // Calculate the vertices of the unrotated triangle
    int16_t x0 = centerX;
    int16_t y0 = centerY - height / 2;
    int16_t x1 = centerX - width / 2;
    int16_t y1 = centerY + height / 2;
    int16_t x2 = centerX + width / 2;
    int16_t y2 = centerY + height / 2;

    // Convert the rotation angle from degrees to radians
    float angle = rotation * 3.14159265358979323846 / 180.0;
    float cosA = cos(angle);
    float sinA = sin(angle);

    // Rotate the vertices around the center
    int16_t newX0 = centerX + (x0 - centerX) * cosA - (y0 - centerY) * sinA;
    int16_t newY0 = centerY + (x0 - centerX) * sinA + (y0 - centerY) * cosA;
    int16_t newX1 = centerX + (x1 - centerX) * cosA - (y1 - centerY) * sinA;
    int16_t newY1 = centerY + (x1 - centerX) * sinA + (y1 - centerY) * cosA;
    int16_t newX2 = centerX + (x2 - centerX) * cosA - (y2 - centerY) * sinA;
    int16_t newY2 = centerY + (x2 - centerX) * sinA + (y2 - centerY) * cosA;

    // Call the existing drawFillTriangle function with the rotated vertices
    this->drawFillTriangle(newX0, newY0, newX1, newY1, newX2, newY2, c, m);
}

void gfxSurface::drawWnd(uint16_t x, uint16_t y, uint16_t w, uint16_t h,
                         const char* title) {
    // memset(oBgM, 0x00, OLED_BUF_SIZE);
    this->drawRectFill(x, y, w, h, 0xC, 0);
    this->drawRectFill(x + 1, y + 1, w - 2, TALLEST_CHAR_PIXELS + 1, 0x2, 2);
    this->drawRaisedBox(x, y, w, h);
    this->drawText(x + 1, y - 1, title, 14);
}

void gfxSurface::drawRaisedBox(uint16_t x, uint16_t y, uint16_t w, uint16_t h) {
    // drawRectFill(x, y, w, h, 0x2, 8);
    // LeftTop
    this->drawLineH(x, y, w - 1, 0xC, 0);
    this->drawLineV(x, y, h - 1, 0xC, 0);
    this->drawLineH(x+1, y + 1, w - 3, 0xF, 0);
    this->drawLineV(x+1, y + 1, h - 3, 0xF, 0);
    // RightBottom
    this->drawLineH(x + 1, y+h-2, w, 0x8, 0);
    this->drawLineH(x, y+h-1, w, 0x4, 0);
    this->drawLineV(x+w-2, y + 1, h - 2, 0x8, 0);
    this->drawLineV(x+w-1, y, h, 0x4, 0);
}

//void drawRaisedBox(uint8_t buf[], uint16_t x, uint16_t y, uint16_t w,
//                   uint16_t h) {
//    // drawRectFill(buf, x, y, w, h, 0x2, 8);
//    // LeftTop
//    drawLineH(buf, x - 1, y - 1, w + 1, 0xF, 0);
//    drawLineH(buf, x - 2, y - 2, w + 3, 0xC, 0);
//    drawLineV(buf, x - 1, y - 1, h + 1, 0xF, 0);
//    drawLineV(buf, x - 2, y - 2, h + 3, 0xC, 0);
//    // RightBottom
//    drawLineH(buf, x - 1    , y + h    , w + 1, 0x8, 0);
//    drawLineH(buf, x - 2    , y + h + 1, w + 3, 0x0, 0);
//    drawLineV(buf, x + w    , y - 1    , h + 2, 0x8, 0);
//    drawLineV(buf, x + w + 1, y - 2    , h + 4, 0x0, 0);
//}

void gfxSurface::drawRaisedBoxFill(uint16_t x, uint16_t y, uint16_t w,
                                   uint16_t h) {
    this->drawRectFill(x, y, w, h, 0xC, 0);
    this->drawRaisedBox(x, y, w, h);
}

void gfxSurface::drawSunkenBox(uint16_t x, uint16_t y, uint16_t w, uint16_t h) {
    // drawRectFill(x, y, w, h, 0x2, 8);
    // LeftTop
    this->drawLineH(x - 1, y - 1, w + 1, 0x0, 0);
    this->drawLineH(x - 2, y - 2, w + 3, 0x8, 0);
    this->drawLineV(x - 1, y - 1, h + 1, 0x0, 0);
    this->drawLineV(x - 2, y - 2, h + 3, 0x8, 0);
    // RightBottom
    this->drawLineH(x - 1, y + h, w + 1, 0xC, 0);
    this->drawLineH(x - 2, y + h + 1, w + 3, 0xF, 0);
    this->drawLineV(x + w, y - 1, h + 2, 0xC, 0);
    this->drawLineV(x + w + 1, y - 2, h + 4, 0xF, 0);
}

void gfxSurface::clear(uint8_t c) { memset(this->buf, c, this->bufSize); }

uint8_t gfxSurface::getPixel(uint16_t x, uint16_t y) {
    if (x < 1 || y < 1 || y > this->h || x > this->w) return 0;
    uint16_t off = this->map(1, this->w - 1, 0, this->bitW - 1, x);
    uint8_t newp = 0;
    if (y > 1) off += this->bitW * (y - 1);
    uint8_t p = this->buf[off];
    uint16_t hi = (p >> 4) & 0xf;
    uint16_t lo = p & 0xf;
    if (x % 2 == 0) {
        return lo;
    } else {
        return hi;
    }
}

void gfxSurface::loadImg(const char* fPath, uint16_t xDst, uint16_t yDst,
                         uint16_t wSrc, uint16_t hSrc, uint16_t xSrc,
                         uint16_t ySrc) {
    std::ifstream inb(fPath, std::ios::binary);
    if (inb.is_open()) {
        uint16_t imgWidth, imgHeight;
        inb.read(reinterpret_cast<char*>(&imgWidth), sizeof(uint16_t));
        inb.read(reinterpret_cast<char*>(&imgHeight), sizeof(uint16_t));
        if (ySrc == 0) ySrc = yDst;
        if (xSrc == 0) xSrc = xDst;
        if (wSrc == 0) wSrc = imgWidth - xSrc;
        if (hSrc == 0) hSrc = imgHeight - ySrc;
        // ySrc = ySrc * 2;
        if (xSrc + wSrc <= imgWidth && ySrc + hSrc <= imgHeight) {
            gfxSurface* copyBuf;
            copyBuf = new gfxSurface(wSrc, hSrc);
            for (uint16_t h = 0; h < hSrc; ++h) {
              inb.seekg(4 + (xSrc / 2) + (h * (imgWidth / 2)) +
                        (ySrc * (imgWidth / 2)));
              inb.read(
                  reinterpret_cast<char*>(&copyBuf->buf[h * (copyBuf->bitW)]),
                  sizeof(uint8_t) * (copyBuf->bitW));
            }
            uint16_t x;
            uint16_t y;
            for (uint16_t h = 1; h <= hSrc; ++h) {
              for (uint16_t w = 1; w <= wSrc; ++w) {
                x = xDst + w;
                y = yDst + h;
                // if (xDst + w > this->w && yDst + h > this->h)
                //     continue;
                this->setPixel(x, y, copyBuf->getPixel(w, h));
              }
            }
        }
        inb.close();
    }
}

void gfxSurface::drawText(uint16_t pos_x, uint16_t pos_y, const char* s,
                          uint8_t m) {
    char c = *s;
    uint16_t dstx = 0;
    uint16_t srcw = 0;
    uint16_t off;
    uint8_t v;
    uint8_t lo;
    uint8_t hi;
    while (c) {
        const struct font_char font_c = consolas10_lookup[c];
        for (int y = 0; y < font_c.h; ++y) {
            dstx = 0;
            srcw = font_c.w / 2;
            if (font_c.w % 2) {
              ++srcw;
            }
            for (int x = 0; x < srcw; ++x) {
              off = font_c.offset + x + (y * srcw);
              v = consolas10_pixels[off];
              lo = (v >> 4) & 0xf;
              hi = v & 0xf;
              if (lo != 0)
                this->setPixel(1 + pos_x + dstx + font_c.left,
                               pos_y + y + font_c.top, lo, m);
              if (hi != 0)
                this->setPixel(1 + pos_x + dstx + 1 + font_c.left,
                               pos_y + y + font_c.top, hi, m);
              dstx += 2;
              // buf[pos_x + x + font_c.left + (pos_y + y + font_c.top) * 2] =
              // (hi << 4) | hi; buf[pos_x + x + 1 + font_c.left + (pos_y + y
              // + font_c.top) * 2] = (lo << 4) | lo;
            }
            if (font_c.w % 2) {
              ++dstx;
            }
        }
        pos_x += font_c.advance;
        c = *(++s);
    }
}



class guiElement {
   public:
    void blit(uint8_t buf[], uint16_t x, uint16_t y, uint8_t m);
    void blit(gfxSurface* buf, uint16_t x, uint16_t y, uint8_t m);
    guiElement();
    gfxSurface* bitmBuf;

   protected:
    bool bDraw;
};
guiElement::guiElement() { this->bDraw = false; };
void guiElement::blit(uint8_t buf[], uint16_t x, uint16_t y, uint8_t m = 0) {
    uint16_t yPos = y + 1;
    uint16_t xOff = x;
    uint16_t xPos = 0;
    uint16_t yOff = 0;
    for (uint16_t i = 0; i < this->bitmBuf->bufSize; i++) {
        yOff = (160 * (yPos - 1));
        buf[xOff + xPos + yOff] = this->bitmBuf->buf[i];
        xPos++;
        if (xPos > this->bitmBuf->w / 2) {
            xPos = 0;
            yPos++;
        }
    }
};
void guiElement::blit(gfxSurface* buf, uint16_t x, uint16_t y, uint8_t m = 0) {
    uint16_t o = 1;
    for (uint16_t i = 1; i <= this->bitmBuf->h; ++i) {
        for (uint16_t j = 1; j <= this->bitmBuf->w; ++j) {
            uint8_t pix = this->bitmBuf->getPixel(j, i);
            buf->setPixel(x + j - 1, y + i - 1, pix, m);
        }
    }
}

class guiButton : public guiElement {
   public:
    uint16_t w;
    uint16_t h;
    uint16_t type;
    void draw(uint16_t mh, bool bReDraw);
    guiButton(uint8_t type, uint16_t w, uint16_t h);
};
guiButton::guiButton(uint8_t type, uint16_t w, uint16_t h = 1) {
    this->w = w;
    this->h = h;
    this->bitmBuf = new gfxSurface(w, h);
    this->type = type;
};
void guiButton::draw(uint16_t mh = 0, bool bReDraw = false) {
    if (this->bDraw && !bReDraw) return;
    if (mh > 0) {
        delete this->bitmBuf;
        this->h = mh;
        this->bitmBuf = new gfxSurface(w, h);
    }
    // this->bitmBuf->setPixel(1, 1, 0xf,0);
    // this->bitmBuf->setPixel(this->w, this->h, 0xf, 0);
    // this->bitmBuf->setPixel(1, this->h, 0xf, 0);
    // this->bitmBuf->setPixel(this->w, 1, 0xf,0);
    this->bitmBuf->drawRectFill(3, 3, this->w - 4, this->h - 4, 0xC, 0);
    this->bitmBuf->drawRaisedBox(1, 1, this->w, this->h);
    if (this->type == 0) return;
    if (this->type == 1) {
        this->bitmBuf->drawFillTriangle(
            (this->w / 2) + (this->w % 2), (this->h / 2) + (this->h % 2),
            this->w / (this->h % 2 ? 2 : 2), this->h / ((this->h % 2) ? 3 : 3),
            0, 0x0, 0);
    } else if (this->type == 2) {
        this->bitmBuf->drawFillTriangle(
            (this->w / 2) + (this->w % 2), (this->h / 2) + (this->h % 2),
            this->w / (this->h % 2 ? 2 : 2), this->h / ((this->h % 2) ? 3 : 3),
            180, 0x0, 0);
    }
    this->bDraw = true;
};

class guiScrollbar : public guiElement {
   public:
    uint16_t w;
    uint16_t h;
    void draw(uint16_t y, uint16_t th, bool bReDraw);
    guiScrollbar(uint16_t pw, uint16_t ph, uint16_t w);
    // void setThumbHeight(uint16_t);
   private:
    uint16_t pw;
    uint16_t ph;
    uint16_t ihThumb;
    guiButton* btnUp;
    guiButton* btnDown;
    guiButton* btnThumb;
};
guiScrollbar::guiScrollbar(uint16_t pw, uint16_t ph, uint16_t w = 11) {
    this->w = w;
    this->h = ph;
    this->pw = pw;
    this->ph = ph;
    // this->bitmSize = this->w * this->h;
    // this->bitmBuf = new uint8_t[this->bitmSize];
    this->bitmBuf = new gfxSurface(this->w, this->h);
    this->ihThumb = 5;
    this->btnUp = new guiButton(1, w, w);
    this->btnDown = new guiButton(2, w, w);
    this->btnThumb = new guiButton(0, w, this->ihThumb);
};
void guiScrollbar::draw(uint16_t pos, uint16_t th = 0, bool bReDraw = false) {
    if (this->bDraw && !bReDraw) return;
    uint8_t bgp;
    this->btnUp->draw();
    for (uint16_t ybg = this->btnUp->h;
         ybg < (this->h + this->btnUp->h) - this->btnDown->h; ++ybg) {
        for (uint16_t xbg = 0; xbg < this->w + 1; ++xbg) {
            if (ybg % 2) {
              bgp = xbg % 2 ? 0xf : 0xc;
            } else {
              bgp = xbg % 2 ? 0xc : 0xf;
            }
            this->bitmBuf->setPixel(xbg, ybg, bgp, 0);
        }
    }
    this->btnDown->draw();
    if (th > 0) {
        this->ihThumb = th;
        if (!this->bDraw) this->btnThumb->draw(this->ihThumb, true);
    } else {
        this->btnThumb->draw();
    }
    this->btnUp->blit(this->bitmBuf, 1, 1);
    this->btnDown->blit(this->bitmBuf, 1, this->h - this->btnDown->h + 1);
    this->btnThumb->blit(
        this->bitmBuf, 1,
        this->btnUp->h + pos);  //, this->h - this->btnDown->h);
    this->bDraw = true;
}
/*
  float viewportHeight = (h / (2+lineHeight))-1;
  float scrollInitialHeight = 9 * (viewportHeight - 1);
  float percVisible = (100*viewportHeight) / oBgL.size();
  uint16_t ScrollbarTop = y + 12 + 5 + 9 + 4;
  float thumbHeight = ((scrollInitialHeight * percVisible) / 100) + 2;
  float maxScrollBarY = (y + h - 12 - 2) - thumbHeight - 3;

float thumb_pos;
if (viewportHeight >= oBgL.size()) {
    thumbHeight = scrollInitialHeight;
    thumb_pos = ScrollbarTop;
}
else {
    float vsel = oiBg;
    float vmin = 0;
    float vmax = oBgL.size() - 1;
    float smax = maxScrollBarY;
    if ((int)oBgL.size() % 2)
        smax -= 1;
    float smin = ScrollbarTop;
    thumb_pos = (vsel - vmin) / (vmax - vmin) * (smax - smin) + smin;
}
drawRaisedBoxFill(oBuf, x + w - 12 - 2, thumb_pos, 9, thumbHeight);
*/


class tundUI {
   public:
    uint16_t x;
    uint16_t y;
    uint16_t w;
    uint16_t h;
    const char* title;
    tundUI(uint16_t x, uint16_t y, uint16_t w, uint16_t h, const char* title);
    void draw(uint16_t y, uint16_t th, bool bReDraw);
};
tundUI::tundUI(uint16_t x, uint16_t y, uint16_t w, uint16_t h, const char *title){
    this->x = x;
    this->y = y;
    this->w = w;
    this->h = h;
    this->title = title;
}

// tundUI(guiWindow(guiSetting(guiListBox(guiScrollBar()))))