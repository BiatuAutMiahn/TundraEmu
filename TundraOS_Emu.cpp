#include <string>
#include <fstream>
#include <streambuf>
#include <iostream>
#include <stdint.h>
#include <stdlib.h>
#include <fcntl.h>
#include <windows.h>
#include <objidl.h>
#include <gdiplus.h>
#include <tchar.h>
#include <stdio.h>
#include <math.h>
//#include "font-consolas.h"
//#include "OpenFontRender.h"
#include "FontConsolas.h"
#include "TundraOS_Emu.h"
#include <io.h>

#define font_lookup consolas10_lookup
#define font_pixels consolas10_pixels

// #include "util.h"
//#include "gfx.h"

using namespace Gdiplus;
#pragma comment(lib, "Gdiplus.lib")
#pragma comment(lib, "FontConsolas.lib")
//#pragma comment(lib, "freetype242.lib")
//#pragma comment(lib, "OpenFontRender.lib")

class OLED {
 public:
  uint8_t width;
  uint8_t height;
  uint8_t bpp;
  void Init();
};
void OLED::Init() {
  width = 320;
  height = 132;
  bpp = 4;
}

uint8_t gdiBuf[160 * 132];
uint8_t bg[160 * 132];


//void drawList()

//guiScrollbar test(102,160);
//guiButton test(1, 9, 9);
gfxSurface *oled;
gfxSurface *oledBg;// (320, 132);
gfxSurface *oledBgBlr;// (320, 132);
gfxSurface *oledIcon;// (118, 118);
gfxSurface *testSurf;
guiScrollbar *testScroll;
guiButton *testBtn;
uint16_t iRot = 0;

VOID OnPaint(HDC hdc, HWND hWnd) {
    HDC memDC = CreateCompatibleDC(hdc);
    HBITMAP memBitmap = CreateCompatibleBitmap(hdc, wndSize.cx, wndSize.cx);
    SelectObject(memDC, memBitmap);
    Graphics g(memDC);

    //g.ScaleTransform(oiScale, oiScale);
    g.SetPixelOffsetMode(PixelOffsetModeHalf);
    g.SetCompositingMode(CompositingModeSourceCopy);
    //g.SetCompositingQuality(CompositingQualityAssumeLinear);
    g.SetInterpolationMode(InterpolationModeNearestNeighbor);
    g.SetSmoothingMode(SmoothingModeNone);
    Bitmap gdiBitmap(322, 135, PixelFormat32bppRGB);
    render();
    //test.clear(0);
    /*test.setPixel(1,1,0x5,0);

    test.drawFillTriangle(320 / 2, 132 / 2, 50, 50 * 0.8, iRot, 0x2, 0);
    test.drawFillTriangle(320 / 2, 132 / 2, 46, 46 * 0.8, iRot, 0x4, 0);
    test.drawFillTriangle(320 / 2, 132 / 2, 44, 44 * 0.8, iRot, 0x6, 0);
    test.drawFillTriangle(320 / 2, 132 / 2, 40, 40 * 0.8, iRot, 0xe, 0);
    test.drawFillTriangle(320 / 2, 132 / 2, 35, 35 * 0.8, iRot, 0xf, 0);
    iRot+=1;
    if (iRot > 360) iRot = 0;
    printf("%d\n", iRot);*/
    //render();
    for (int ya = 0; ya <= 133; ++ya) {
        gdiBitmap.SetPixel(0, ya, 0xFF888888);
        gdiBitmap.SetPixel(321, ya, 0xFF888888);
    }
    for (int xa = 0; xa <= 321; ++xa) {
        gdiBitmap.SetPixel(xa, 0, 0xFF888888);
        gdiBitmap.SetPixel(xa, 133, 0xFF888888);
    }

    //uint16_t x = 20;
    //uint16_t y = 20;
    //uint16_t w = 160;
    //uint16_t h = 100;
    //const char* line;
    //uint32_t lineHeight;
    //uint32_t yl;
    //uint8_t p = oBgL.size() / (10 + 2);  // Page count;
    //uint8_t r = oBgL.size() - p;         // Remain count;
    //memcpy(gdiBuf, oBg, OLED_BUF_SIZE);
    //memcpy(gdiBuf, oBuf, 160 * 132);
    //gdiBitmap.SetPixel(0, 0, gr42col(0xf));

    uint8_t hi;
    uint8_t lo;
    uint16_t xoff;
    uint16_t off;
    // uint16_t off = map(x, 1, 319, 0, 159);
    //uint8_t newp = 0;
    //if (y > 1) off += 160 * (y - 1);
    //uint8_t p = buf[off];
    //uint16_t hi = (p >> 4) & 0xf;
    for (int ya = 0; ya < 132; ++ya) {
        for (int xa = 0; xa < 320;xa+=2) {
          off = (xa/2)+(160*ya);
          //off += 160 * (ya-1);
          lo = (oled->buf[off] >> 4) & 0xf;
          hi = oled->buf[off] & 0xf;
          gdiBitmap.SetPixel(xa+1, ya+1, gr42col(lo));
          gdiBitmap.SetPixel(xa+2, ya+1, gr42col(hi));
        }
    }
    g.DrawImage(&gdiBitmap, oiScale, oiScale, (OLED_WIDTH * oiScale) + oiScale, (OLED_HEIGHT * oiScale) + oiScale);

    CLSID pngClsid;
    CLSIDFromString(L"{557CF406-1A04-11D3-9A73-0000F81EF32E}", &pngClsid);
    gdiBitmap.Save(L"export.png", &pngClsid, NULL);
    BitBlt(hdc, 0, 0, wndSize.cx, wndSize.cy, memDC, 0, 0, SRCCOPY);
    DeleteObject(memBitmap);
    DeleteDC(memDC);
}


LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, PSTR, INT iCmdShow) {
  HWND hWnd;
  MSG msg;
  WNDCLASS wndClass;
  GdiplusStartupInput gdiplusStartupInput;
  ULONG_PTR gdiplusToken;

  /*if (ofr.loadFont(binaryttf, sizeof(binaryttf))) {
    //Serial.println("OpenFontRender initialize error");
    return true;
  }
  ofr.setFontSize(10);
  ofr.setDrawPixel(bufSetPixel);
  */
  oled = new gfxSurface(320, 132);
  oledBg = new gfxSurface(320, 132);
  oledBgBlr = new gfxSurface(320, 132);
  oledIcon = new gfxSurface(118,118);
  testScroll = new guiScrollbar(30, 40, 11);
  testBtn = new guiButton(1, 11, 11);
  WIN32_FIND_DATA fd;
  HANDLE hFind;
  std::wstring ws;
  hFind = FindFirstFile(L"img/bg/*.bin", &fd);
  if (hFind != INVALID_HANDLE_VALUE) {
    do {
          // read all (real) files in current folder
          // , delete '!' read other 2 default folder . and ..
          if (!(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
            ws.assign(fd.cFileName);
            oBgL.push_back(std::string(ws.begin(),ws.end()));
          }
    } while (FindNextFile(hFind, &fd));
    FindClose(hFind); 
    oiBg=0;
    //loadBg();
  }
  //  bgd.open("/img/bg");
  //while (bgf.openNext(&bgd, O_RDONLY)) {
  //  if (bgf.isDir() || bgf.isHidden() || false) {
  //        bgf.close();
  //        continue;
  //  }
  //  memset(cbgfn, 0, 255);
  //  bgf.getName(cbgfn, 255);
  //  bgfn.assign(cbgfn);
  //  bgl.push_back(bgfn);
  //  Serial.println(bgfn.c_str());
  //  bgf.close();
  //}


  // Initialize GDI+.
  GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);
  RedirectIOToConsole();  
  wndClass.style = CS_HREDRAW | CS_VREDRAW;
  wndClass.lpfnWndProc = WndProc;
  wndClass.cbClsExtra = 0;
  wndClass.cbWndExtra = 0;
  wndClass.hInstance = hInstance;
  wndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
  wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
  wndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
  wndClass.lpszMenuName = NULL;
  wndClass.lpszClassName = TEXT("TundraEmu.OLED");
  //wndClass.hbrBackground = CreateSolidBrush(RGB(0x44,0x44,0x44));
  RegisterClass(&wndClass);

  hWnd = CreateWindow(TEXT("TundraEmu.OLED"),    // window class name
                      TEXT("TundraEmu (OLED)"),  // window caption
                      WS_OVERLAPPEDWINDOW,       // window style
                      CW_USEDEFAULT,             // initial x position
                      CW_USEDEFAULT,             // initial y position
                      (OLED_WIDTH * oiScale) + (2*(oiScale-1)) + oiScale + 24,
                      (OLED_HEIGHT * oiScale) + (2 * (oiScale - 1)) + oiScale + 52, //58
                      NULL,                      // parent window handle
                      NULL,                      // window menu handle
                      hInstance,                 // program instance handle
                      NULL);                     // creation parameters

  //gdiBuf = CreateBitmap(320,132,1,32,NULL);
  GetWindowRect(hWnd, &wndRect);
  wndSize = {wndRect.right - wndRect.left, wndRect.bottom - wndRect.top};
  ShowWindow(hWnd, iCmdShow);
  UpdateWindow(hWnd);


  while (GetMessage(&msg, NULL, 0, 0)) {
    TranslateMessage(&msg);
    DispatchMessage(&msg);
    InvalidateRect(hWnd, NULL, NULL);
  }

  GdiplusShutdown(gdiplusToken);
  return msg.wParam;
}  // WinMain

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam,
                         LPARAM lParam) {

    switch (message) {
        case WM_PAINT:
            hdc = BeginPaint(hWnd, &ps);
            OnPaint(hdc, hWnd);
            EndPaint(hWnd, &ps);
            return 0;
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
        case WM_KEYDOWN:
            if (wParam == VK_F11) {
                std::cout << "F11" << std::endl;
            } else if (wParam == 0x53) {
                if (oiScale < 6) {
                    oiScale++;
                } else if (oiScale==6)  {
                    oiScale=1;
                }
                std::cout << "Scale" << std::endl;
                //GetWindowRect(hWnd, &wndRect);
                //wndSize = {wndRect.right - wndRect.left,
                //           wndRect.bottom - wndRect.top};
                //
                MoveWindow(hWnd, wndRect.left, wndRect.top,
                    (OLED_WIDTH * oiScale) + (2 * (oiScale - 1)) + oiScale + 24,
                    (OLED_HEIGHT * oiScale) + (2 * (oiScale - 1)) + oiScale + 52, //58
                    1);  // initial y size
                GetWindowRect(hWnd, &wndRect);
                wndSize = {wndRect.right - wndRect.left,
                           wndRect.bottom - wndRect.top};
            } else if (wParam == VK_DOWN) {
                if (oiBg < oBgL.size() - 1) {
                  oiBg++;
                  //loadBg();
                  std::cout << "Up" << std::endl;
                  InvalidateRect(hWnd, NULL, FALSE);
                }
            } else if (wParam == VK_UP) {
                if (oiBg > 0) {
                  oiBg--;
                  //loadBg();
                  std::cout << "Down" << std::endl;
                  InvalidateRect(hWnd, NULL, FALSE);
                }
            }
            return 0;
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
    }
}  // WndProc

void loadBg() {
    sprintf_s(fNameOledBg, "img/bg/%s", oBgL.at(oiBg).c_str());
    sprintf_s(fNameOledBgBlr, "img/bg/blur/%s", oBgL.at(oiBg).c_str());
    printf("loadBg: %s\n", fNameOledBg);
    std::ifstream inb(fNameOledBg, std::ios::binary);
    std::ifstream inbl(fNameOledBgBlr, std::ios::binary);
    inb.read(reinterpret_cast<char*>(oledBg->buf), sizeof(uint8_t) * oledBg->bufSize);
    inbl.read(reinterpret_cast<char*>(oledBgBlr->buf), sizeof(uint8_t) * oledBgBlr->bufSize);
    // loadImg(oBg, fNameOledBg);
    // loadImg(oBgB, fNameOledBgBlr);
}

Gdiplus::Color gr42col(uint8_t c) {
    BYTE col = c | c << 4;
    return RGB(col, col, col);
}

void RedirectIOToConsole() {
    // Create a console for this application
    AllocConsole();

    // Get STDOUT handle
    HANDLE ConsoleOutput = GetStdHandle(STD_OUTPUT_HANDLE);
    int SystemOutput = _open_osfhandle(intptr_t(ConsoleOutput), _O_TEXT);
    FILE* COutputHandle = _fdopen(SystemOutput, "w");

    // Get STDERR handle
    HANDLE ConsoleError = GetStdHandle(STD_ERROR_HANDLE);
    int SystemError = _open_osfhandle(intptr_t(ConsoleError), _O_TEXT);
    FILE* CErrorHandle = _fdopen(SystemError, "w");

    // Get STDIN handle
    HANDLE ConsoleInput = GetStdHandle(STD_INPUT_HANDLE);
    int SystemInput = _open_osfhandle(intptr_t(ConsoleInput), _O_TEXT);
    FILE* CInputHandle = _fdopen(SystemInput, "r");

    // make cout, wcout, cin, wcin, wcerr, cerr, wclog and clog point to console
    // as well
    std::ios::sync_with_stdio(true);

    // Redirect the CRT standard input, output, and error handles to the console
    freopen_s(&CInputHandle, "CONIN$", "r", stdin);
    freopen_s(&COutputHandle, "CONOUT$", "w", stdout);
    freopen_s(&CErrorHandle, "CONOUT$", "w", stderr);

    // Clear the error state for each of the C++ standard stream objects. We
    // need to do this, as attempts to access the standard streams before they
    // refer to a valid target will cause the iostream objects to enter an error
    // state. In versions of Visual Studio after 2005, this seems to always
    // occur during startup regardless of whether anything has been read from or
    // written to the console or not.
    std::wcout.clear();
    std::cout.clear();
    std::wcerr.clear();
    std::cerr.clear();
    std::wcin.clear();
    std::cin.clear();
}

void render() {
    //drawText(oledBg, 0, 0 * TALLEST_CHAR_PIXELS, "!\"% &");
    //drawText(oledBg, 1, 0 * TALLEST_CHAR_PIXELS);
    // Draw Background
    // drawWnd;
    //memcpy(gdiBuf, oled.buf, oled.bufSize);
    //oledBg.loadImg("");
    // 
    sprintf_s(fNameOledBg, "img/bg/%s", oBgL.at(oiBg).c_str());
    oledBg->loadImg(fNameOledBg);
    //sprintf_s(fNameOledBgBlr, "img/bg/blur/%s", oBgL.at(oiBg).c_str());
    //oledBg->drawRect(30, 62, 102, 52, 0x6, 1);
    //oledBg->loadImg(fNameOledBgBlr, 30, 62, 100, 50);
    oledBg->drawWnd(20, 20, 160, 102, "Backgrounds");
    oledBg->drawSunkenBox(24,34,152,84);
    oledBg->drawRectFill(24,34, 152, 84, 0xf, 0);
    guiScrollbar scrollBar(152, 84);
    scrollBar.draw(0,10);
    scrollBar.blit(oledBg,165,34);
    //testScroll->draw(1,10,true);
    //testScroll->blit(oledBg, 152, 34);
    memcpy(oled->buf, oledBg->buf, oled->bufSize);

    //const char* line;
    //uint32_t lineHeight = 10;
    //uint32_t yl;
    //float viewportHeight = (h / (2+lineHeight))-1;
    //float scrollInitialHeight = 9 * (viewportHeight - 1);
    //float percVisible = (100*viewportHeight) / oBgL.size();
    //uint16_t ScrollbarTop = y + 12 + 5 + 9 + 4;
    //float thumbHeight = ((scrollInitialHeight * percVisible) / 100) + 2;
    //float maxScrollBarY = (y + h - 12 - 2) - thumbHeight - 3;

    ////memcpy(oBuf, oBg, OLED_BUF_SIZE);
    //memset(oBuf, 0xFE, OLED_BUF_SIZE);
    //drawWnd(oBuf, x, y, w, h);
    //// Draw List Fill
    //drawRectFill(oBuf, x + 3, y + 12 + 3, w - 6, h - 12 - 6, 0xf, 0);
    //drawRectFill(oBuf, x + w - 12 - 4, y + 12 + 12 + 4, 14, h - (12 * 3) - 8, 0xD, 0);
    ////test.draw(0, thumbHeight);
    //drawRaisedBoxFill(oBuf, 1, 1, 9, 9);
    ////test.draw();
    ////test.blit(oBuf,0,0);
    //// Scroll
    //setPixel(oBuf, 1, 1, 0x2, 0);
    //setPixel(oBuf, 1, 132, 0x2, 0);
    //setPixel(oBuf, 320, 1, 0x2, 0);
    //setPixel(oBuf, 320, 132, 0x2, 0);
    ////drawRaisedBoxFill(oBuf, 0, 0, 9, 9);
    ///*
    //drawRaisedBoxFill(oBuf, x + w - 12 - 2, y + h - 12 - 2, 9, 9);
    ////drawFillTriangle(oBuf, x + w - 12 - 2 + 6 - 2, y + 12 + 5 + 5 - 2,
    ////                 x + w - 12 - 2 + 4 - 2, y + 12 + 5 + 7 - 2,
    ////                 x + w - 12 - 2 + 8 - 2, y + 12 + 5 + 7 - 2, 0x0, 0);
    //drawFillTriangle(oBuf, x + w - 10, y + 12 + 9, 9/2, 9/3, 0, 0x0, 0);

    //drawFillTriangle(oBuf, x + w - 12 - 2 + 6 - 2, y + h - 12 - 2 + 7 - 2,
    //                 x + w - 12 - 2 + 4 - 2, y + h - 12 - 2 + 5 - 2,
    //                 x + w - 12 - 2 + 8 - 2, y + h - 12 - 2 + 5 - 2, 0x0, 0);
    //float thumb_pos;
    //if (viewportHeight >= oBgL.size()) {
    //    thumbHeight = scrollInitialHeight;
    //    thumb_pos = ScrollbarTop;
    //}
    //else {
    //    float vsel = oiBg;
    //    float vmin = 0;
    //    float vmax = oBgL.size() - 1;
    //    float smax = maxScrollBarY;
    //    if ((int)oBgL.size() % 2)
    //        smax -= 1;
    //    float smin = ScrollbarTop;
    //    thumb_pos = (vsel - vmin) / (vmax - vmin) * (smax - smin) + smin;
    //}
    //drawRaisedBoxFill(oBuf, x + w - 12 - 2, thumb_pos, 9, thumbHeight);
    //*/

    //// Title Bar
    //ofr.setCursor(x + 1, y + 1);
    //ofr.setFontSize(10);
    //ofr.setBackgroundColor(0x6);
    //ofr.setBackgroundFillMethod(BgFillMethod::None);
    //ofr.setFontColor(0xf);
    //ofr.printf("Backgrounds");

    //if (oiBg < yOffs) {
    //    yOffs--;
    //} else if (oiBg == yOffs+viewportHeight) {
    //    yOffs++;
    //}
    //for (uint16_t i = 0; i < viewportHeight; ++i) {
    //  if (yOffs + i > oBgL.size()-1)
    //      break;
    //  line = oBgL.at(yOffs + i).c_str();
    //  yl = y + 12 + 5 + ((2 + lineHeight) * i);
    //  if (yl >= x + h - lineHeight) {
    //    break;
    //  }
    //  ofr.setFontColor(0x0);
    //  ofr.setCursor(x + 4, yl-1);
    //  ofr.setBackgroundColor(0xF);
    //  if (oiBg == yOffs + i) {
    //    ofr.setBackgroundColor(0x9);
    //    drawRectFill(oBuf, x + 1, yl - 2, w - 12 - 5, lineHeight + 2, 0x9, 0);
    //  }
    //  ofr.printf("%s", line);
    //}
    ////for (int o = 0; o < OLED_BUF_SIZE; ++o) {
    ////  if (oUI[o]) {
    ////    oBuf[o] |= oUI[o];
    ////  }
    ////}
    ////OLED_ShowPicture_gray(0, 0, 320, 132, oBuf);
    //drawSunkenBox(oBuf, x + 3, y + 12 + 3, w - 6, h - 12 - 6);
    //memcpy(gdiBuf, oBuf, OLED_BUF_SIZE);
};
