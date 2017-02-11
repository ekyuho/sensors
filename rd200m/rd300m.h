// A library for FTLab's RD200M Radon Sensor
// 2017-02-12 Kyuho KIm <ekyuho@gmail.com>
//
#ifndef Pms3003_h
#define Pms3003_h

#include "Arduino.h"

class Rd300m 
{
  public:
    Rd300m(SoftwareSerial*);
    int value(void);
    String uptime(void);
    int elapsed(void);
    String status(void);
    void update(void);
    void request(void);
    void reset(void);
    void debug(void);
    void LOG(String);
    void LOG(byte, int);
    void onPacket(void (*function)(void));
    void stop(void);
    bool ready(void);
  private:
    String upmsg[5] = {"less than  3 min", "less than 1 hour", "more than 1 hour" };
    byte req[4] = { 0x02, 0x01, 0x00, 0xFE };
    byte _reset[4] = { 0x02, 0xA0, 0x00, 0xFF-0xA0 };
    SoftwareSerial* radon;
    int _value;
    int _elapsed;
    int _up;
    int csum;
    int state;
    String _stat;
    bool _debug;
    bool _ready;
    void (*_onPacket)(void);
};
#endif
