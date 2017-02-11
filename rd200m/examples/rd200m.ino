#include "Timer.h"
Timer t;

#include <SoftwareSerial.h>
SoftwareSerial r(D3, D4);  // Rx,  Tx

#include <rd300m.h>
Rd300m radon(&r);

void getit() {
  static bool first = true;
  if (first) { first = false; return; }
  
  Serial.println(String(radon.value()) + " pCi");
  String s = radon.status();
  if (s != "") Serial.println("S:"+s);
}

void doit() {
  radon.request();
}

void setup() {
  Serial.begin(115200);
  radon.debug();
  radon.onPacket(getit);
  t.every(3000, doit);
}

void loop() {   
  t.update(); 
  radon.update();
}

