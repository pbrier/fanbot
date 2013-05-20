/*

Copyright (c) 2013 Peter Brier

This file is part of the FanBot project.

Fanbot is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.

*/

#include "mbed.h"
#include "USBHID.h"
#include "serial_api.h"
#include "pwmout_api.h"
#include "pinmap.h"
#include "IAP.h"

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
BusOut leds(P1_19, P1_25, P0_8, P0_9, P0_22, P0_13, P0_14); // Leds
DigitalIn  button(P0_1);   // Button
DigitalIn  uart_rx(P1_26); // rx pin as input, so we can disable the pull-up before enumeration


// IR led and receiver
// 1st 5 protos had IR on P1.16: DigitalIn ir_rx(P1_16);
DigitalIn ir_rx(P1_14);
DigitalOut ir_tx(P1_13);
PwmOut ir_pwm(P1_13);


// IAP and EEPROM defines
#define     MEM_SIZE        256
#define     TARGET_ADDRESS   64
IAP iap;

#define FAN_STEPS 20 // nr of program steps
unsigned short int program[128]; // buffer for EEPROM, 256 bytes, need to be word aligned

// Globals
volatile int read_res=0, send_res=0, counter=0;
int serial_nr = 0;

// USB send Tick handler, send a report (every msec)
// The report format is:
// 0: 0
// 1: status code
// 2: Counter
// 3: Button state
// 4: Servo 1 position
// 5: Servo 2 position
// 6: Led state
// 7: n.a.
// 8..11: Serial #
void tic_handler() 
{
  send_report.data[2] = counter++;
  memcpy(&send_report.data[8], &serial_nr, sizeof(serial_nr) );
  send_res = hid.send(&send_report);	
}

// Software UART send. mark is LOW space is HIGH
#define BAUD 9600
#define t_bit (1000000/BAUD) // 9600 bps, ~10 microseconds bit time
void send(DigitalOut &p, const char c)
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

// Transmit a string in serial mode
void tx(DigitalOut &p, const char *s)
{
  wait_us(2000);
  while ( *s ) 
  {
    send(p, *s++);
    wait_us(1000);
  }
}

// send a single nibble
void tx_nibble(DigitalOut &p, const int n)
{
  wait_us(2000);
  send(p, "0123456789ABCDEF"[n & 0xF] );
  wait_us(2000);
}

// Play the sequence
void play()
{
  iap.read_eeprom( (char*)TARGET_ADDRESS, (char *)program, MEM_SIZE );
  for(int i=0; i<FAN_STEPS; i++)
  {
    leds = program[i] & 0xFF;
    servo1.pulsewidth_us(700 + (program[i]>>8 & 0xFF) );
    wait(0.2);
  }
}

// Set servo position
void set_servo(char n, int val)
{
  if ( n == 'A' ) servo1.pulsewidth_us(700 + 25 * val);
  if ( n == 'B' ) servo2.pulsewidth_us(700 + 25 * val);
}

// get value, wait max 30msec
int get_val()
{
  char c = 0;
  for(int i=0; i<30 && !serial_readable( &stdio_uart );i++ )
    wait(0.001);
  c = serial_getc(&stdio_uart); 
  return (int)(c-'0');
}

/**
*** run serial communication mode
*** Commands:
*** 'p' Play sequence
*** 's' Stop
*** 'L' [val]: select LED value
*** 'A' [val]  arm position [servo 1] (0 is off)
*** 'B' [val]  arm position [servo 2] (0 is off)
*** 'n' Report serial nr (send as 8 digit HEX number)
*** 'r' Read memory
***
**/
void do_serial()
{
  volatile static int i;
  DigitalOut usb_conn(P0_6); // we control the USB connect output now
  pwmout_t _pwm;  // dummy PWM struct to re-init the PWM ports (reselects the pin functions after serial init)
   
  serial_init( &stdio_uart, P1_27, P1_26); // pins: tx, rx
  pin_mode( P1_26, PullNone); // disable weak pullup for RX
  // pin_mode( P0_6, PullNone); // disable weak pullup for USB_CON
  
  pwmout_init(&_pwm, P0_18);
  pwmout_init(&_pwm, P0_19);
  
  while( 1 )
  {
   /* if ( ir_rx == 0 ) // button pressed or IR received
    {
      leds = 1<<i;
      if ( ++i > 6 ) 
        i = 0;
      wait(0.2);
    } */
    
    if ( button == 0 )
      play();
    
    if ( serial_readable( &stdio_uart ) )
    {
      char c = 0;
      while ( serial_readable( &stdio_uart ) )
        c = serial_getc(&stdio_uart); 
      switch( c )
      {
        case 0: 
          break;
        case 'n':  // send serialnr as hex value, values are echoed back: so clean the rx FIFO after each TX
          for(int b=28; b>=0; b-=4) 
            tx_nibble(usb_conn, serial_nr >> b);
          wait(0.01);
          serial_clear(&stdio_uart);
          break;
        case 'p':
          play();
          break;
        case 's': // stop
          break;
        case 'L': // led value
          leds = 1<<get_val();
          break;
        case 'A': // first servo
        case 'B': // second servo
          set_servo( c, get_val() );
          break;
        default:
          break;
      }
	  }
  }
} // do_serial()


/**
*** Main program
*** work in 2 modes: 
***   1) HID mode (tried first): receive commands from USB
***   2) Serial mode (if USB/HID handshake fails within a certain time): Receive commands from serial port
*** In addition the chip has a USB/MSD bootloader that can be activated by holding the program swith when the chip
*** comes out of reset. 
**/
int main(void) {

  wait(2.0);

  serial_nr = iap.read_serial();
 
// enable IR transmitter
  ir_pwm.period(1.0/38000.0);
  ir_pwm.pulsewidth(1.0/72000.0);

	button.mode(PullUp);
	uart_rx.mode(PullNone); // disable pull-up, so we can enumerate 
	
	for(int l=0, p=1; l<7;l++, p|= (1<<l))
	{
	  leds = p;
	  wait(0.15);
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
	    play();

    if (hid.readNB(&recv_report)) 
	  {
	    read_res++;
      switch ( recv_report.data[0] ) 
      {
        case 0: // set outputs
          leds = recv_report.data[1];
          servo1.pulsewidth_us(700 + recv_report.data[2]);
          servo2.pulsewidth_us(700 + recv_report.data[3]);
          send_report.data[2] = button;
          break;
        case 1: // Save data to eeprom 
          for(int i=0; i<FAN_STEPS; i++)
            program[i] = ((unsigned short int)recv_report.data[1 + 2*i]) | ((unsigned short int)recv_report.data[2 + 2*i] << (unsigned short int)8);
          send_report.data[1] = iap.write_eeprom( (char*)program, (char*)TARGET_ADDRESS, MEM_SIZE );
          break;
        case 2: // Run program
          play();        
        default: // unkown command
          break;
        }			
    }
	 // printf("send: %d receive: %d counter: %d\n", send_res, read_res, counter);
	  wait_ms(1);
	}
} // main
