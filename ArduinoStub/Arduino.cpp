#include "Arduino.h"
#include <assert.h>

MockSerial Serial;
MockSerial Serial1;

void delay(uint16_t msec) {};
void delayMicroseconds(uint32_t usec) {};
unsigned long millis() { return 0; };
uint32_t micros() { return 0; };
void interrupts() {};
void noInterrupts() {};

int pins[ARDUINO_STUB_MAX_PINS];
int pinModes[ARDUINO_STUB_MAX_PINS];
void pinMode(uint8_t pin, uint8_t mode) {
  pinModes[pin] = mode;
};
static digitalWriteHookFn digitalWriteHook;
void digitalWrite(uint8_t pin, uint8_t val) {
  // printf("digitalWrite: %d, %d\n", (int)pin, (int)val);
  assert(pin<ARDUINO_STUB_MAX_PINS);
  pins[pin] = val;
  if (digitalWriteHook!=NULL) {
    digitalWriteHook(pin, val);
  }
};
void hookDigitalWrite(digitalWriteHookFn fn) {
  digitalWriteHook = fn;
}
bool digitalRead(uint8_t pin) {
  assert(pin<ARDUINO_STUB_MAX_PINS);
  return pins[pin];
};
void mockDigitalRead(uint8_t pin, bool value) {
  assert(pin<ARDUINO_STUB_MAX_PINS);
  pins[pin] = value ? HIGH : LOW;
}
int analogRead(uint8_t pin) {
  assert(pin < ARDUINO_STUB_MAX_PINS);
  return pins[pin];
}
int analogWrite(uint8_t pin) {
  assert(pin < ARDUINO_STUB_MAX_PINS);
  return pins[pin];
}
void mockAnalogWrite(uint8_t pin, int value) {
  assert(pin < ARDUINO_STUB_MAX_PINS);
  pins[pin] = value;
}
void mockAnalogRead(uint8_t pin, int value) {
  assert(pin < ARDUINO_STUB_MAX_PINS);
  pins[pin] = value;
}
void analogReadResolution(int) {}

long nativeRandom(long max) {
  return max / 2;
}

#if defined(AVR)
size_t printf(const char *fmt, ...) {
  return 0;
}

size_t  snprintf(char *buffer, const size_t size, const char *fmt, ...) {
  buffer[0] = 0;
  return 0;
}
#endif

size_t Print::print(const char value[]) {
  return printf("%s", value);
}
size_t Print::print(char value) {
  return printf("%c", value);
}
size_t Print::print(unsigned char value, int base) {
  return printf(base==HEX ? "%x" : "%u", (unsigned)value);;
}
size_t Print::print(int value, int base) {
  return printf(base==HEX ? "%x" : "%d", (int)value);;
}
size_t Print::print(unsigned int value, int base) {
  return printf(base==HEX ? "%x" : "%u", (unsigned)value);;
}
size_t Print::print(long value, int base) {
  return printf(base==HEX ? "%x" : "%d", (int)value);;
}
size_t Print::print(unsigned long value, int base) {
  return printf(base==HEX ? "%lx" : "%lu", value);;
}
size_t Print::print(double value, int base) {
  return printf("%lf", value);
}

void wdt_reset() {}
