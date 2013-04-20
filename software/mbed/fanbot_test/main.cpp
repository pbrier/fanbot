#include "mbed.h"
#include "USBHID.h"


DigitalOut led1(P1_19);
DigitalOut led2(P1_25);
DigitalOut led3(P0_8);
DigitalOut led4(P0_9);
DigitalOut led5(P0_22);
DigitalOut led6(P0_13);
DigitalOut led7(P0_14);
DigitalOut servo1(P0_18);
DigitalOut servo2(P0_18);
DigitalIn  button(P0_1);

#define max 0.0003


int main() {
	float t = 0, d =1,tw=0.2;
	unsigned char i;
    while(1)
    {	
	  t = t + 0.00003*d;
	  servo1 = 1;
   	  wait(0.0005+t);
	  servo1 = 0;
      wait(tw);
	  if ( t > max ) { wait(0.5); t = max; d = -1; i = (i ? i<<1 : 1); }
	  if ( t < 0 ) { t = 0; d = 1; };
	  if ( tw > 0.030 ) tw = tw  * 0.95;
	  led1 = (i & 1 ? 0 :1);
	  led2 = (i & 2 ? 0 : 1);
	  led3 = (i & 4 ? 0 :1);
	  led4 = (i & 8 ? 0 : 1);
	  led5 = (i & 16 ? 0 : 1);
	  led6 = (i & 32 ? 0 : 1);
	  led7 = (i & 64 ? 0 : 1);
	}
	
}
