/*
 * USB HID Example, with serial console (not using Serial object, cause this is too large
 * Peter Brier
 */

#include "mbed.h"
#include "USBHID.h"
#include "serial_api.h"
#include "pinmap.h"


// externals 
extern int stdio_uart_inited;
extern serial_t stdio_uart;

//We declare a USBHID device
USBHID hid(
 64,  // out 
 64,  // in
 0x1234, // vid
 0x0006, // pid
 0x0001, // p_release
 false // connect
);

//This report will contain data to be sent
HID_REPORT send_report, recv_report;
PwmOut servo1(P0_18);
PwmOut servo2(P0_19);
Ticker tic;

// fanbot IO
BusOut leds(P1_19, P1_25, P0_8, P0_9, P0_22, P0_13, P0_14);
/* DigitalOut led1(P1_19);
DigitalOut led2(P1_25);
DigitalOut led3(P0_8);
DigitalOut led4(P0_9);
DigitalOut led5(P0_22);
DigitalOut led6(P0_13);
DigitalOut led7(P0_14);
*/
DigitalIn  button(P0_1);
DigitalIn  uart_rx(P1_26); // rx pin as input, so we can disable the pull-up before enumeration



volatile int read_res=0, send_res=0, counter=0;

void tic_handler() {
    send_report.data[2] = counter++;
     send_res = hid.send(&send_report);	
}


#define IAP_LOCATION 0x1fff1ff1
typedef void (*IAP)(unsigned long [],unsigned long[]);
IAP iap_entry=(IAP) IAP_LOCATION;

void iap(void )
{
  unsigned long command[5];
  unsigned long result[4];
  command[0] = 57; // re-invoke ISP
  iap_entry (command, result);
};


// Software UART send. mark is LOW space is HIGH
#define BAUD 9600
#define t_bit (1000000/BAUD) // 9600 bps, 10 microseconds bit time
void send(DigitalOut &p, char c)
{
  p = 1; // 1 start bit
  wait_us(t_bit);
  for(int bit=0; bit<8; bit++) // 8 bits
  { 
    p = ( ( (unsigned char) c & (1<<bit) ) ? 0 : 1 );
    wait_us(t_bit);
  }
  p = 0; // 1 stop bit
  wait_us(t_bit);
}

void tx(DigitalOut &p, char *s)
{
  while ( *s ) send(p, *s++);
}

void do_serial()
{
  volatile static int i;
  DigitalOut usb_conn(P0_6); // we control the USB connect output now

  serial_init( &stdio_uart, P1_27, P1_26); // pins: tx, rx
  pin_mode( P1_26, PullNone); // disable weak pullup for RX
  // pin_mode( P0_6, PullNone); // disable weak pullup for USB_CON
  
  printf("Start!\n");
  while( 1 )
  {
    if ( serial_readable( &stdio_uart ) )
    {
	  char c= '0';
	  while ( serial_readable( &stdio_uart ) )
	    c = serial_getc(&stdio_uart); 
		i = c - '0' - 1;
	  // if ( i > 0 || i < 0 ) i = 0;
	  if ( c == 'h' )  
	    tx(usb_conn, "Hello world!\n");
	  leds = 1<<i;
	  i++;   
	}
	//wait(0.01);
  }
}


int main(void) {
	button.mode(PullUp);
	uart_rx.mode(PullNone); // disable pull-up, so we can enumerate 
	
	for(int l=0, p=1; l<7;l++, p|= (1<<l))
	{
	  wait(0.1);
	  leds = p;
	  wait(0.1);
	}
	//wait(3);
	hid.connect(false); // do not block
 	
	servo1.period_ms(15);
	servo1.pulsewidth_us(700);
	
	//Fill the report
    for(int i = 0; i < 64; i++)
        send_report.data[i] = i;
    send_report.length = 64;

	for(int i=0; i<7 && !hid.configured(); i++)
	{
	  wait(0.1);
	  leds = 1<<i;
	  wait(0.1);
	}	 
    if ( !hid.configured() )
	  do_serial();
    tic.attach(tic_handler, 0.1);
	
    while (1)
	{
	  if ( button == 0 ) // button pressed == LOW 
	  {
	    iap();
	    servo1.pulsewidth_us(700);
	    servo2.pulsewidth_us(700);
		leds=255;
		wait(0.1);
		leds = 0;
	    servo1.pulsewidth_us(700 + 255);
	    servo2.pulsewidth_us(700 + 255);
		wait(0.1);
      }

      if (hid.readNB(&recv_report)) 
	  {
	    read_res++;
        leds = recv_report.data[0];
		servo1.pulsewidth_us(700 + recv_report.data[1]);
		servo2.pulsewidth_us(700 + recv_report.data[2]);
		send_report.data[1] = button;		
      }
	 // printf("send: %d receive: %d counter: %d\n", send_res, read_res, counter);
	  wait_ms(1);
	}
}
