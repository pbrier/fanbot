/*

FanBot HUB

RS485 to 24 Serial HUB

Copyright (c) 2013 Peter Brier

This file is part of the FanBot project. 
See www.kekbot.org

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
#include "USBSerial.h"
#include "pinmap.h"
#include "serial_api.h"
#include "IAP.h"

// externals from serial_api
extern int stdio_uart_inited;
extern serial_t stdio_uart;
 
 
//Virtual serial port over USB
USBSerial usb;


DigitalIn button(P0_1);
DigitalOut led_b(P1_28);
DigitalOut led_a(P1_31);

#define BAUD 9600
#define t_bit (1000000/BAUD) // 9600 bps, 10 microseconds bit time

// Hub pin numbers for 24 ports
static PinName hub_pin[] = {
  P0_16, //D1
  P0_15, //D2 *SWDIO (select pin function 1 for GPIO)
  P1_22, //D3
  P0_14, //D4
  P0_13, //D5
  P0_12, //D6
  P0_11, //D7
  P1_29, //D8
  P0_22, //D9
  P0_10, //D10 *SWDCLK (select pin function 1 for GPIO)
  P0_9,  //D11 
  P0_8,  //D12
  P0_23, //D13
  P1_15, //D14
  P0_18, //D15
  P0_19, //D16
  P1_25, //D17
  P0_20, //D18
  P0_2,  //D19
  P1_26, //D20
  P1_27, //D21
  P1_23, //D22
  P1_24, //D23
  P0_7,  //D24
};


// EEPROM globals and functions
#define CONFIG_ADDRESS       0  // EEPROM starting address for config data
IAP iap;
unsigned short int config[128]; // buffer for EEPROM, 256 bytes, need to be word aligned, nr of of bytes, needs to be 256, 512 or 1024
int serial_nr = 0; // serial nr
unsigned short checksum; // communication checksum, sum of all characters in frame, excluding pre-abmle and checksum

void inline read_config()
{
  serial_nr = iap.read_serial();
  iap.read_eeprom( (char*)CONFIG_ADDRESS, (char *)config, sizeof(config) );
}

void inline write_config()
{
 iap.write_eeprom( (char*)config, (char*)CONFIG_ADDRESS, sizeof(config) );
}



// Software UART send. mark is LOW space is HIGH
void send(DigitalInOut  &pin, char c)
{
  pin.output();
  pin.mode(PullDown);
  pin = 0; // 1 start bit
  wait_us(t_bit);
  for(int bit=0; bit<8; bit++) // 8 bits
  { 
    pin = ( ( (unsigned char) c & (1<<bit) ) ? 1 : 0 );
    led_a = ( ( (unsigned char) c & (1<<bit) ) ? 1 : 0 );
    wait_us(t_bit);
  }
  pin = 1; // 1 stop bit
  wait_us(t_bit);
  pin.input();
}


// receive RS232, 9600bps, wait for some time to see if there is a start bit, or return 0
char receive(DigitalInOut &pin)
{
  char c = 0;
  pin.input();
  pin.mode(PullDown);
  for(int i=0; i<30 && pin == 1; i++); // wait for first falling edge
     wait_us(1);
  if ( pin == 1 ) return 0; // we have not seen an edge
  wait_us(1.5*t_bit); // wait for the start bit to finish and sample haveway the first bit
  for(int bit=0; bit<8; bit++) // sample 8 bits
  { 
    c |= ( pin == 1 ?  (1<<bit) : 0 );
    led_a = ( ( (unsigned char) c & (1<<bit) ) ? 1 : 0 );
    wait_us(t_bit);
  }
  wait_us(t_bit); // wait for the stopbit to complete
  return c;
}

class Watchdog {
public:
    void kick(float s) {
     __disable_irq();
        LPC_WWDT->CLKSEL = 0x1;                // Set CLK src to PCLK
        //uint32_t clk = 6000/2;    // WD has a fixed /4 prescaler, PCLK default is /4 
        LPC_WWDT->TC = 0xFFFF; // s * (float)clk;         
        LPC_WWDT->MOD = 0x3;                   // Enabled and Reset    
        LPC_WWDT->FEED = 0xAA;
        LPC_WWDT->FEED = 0x55;        
        kick();
         __enable_irq();
    }
    
    void kick() {
        LPC_WWDT->FEED = 0xAA;
        LPC_WWDT->FEED = 0x55;
    }
};
 
// Watchdog w;
  
 /**
 *** iotest()
 *** Toggle all outputs
 **/
 void iotest()
 {
  int i=0;
  // w.kick(2.5);

  while( button )
  {
    i++;
    led_a = 1;
    led_b = 0;
    for(i=0; i<(sizeof(hub_pin)/sizeof(hub_pin[0])); i++)
    {
      DigitalOut hub(hub_pin[i]);
      pin_function(P0_15,1);
      pin_function(P0_10,1);
      hub = 1;
    }
    wait(0.1);
    led_a = 0;
    led_b = (button ? 0 : 1);
    for(i=0; i<(sizeof(hub_pin)/sizeof(hub_pin[0])); i++)
    {
      DigitalOut hub(hub_pin[i]);
      pin_function(P0_15,1);
      pin_function(P0_10,1);
      hub = 0;
    }
    wait(0.1);
  }
}

/**
*** Send test string over RS485 and echo received chars
**/
void rs485_test()
{
  char buf[128];
  int n=0;
  //DigitalOut de(P0_17);
  DigitalOut de(NC);
  serial_init( &stdio_uart, P1_13, P1_14); // pins: tx, rx
  LPC_USART->RS485CTRL |= (1<<4) | (1<<5); // Enable bit4: Automatic Direction control
  pin_function(P0_17,1); // RTS
  memset(buf,0,sizeof(buf));
  printf("Hello World!\n");
  while( button )
  {    
    de = 0;
    while ( serial_readable( &stdio_uart ) && n >= 0 )
    {     
     led_a = 1;
     buf[n] = serial_getc(&stdio_uart);
     de = 1;
     serial_putc(&stdio_uart, buf[n]);
     de = 0;
     if ( buf[n] == '\r' ) 
       n = -1;
     else
       n++;       
    }
    if ( n < 0 )
    {     
      led_a = 0;
      n = 0;
      de = 1; // driver enable
      printf("Hello World!\n");
      printf(buf);
      memset(buf,0,sizeof(buf));
    }
  }
}




/**
*** send_masked()
*** Send character to all ports where the mask is '1'
**/
void send_masked(char c, unsigned long mask)
{
  for(int i=0; i<(sizeof(hub_pin)/sizeof(hub_pin[0])); i++)
  {
    if ( mask & 1<<i )
    {
      DigitalInOut hub(hub_pin[i]);
      pin_function(P0_15,1);
      pin_function(P0_10,1);
      send(hub, c);   
    }
  }
}


/**
*** Receive via USB virtual COM port and send to one port, and receive chars from that port
**/
void single_comm(DigitalInOut &pin)
{
  static int i=0;
  char c;
  pin_function(P0_15,1);
  pin_function(P0_10,1);

  while( button )
  { 
	  while ( usb.available () )
    {	 
       c = usb.getc();
       if ( c == '$' ) return;
       send(pin, c);   
    }
    c = receive(pin);
    if ( c ) usb.putc( c );  
    led_a = ( (i++) & 256 ? 1 : 0 );
    led_b = c;
  }
}


/**
*** getint()
*** receive 16-bit integer from serial port
**/
#define RX_TIMEOUT 100
unsigned short getint(unsigned short *v)
{
  unsigned short int i;
  unsigned char c;
  for(i=0; i<RX_TIMEOUT && !usb.available(); i++ ) 
    wait(0.01);
  if ( i == RX_TIMEOUT ) return 0;
  c = usb.getc();
  checksum += c;
  *v = c;
  for(i=0; i<RX_TIMEOUT && !usb.available(); i++ ) 
    wait(0.01);
  if ( i == RX_TIMEOUT ) return 0;
  c = usb.getc();
  checksum += c;
  *v = *v + ((unsigned short int)c)<<8;
  return 1;
}


 unsigned short int execute_opcode(unsigned short int opcode)
 {
   for(int i = 0; i< 10; i++)
   {
     led_b = i & 1;
     wait(0.1);
   }
   return 1;
 }
 
 
 unsigned short int process_opcode(unsigned short int opcode, unsigned short int length)
 {
   unsigned short int i;
   unsigned char c;
   
   usb.putc(opcode & 0xFF);
   usb.putc(opcode >> 8);
   usb.putc(length & 0xFF);
   usb.putc(length >> 8);
    
   while ( length )
   {
     for(i=0; i<RX_TIMEOUT && !usb.available(); i++ ) 
       wait(0.01);
     if ( i == RX_TIMEOUT ) return 0;
     c = usb.getc();
     checksum += c;
     length--;  
   }
   return 1;
 }
 
 
/**
*** HUB Communication
*** Receive commands via serial port
*** Protocol: '#' '#' [Opcode LSB] [Opcode MSB] [Length LSB] [Length MSB] [DATA 0] .. [Data N] [Checksum LSB] [Checksum MSB]
*** Checksum is 16 bit unisgned sum of [Opcode LSB] to [Data N]
*** Only execute opcode is checksum is correct
*** All data is received non blocking, with a timeout of 100 msec, if no data is received for 100 msec, the communication state is reset
**/
void hub_comm()
{
  static int i;
  char c=0, c1=0, state=0;
  unsigned short int opcode, length, rx_checksum;
  while( button )
  { 
    switch ( state )
    {
      case 0: 
        led_a = 0;
        c = usb.getc();
        if ( c == '#' && c1 == '#' )
        {
          led_a = 1;
          c1 = 0;
          checksum = 0;          
          state = 1;
        }
        else
          c1 = c;          
        break;
      case 1: 
        state = (getint(&opcode) ? 2 : 0); 
        break;
      case 2:
        state = (getint(&length) ? 3 : 0); 
         break;
      case 3: // receive data
        state = (process_opcode(opcode, length) ? 4 : 0);
        break;
      case 4:
        led_b = 1;
        state = (getint(&rx_checksum) ? 5 : 0); 
        break;
      case 5:
        led_b = 1;
        if ( checksum == rx_checksum ) 
          execute_opcode(opcode);
        led_b = 0;
      default:
        state = 0;
        break;
    }
  }
}
       
/**
*** Receive via USB virtual COM port and send to all ports
**/
void rs485_comm()
{
  static int i;
  char c=0;
  while( button )
  { 
	  while ( usb.available () )
    {	 
       c = usb.getc();
       if ( c == '#' ) 
       {
         i = usb.getc() - 'A';
         DigitalInOut hub(hub_pin[i]);      
         single_comm(hub);        
       }
       for(i=0; i<(sizeof(hub_pin)/sizeof(hub_pin[0])); i++)
       {
         DigitalInOut hub(hub_pin[i]);
         pin_function(P0_15,1);
         pin_function(P0_10,1);
         send(hub, c);   
       }
    }
    if ( c ) usb.putc( c );  
    led_a = ( (i++) & 256 ? 1 : 0 );
    led_b = c;
  }
}


/**
*** play test
*** Activate all hub ports one by one
**/
void play_test()
{
  unsigned long mask;
  while( button )
  {
    for(int i=0; i<24; i++)
    {
      led_b = i & 1;
      mask = 1 << i;
      send_masked('p', mask);
      wait(0.5);
    }
  }
}


/**
*** led test
*** Activate all leds one by one
**/
void led_test()
{
  unsigned long mask;
  unsigned char j;
  while( button )
  {
    j++;
    if ( j > 7 ) j = 0;
    for(int i=0; i<24; i++)
    {
      led_b = i & 32;
      mask = 1 << i;
      send_masked('L', mask);
       //wait(0.04);
      send_masked(1<<j, mask );
     // wait(0.04);
    }
  }
}


/**
*** servo test
*** Activate all servos one by one
**/
void servo_test()
{
  unsigned long mask;
  int pos=30,dir=-1;
  while( button )
  {
    if ( pos <= 20 || pos >= 50) dir *= -1;
    pos += dir;
    for(int i=0; i<24; i++)
    {
      led_b = i & 1;
      mask = 1 << i;
      send_masked('A', mask);
       //wait(0.04);
      send_masked(pos, mask );
     // wait(0.04);
    }
  }
}



/**
*** menu select function
*** Press button in 1st second to make selection, 
*** then press button to scroll through choices, 
*** release button for 2 seconds and we are good to go
*** The current selection is indicated by flashes on LED A
*** Return: 0 (no selection) 1..n (one of the functions was selected)
**/
int select_function(int n)
{
  int i=0, val=1, prev=1, timer=0;
  button.mode(PullUp);
  for(i=0; i<10; i++) // toggle leds, to indicate we can press the button to select function
  {
    led_a = (i & 1 ? 0 : 1);
    led_b = (i & 1 ? 1 : 0);
    wait(0.1);
    if ( button == 0 ) break;
  }
  if ( i == 10 ) // Button was not pressed, return
  {
    led_a = 0;
    led_b = 0;
    return 0; // not pressed
  }

  while( 1 ) // Stay in selection mode
  {
    i++;
    if ( !button && button != prev ) // button pressed event, increment selection value and reset timeout
    {
      val++;
      i = 0;
      timer = 0;
      led_b = 0;
      wait(0.15);
    }
    led_b = button;
    prev = button;
    led_a = ( (i&2) &&  (i/4) < val ? 1 : 0); // flash led 1/4th of the period, 'val' flashes
    if ( val > n ) val = 1; // wrap selection around
    if ( i > (4*val)+16 ) // wrap flash index around 
    {
      i = 0; 
      timer++;
    } 
    if ( timer > 2 ) 
      return val; // if nothing pressed, return
    wait(0.08);
  }
}


/**
*** Main function
**/
int main(void) {
  read_config();

  while( 1 ) 
  {
    switch ( select_function(7) )
    {
      case 0: // no selection
      case 1: hub_comm(); break;
      case 2: iotest(); break;
      case 3: led_test(); break;
      case 4: rs485_test(); break;
      case 5: rs485_comm(); break;
      case 6: play_test(); break;
      case 7: servo_test(); break;
      default: break;
    }
  }
}

