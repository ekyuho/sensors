#ifndef Sds011_h
#define Sds011_h

#include "Arduino.h"

class Sds011 
{
  public:
    Sds011(void);
    bool append(uint8_t c);
    int get(int[]);
    void debug(int);
    void onPacket(void (*function)(int, int));
  private:
    void bubbleSort(int[], int);
    byte _buffer[26];
    byte b1[3] = { 0xAA, 0xC0, 0xAB };
    int bi;
    int _len;
    int pm25b[100], pm10b[100];
    int bnum;
    int _debug;
    void (*_onPacket)(int, int);
};
#endif
