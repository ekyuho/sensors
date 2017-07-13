#include "Arduino.h"
#include "Sds011.h"

//http://breathe.indiaspend.org/wp-content/uploads/2015/11/nova_laser_sensor.pdf

Sds011::Sds011(void) {
  bi = 0;
  bnum = 0;
  _debug = false;
}

void Sds011::debug(int cmd) {  // true or false
  _debug = cmd;
}

bool Sds011::append(byte b) {
  if (bi == 0) {
    if (b == b1[0]) {
      bi++; 
      if (_debug> 99) Serial.print(" s0");
    } else {
      bi = 0;
      Serial.print(" X");
    }
    return false;
  }

  if (bi == 1) {
    if (b == b1[1]) {
      bi++;
      if (_debug > 99) Serial.print(" s1");
    } else {
      bi = 0;
      Serial.print(" Y");
    }
    return false;
  }

  if (bi == 2) {
    if (_debug > 99) Serial.print(" packet begin");
    _buffer[bi++] = b;
  } else
  if (bi == 9) {
    if (b == b1[2]) {
      pm25b[bnum] = _buffer[2] + _buffer[3]*256;
      pm10b[bnum] = _buffer[4] + _buffer[5]*256;
      //if (_debug> 0) Serial.println("got packet [25,10]["+ String(bnum) +"]=["+ String(pm25b[bnum])+","+ String(pm10b[bnum])+"]");
      if (_onPacket) _onPacket(pm25b[bnum], pm10b[bnum]);
    } else { Serial.print("wrong tailer"); Serial.print(b, HEX); }
    bnum++;
    if (bnum > 99) bnum = 0;
    bi = 0;    
    
    return true;
  } else
  {
    if (_debug>199) Serial.print(" ("+String(bi)+")");
    if (_debug>199) Serial.print(b, HEX);
    _buffer[bi++] = b;
    return false;
  }
}

int Sds011::get(int A[]) { 
    int t;

    /*
    if (bnum == 0) {
        A[0] = A[1] = A[2] = -1;
	return(0);
    }
    */

    if (_debug>80) {
	Serial.print("\nBefore sorting");
        Serial.print("\nPM2.5: "); for (int i=0; i<bnum; i++) Serial.print(String(pm25b[i]) + String(" "));
        Serial.print("\nPM10: "); for (int i=0; i<bnum; i++) Serial.print(String(pm10b[i]) + String(" "));
    }

    bubbleSort(pm25b, bnum);
    bubbleSort(pm10b, bnum);
    
    if (_debug>80) {
	Serial.print("\nSorted");
        Serial.print("\nPM2.5: "); for (int i=0; i<bnum; i++) Serial.print(String(pm25b[i]) + String(" "));
        Serial.print("\nPM10: "); for (int i=0; i<bnum; i++) Serial.print(String(pm10b[i]) + String(" "));
    }

    A[0] = pm25b[int(bnum/2)];
    A[1] = pm10b[int(bnum/2)];

    t = bnum;
    bnum = 0;
    return(t); 
}

void Sds011::bubbleSort(int A[], int len) {
  int newn;
  int n=len;
  int temp=0;
  do {
    newn=1;
    for(int p=1;p<len;p++){
      if(A[p-1]>A[p]){
        temp=A[p];           //swap places in array
        A[p]=A[p-1];
        A[p-1]=temp;
        newn=p;
      } //end if
    } //end for
    n=newn;
  } while(n>1);
}

void Sds011::onPacket(void (*function)(int, int) ) { _onPacket = function; }
