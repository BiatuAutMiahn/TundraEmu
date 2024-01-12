#ifndef STREAM_H
#define STREAM_H

#include "Print.h"

#if defined(AVR)
size_t  snprintf(char *buffer, const size_t size, const char *fmt, ...);
size_t  printf(const char *fmt, ...);
#endif

class Stream : public Print {
  public:
  void begin(int baud) {};
  // void print(const char c) {
  //   write(c);
  // };
  void print(char c) {
    write(c);
  };
  void print(const int i) {
    char buffer[100];
    snprintf(buffer, sizeof(buffer), "%d", i);
    print(buffer);
  };
  void print(const unsigned int u) {
    char buffer[100];
    snprintf(buffer, sizeof(buffer), "%u", u);
    print(buffer);
  };
  void print(float flt) {
    char buffer[100];
    snprintf(buffer, sizeof(buffer), "%f", flt);
    // print(buffer);
  };
  void print(double dbl) {
    char buffer[100];
    snprintf(buffer, sizeof(buffer), "%lf", dbl);
    // print(buffer);
  };
  void print(char const *s) {
    for (; *s; ++s) {
      write(*s);
    }
  };
  void println(const unsigned int u) {
    print(u);
    print("\r\n");
  }
  void println(const char *s) {
    print(s);
    print("\r\n");
  }
  virtual void flush() {};
  int read() { return -1; };
};

class MockSerial : public Stream {
  public:
    virtual size_t write(uint8_t c) {
      printf("%c", c); // Ultimately writes are sent here to print to stdout
      return 1;
    }
    virtual size_t write(const uint8_t *buffer, size_t size) {
      for (size_t i = 0; i<size; ++i) {
        write(buffer[i]);
      }
      return size;
    }
    uint8_t dtr() { return 1; }
    size_t available() { return 0; }
    void end() {}
};

typedef MockSerial HardwareSerial;

extern MockSerial Serial;
extern MockSerial Serial1;

#endif
