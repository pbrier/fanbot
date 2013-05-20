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

// IR led and receiver
DigitalIn ir_rx(P1_16);
DigitalOut ir_tx(P1_13);

//PwmOut ir_pwm(P0_21);
PwmOut ir_pwm(P1_13);


#define max 0.0003

void ir_test()
{
  ir_pwm.period(1.0/38000.0);
  ir_pwm.pulsewidth(1.0/72000.0);
  while ( button == 0 )
  {
	led1 = ir_rx;
	/* ir_tx = 1;
	wait_us(10);
	ir_tx = 0;
	wait_us(10); */
  }
}

int main() {
	float t = 0, d =1,tw=0.2;
	unsigned char i;
	button.mode(PullUp);
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
	  
	  if ( button == 0 ) ir_test();
	  
	}
	
}
