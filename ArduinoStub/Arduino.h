#ifndef ARDUINO_H
#define ARDUINO_H

// Mock Arduino.h used when compiling native platform tests.

#include <string.h>
#if defined(AVR)
#include <stdint.h>
#else
#include <string>
#include <cstdint>
#include <cstring>
#include <cstdio>
#endif

#define String std::string

#define isDigit isdigit
#define isAlpha isalpha

#include "Stream.h"

#define F_CPU 100000000

#define F(s) (s)
class __FlashStringHelper;

#define LED_BUILTIN 13

#define ARDUINO_STUB_MAX_PINS 110
#define A0 100
#define A1 (A0+1)
#define A2 (A0+2)
#define A3 (A0+3)
#define A4 (A0+4)
#define A5 (A0+5)

#define LOW   0
#define HIGH  1

#define INPUT   0
#define OUTPUT  1
#define INPUT_PULLDOWN 2 

#define PROGMEM
#define PGM_P const char*
#define strncpy_P strncpy
#define strlen_P strlen
#define pgm_read_byte(p) (*((uint8_t*)(p)))
#define pgm_read_dword(p) (*((uint32_t*)(p)))

typedef unsigned char   boolean;
typedef unsigned char   uint8_t;
#if !defined(AVR)
typedef unsigned short  uint16_t;
typedef unsigned int    uint32_t;
#endif
typedef uint8_t byte;

#ifdef __cplusplus
extern "C" {
#endif
void delay(uint16_t msec);
void delayMicroseconds(uint32_t usec);
unsigned long millis();
uint32_t micros();
void interrupts();
void noInterrupts();

long nativeRandom(long max);

void pinMode(uint8_t pin, uint8_t mode);
void digitalWrite(uint8_t pin, uint8_t val);
bool digitalRead(uint8_t pin);
int analogRead(uint8_t);
void analogWrite(uint8_t pin, uint8_t val);
void analogReadResolution(int);
void mockDigitalRead(uint8_t pin, bool value);
void mockAnalogRead(uint8_t pin, int value);
void mockAnalogWrite(uint8_t pin, int value);
typedef void (*digitalWriteHookFn)(uint8_t pin, uint8_t val);
void hookDigitalWrite(digitalWriteHookFn fn);

#ifdef __cplusplus
}
#endif

template<class T> T constrain(const T value, const T min, const T max) {
  if (value<min) {
    return min;
  }
  else if (value>max) {
    return max;
  }
  else {
    return value;
  }
}

// Watchdog timer reset
void wdt_reset();

#endif // ARDUINO_H
