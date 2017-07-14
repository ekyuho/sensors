
#include <SoftwareSerial.h>

SoftwareSerial swSer(D4, 12, false, 256);

void setup() {
  Serial.begin(115200);
  swSer.begin(9600);

  Serial.println("\nSoftware serial test started");
}

int stat = 1;
int cnt = 0;
char buf[10];

void loop() {
  while (swSer.available() > 0) {
    char c;
    c = swSer.read();
    //Serial.print("stat="+ String(stat) +", "+ "cnt="+ String(cnt) +" ");
    //Serial.print(c, HEX);
    //Serial.println(" ");

    if (stat == 1) {
       if (c == 0xAA) stat = 2;
    } else
    if (stat == 2) {
       if (c == 0xC0) stat =3;
       else stat = 1;
    } else
    if (stat == 3) {
       buf[cnt++] = c;
       if (cnt == 7) stat = 4;
    } else
    if (stat == 4) {
       if (c == 0xAB) {
          //check checusum
          stat = 1;
       }
       else {
          Serial.println("Eh? wrong tailer");
       }
       cnt = 0;
       int pm25 = buf[0] + 256*buf[1];
       int pm10 = buf[2] + 256*buf[3];
       Serial.println("PM2.5="+ String(pm25) +", PM10="+ String(pm10));
    }
  }
}
