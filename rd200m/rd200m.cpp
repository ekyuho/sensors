// A library for FTLab's RD200M Radon Sensor
// 2017-02-12 Kyuho KIm <ekyuho@gmail.com>
//

#include "Arduino.h"
#include <SoftwareSerial.h>
#include "Rd300m.h"


Rd300m::Rd300m(SoftwareSerial* s) {
  radon = s;
  radon->begin(19200);
  _stat = "";
  _debug = false;
  _ready = false;
}

void Rd300m::request(void) {
  radon->write(req, 4);    
  _stat = "";
  _ready = false;
  LOG("request radon\n");
}

void Rd300m::reset(void) {
  radon->write(_reset, 4);    
  _stat = "";
  _ready = false;
  LOG("reset radon\n");
}

void Rd300m::update(void) {
  while (radon->available() > 0) {
    byte c = radon->read();
    LOG("("+ String(state)+")");
    if (!(c&0xf0)) LOG("0");
    LOG(c, HEX);
    LOG(" ");

    switch (state) {
      case 0: if (c == 0x02) { state = 1; csum=0; } break;
      case 1: if (c == 0x10) { state = 2; csum += c; } else state = 0; break;
      case 2: if (c == 0x04) { state = 3; csum += c; } else state = 0; break;
      case 3: state = 4; csum += c; 
	if (c == 0xE0) _stat += "Shocked; ";
        _up = 0x3&c;
	if (_up < 2) _stat += upmsg[_up];
	break;
      case 4: 
	_elapsed = c; csum += c; state = 5;
	if (_stat) _stat += " ("+ String(_elapsed) +" min up)";
	break;
      case 5: _value = c*100; state = 6; csum += c; break;
      case 6: _value += c; state = 7; csum += c; break;
      case 7: csum += c; 
	if (0xff != csum) { 
          LOG(" Checksum error\n"); 
	  LOG(csum, HEX); LOG("\n");
	  if (_elapsed < 2) _stat = upmsg[_elapsed];
	  _stat += "checksum error; ";
	}  else
	  LOG(" Good value\n");
	_ready = true;
	state = 0; 
	if (_onPacket) _onPacket();
        break;
    }
  }
}

int Rd300m::value(void) {
  return(_value);
}

String Rd300m::uptime(void) {
  return(upmsg[_up]);
}

int Rd300m::elapsed(void) {
  return(_elapsed);
}

String Rd300m::status(void) {
  return(_stat);
}

void Rd300m::debug(void) {
  _debug = true;
  LOG("Debug ON\n");
}

void Rd300m::LOG(String s) {
  if (_debug) Serial.print(s);
}

void Rd300m::LOG(byte a, int b) {
  if (_debug) Serial.print(a, b);
}

void Rd300m::onPacket(void (*function)(void) ) { _onPacket = function; }
void Rd300m::stop(void) { _onPacket = NULL; }

bool Rd300m::ready(void) { return(_ready); }
