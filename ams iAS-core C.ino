#include "Timer.h"
Timer t;

#include <Wire.h>
// https://github.com/sandeepmistry/esp8266-Arduino/blob/master/esp8266com/esp8266/libraries/ESP8266WiFi/examples/WiFiClient/WiFiClient.ino


int Seq = 0;

void checkco2voc() {
  byte r[10];
  byte c1[] = { 0x04, 0x13, 0x8a, 0x00, 0x01 };
  byte c2[] = { 0x04, 0x13, 0x8b, 0x00, 0x01 };
  int i=0;

  Wire.beginTransmission(21);
  for (i=0; i<5; i++) Wire.write(c1[i]);
  Wire.endTransmission();
  Wire.requestFrom(21, 4);    // request 9 bytes

  while (Wire.available()) { // slave may send less than requested
    r[i] = Wire.read(); // receive a byte as character
    Serial.print(r[i], HEX);
    Serial.print(" ");
    i++;
  }
  if (r[2] == 0x8) Serial.println("warming up");
  else Serial.println("stat ok");

  Wire.beginTransmission(21);
  for (int i=0; i<5; i++) Wire.write(c2[i]);
  Wire.endTransmission();
  Wire.requestFrom(21, 4);    // request 9 bytes

  i = 0;
  while (Wire.available()) { // slave may send less than requested
    r[i] = Wire.read(); // receive a byte as character
    Serial.print(r[i], HEX);
    Serial.print(" ");
    i++;
  }
  int ppm = r[2] * 256 + r[3];
  Serial.println("CO2 = "+ String(ppm) + " ppm");


}

void setup() {
  Wire.begin();  //sda, scl
  //Wire.begin();
  Serial.begin(9600);  // start serial for output
  t.every(5000, checkco2voc);
}

void loop() {
  t.update();
}
