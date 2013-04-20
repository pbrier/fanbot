/*
 * Software serial example
 * Peter Brier
 */

#include "mbed.h"
#include "USBSerial.h"
 
// externals 
extern int stdio_uart_inited;
extern serial_t stdio_uart;


 
//Virtual serial port over USB
USBSerial usb;
// Serial pc(USBTX, USBRX); // P0_19, P0_18
// Serial uart(P1_27, P1_26);

DigitalOut led(P0_7);
DigitalIn button(P0_1);

int i = 0;

#define BAUD 9600
#define t_bit (1000000/BAUD) // 9600 bps, 10 microseconds bit time

DigitalOut pin(P0_20);

// Software UART send. mark is LOW space is HIGH
void send(char c)
{
  pin = 0; // 1 start bit
  wait_us(t_bit);
  for(int bit=0; bit<8; bit++) // 8 bits
  { 
    pin = ( ( (unsigned char) c & (1<<bit) ) ? 1 : 0 );
    led = ( ( (unsigned char) c & (1<<bit) ) ? 1 : 0 );
    wait_us(t_bit);
  }
  pin = 1; // 1 stop bit
  wait_us(t_bit);
}

int main(void) {

    serial_init(&stdio_uart, P1_27, P1_26); // default is 9600bps
 
   // uart.baud(BAUD);
   // serial.printf("I am a virtual serial port\r\n");
  while(1)
  { 
    while ( serial_readable( &stdio_uart ) ) 
    {
      usb.putc( serial_getc(&stdio_uart) );  
    } 
	while ( usb.available () )
     {	 
      send(usb.getc());
	 }
  }
}

