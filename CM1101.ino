#include "Timer.h"
Timer t;
#include <SoftwareSerial.h>
SoftwareSerial cSerial(5, 6); // RX, TX
int Co2 = 0;

void readCo2() {
  char b[] = { 0x11, 0x01, 0x01, 0xED };
  byte r[40];
  int c;
  cSerial.write(b, 4);

  if ((c = cSerial.readBytes(r, 8)) < 8) {
    Serial.println("co2 error "+ String(c));
    return;
  }
  
  Co2 = (r[3] * 256) + r[4];
  Serial.println(Co2);
}

void setup() {
  Serial.begin(9600);
  cSerial.begin(9600);
  t.every(4000, readCo2);
}

void loop() {
  t.update();
}

