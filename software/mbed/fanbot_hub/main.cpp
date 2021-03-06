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
#define VERSION_NR 123

// #define DEBUG 
// #define DEBUG_I2C

#define BAUDRATE 115200

#include "mbed.h"

#ifdef DEBUG
#include "USBSerial.h"
#endif


#include "pinmap.h"
#include "serial_api.h"
#include "IAP.h"
#include "serial_ringbuffer.h"

// externals from serial_api
extern int stdio_uart_inited;
extern serial_t stdio_uart;
 
#ifdef DEBUG
//Virtual serial port over USB
USBSerial usb;
#endif


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
unsigned int serial_nr = 0, serial_nr_rx; // serial nr and recieved serial # (for addressing)
unsigned char tagged = 0; // are we tagged?
unsigned short int checksum; // communication checksum, sum of all characters in frame, excluding pre-abmle and checksum
unsigned int port_state[24]; // 0 if port not used, serial number of connected device if port is used
unsigned short int port_cmd[24]; // 0: no command, MSB == opcode, LSB==value
unsigned short int arg[24]; // arguments for the opcodes


#ifdef DEBUG_I2C
I2C i2c(P0_5,P0_4);
static char init_code[] = {  // I2C init code
 0x38, // function set
 0x39, // function set
 0x14, // internal osc frequency
 0x7F, // contrast set (low nibble is contrast [0..F] for [min..max] )
 0x50, // power/icon control/contrast set
 0x6c, // follower control
 0x0c, // display on/off
 0x01 // clear display
};
const int i2c_addr = 124;
  
  
/**
*** i2c_test()
**/
void i2c_init()
{
  char cmd[2];
  i2c.frequency(300000);
  cmd[0] = 0;
  for(int i=0; i<sizeof(init_code); i++)
  {
    cmd[1] = init_code[i];
    i2c.write(i2c_addr, cmd, 2);
    wait(0.01);
  }
}

void i2c_data(char c)
{
  char cmd[2]; 
  cmd[0] = 0x40;
  cmd[1] = c;
  i2c.write(i2c_addr, cmd, 2);
}

void i2c_cmd(char c)
{
  char cmd[2]; 
  cmd[0] = 0x00;
  cmd[1] = c;
  i2c.write(i2c_addr, cmd, 2);
 // wait(0.001);
}


void i2c_test()
{
  char c;
  i2c_init();
  while(button)
  {
    //i2c_cmd(1);
    i2c_cmd(0x80); // cursor home
    c = '0';
    for(int i=0; i<16; i++)
      i2c_data(c++);
    c = 'A';
    i2c_cmd(0x80 + 0x40); // set data ptr to address 40 (start of 2nd line)
    for(int i=0; i<16; i++)
      i2c_data(c++);
  }
}
#endif

void debugstring(char *s)
{
#ifdef DEBUG
  usb.writeBlock((uint8_t *)s, strlen(s));
#endif

#ifdef DEBUG_I2C
  i2c_cmd(1); // cls
  i2c_cmd(0x80 + 0x40); // set data ptr to address 40 (start of 2nd line)
  while(*s)
    i2c_data(*s++);
#endif


}
 
void debugint(int i)
{
#ifdef DEBUG
  char d[13];
  sprintf(d, "%d", i);
  debugstring(d);
#endif
}

void debughex(int i)
{
#ifdef DEBUG
  char d[13];
  sprintf(d, "%8.8X", i);
  debugstring(d);
#endif
}




/**
*** read config()
*** Read serial number and EEPROM values
*** Config format in EEPROM: 
*** Entry 0..23: Bit offset of the 24 outputs (16 bit values)
***
**/
void inline read_config()
{
  int n=24, c, d, swap;
 

 
  serial_nr = iap.read_serial();
  debugstring("Serial:");
  debugint(serial_nr);
  srand(serial_nr);
  iap.read_eeprom( (char*)CONFIG_ADDRESS, (char *)config, sizeof(config) );
 /* sort in ascending order */
    for (c = 0 ; c < ( n - 1 ); c++)
  {
    for (d = 0 ; d < n - c - 1; d++)
    {
      if (config[d] > config[d+1]) /* For decreasing order use < */
      {
        swap       = config[d];
        config[d]   = config[d+1];
        config[d+1] = swap;
      }
    }
  }
  
}


/**
*** write_config()
*** Store the (updated) configuration in EEPROM
**/
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


#define MARK 1
#define SPACE 0
// receive RS232, 9600bps, wait for some time to see if there is a start bit, or return 0
char receive(DigitalInOut &pin)
{
  char c = 0;
  pin.input();
  pin.mode(PullDown);
  for(int i=0; i<30 && pin == MARK; i++); // wait for first falling edge
     wait_us(1);
  if ( pin == MARK ) return 0; // we have not seen an edge
  wait_us(1.5*t_bit); // wait for the start bit to finish and sample haveway the first bit
  for(int bit=0; bit<8; bit++) // sample 8 bits
  { 
    c |= ( pin == MARK ?  (1<<bit) : 0 );
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
 *** getchar
 **/
static inline char c_get()
{
  //char c = serial_getc(&stdio_uart);
  char c = (char) ringbuffer_getc();
//  debugint(c);
  return c;
  // return usb.getc();
}

 /**
 *** getchar
 **/
static inline void c_put(char c)
{
  serial_putc(&stdio_uart, c);
  // usb.putc(c);
}


/**
*** return true if data available
**/
static inline int c_available()
{
 //return serial_readable( &stdio_uart );
 return ringbuffer_empty() == 0;
 // return usb.available ();
}

 
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
*** Initialize UART and set as RS485 halve duplex with automatic transmit enable
**/
void rs485_init()
{
  serial_init( &stdio_uart, P1_13, P1_14); // pins: tx, rx
  serial_baud( &stdio_uart, BAUDRATE );
  LPC_USART->RS485CTRL |= (1<<4) | (1<<5); // Enable bit4: Automatic Direction control
  pin_function(P0_17,1); // RTS
}


/**
*** Send test string over RS485 and echo received chars from RS485
**/
void rs485_test()
{
  char buf[32];
  int n=0;
  //DigitalOut de(P0_17);
  rs485_init();
  memset(buf,0,sizeof(buf));
  printf("Board Serial: 0x%6.6X\n", serial_nr);
  while( button )
  {    
    while ( serial_readable( &stdio_uart ) && n >= 0 && n < (sizeof(buf) - 1) )
    {     
     led_a = 1;
     buf[n] = serial_getc(&stdio_uart);
     serial_putc(&stdio_uart, buf[n]);
     if ( buf[n] == '\r' ) 
       n = -1;
     else
       n++;       
    }
    if ( n < 0 )
    {     
      led_a = 0;
      n = 0;
      printf("\n\rHello World!\n\r\n\r");
      printf(buf);
      memset(buf,0,sizeof(buf));
    }
  }
}

#ifdef DEBUG
/**
*** Send all data from USB to RS485 and vice versa
**/
void rs485_comm()
{
  while( button )
  {    
    led_a = 0;
    led_b = 0;
    if ( serial_readable( &stdio_uart ) )
    {
      led_a = 1;
      usb.putc( serial_getc(&stdio_uart) );
    }
    if ( usb.available () )
    {
      led_b = 1;
      serial_putc(&stdio_uart, usb.getc());
    }
  }
  
}
#endif


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
      if ( hub_pin[i] == P0_15 ) pin_function(P0_15,1);
      if ( hub_pin[i] == P0_10 ) pin_function(P0_10,1);
      send(hub, c);   
    }
  }
}

#ifdef DEBUG
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
#endif

unsigned int request_id(unsigned char n)
{
  unsigned int id=0;
  char val, ch, j, bitshift=32;
  DigitalInOut pin(hub_pin[n]);
  send(pin, 'n');
 
  while( bitshift ) // for all 8 characters
  {
     for(j=0, ch=0; j<200 && ch==0; j++)
       ch = receive(pin);
     if ( ch == 0 ) 
       return 0; // timeout
     if ( ch >= '0' && ch <= '9' )
       val = ch - '0' ;
     else if ( ch >= 'A' && ch <= 'F' )
       val = 10 + ch - 'A';
     id = id + (val << bitshift); 
     bitshift -= 4;
  }
  return id;
}

/**
*** Request the serial nr from the robot connected to this pin
**/
void request_test(void)
{
  char s[32];
    
  while(button)
  {  
    sprintf(s,"0: %8.8X\r\n", serial_nr );
    
    debugstring(s);
    for(int i=0; i<24; i++)
    {
      sprintf(s,"%c: %8.8X\r\n", 'A' + i, request_id(i) );
      debugstring(s);
    } 

  }
  return;
}



#define RX_TIMEOUT 100
// Read n data bytes, if buf == NULL, the data is read, but not stored. Return the number of bytes read
unsigned short read_data(unsigned char *buf, unsigned short length)
{
  unsigned short int i, j=length;
  unsigned char c;
  while ( length )
  {
    for(i=0; i<RX_TIMEOUT && !c_available(); i++ ) 
      wait(0.01);
    if ( i == RX_TIMEOUT ) return 0;
    c = c_get();
    if ( buf != NULL ) *buf++ = c;
    checksum += c;
    length--;  
  }
  return j;
}


// Write data on the bus
unsigned short write_data(unsigned char *buf, unsigned short length)
{
  while(length--)
  {
    c_put(*buf);
    checksum += *buf;
    *buf++;
  }
}

/**
*** getint()
*** receive 16-bit integer from serial port
*** Return 1 if OK, 0 if NOK
**/
unsigned short getint(unsigned short *v)
{
  unsigned short int i;
  unsigned char c[2];
  if ( read_data(c, 2) != 2 ) return 0;
  *v = c[0] + (c[1] << 8);
  return 1;
}


/**
*** getint32()
*** receive 32-bits integer from serial port
**/
unsigned short getint32(unsigned int *v)
{
  unsigned short int i;
  unsigned char c[4];
  if ( read_data(c, 4) != 4 ) return 0;
  *v = c[0] + (c[1] << 8) + (c[2] <<16) + (c[3] << 24);
  return 1;
}

/**
*** putint32()
*** send 32-bits integer from serial port
**/
void putint32(unsigned int i)
{
  unsigned char c[4];
  c[0] = i & 0xFF;
  c[1] = (i>>8) & 0xFF;
  c[2] = (i>>16) & 0xFF;
  c[3] = (i>>24) & 0xFF;
  write_data(c, 4);
}

/**
*** putint()
*** send 16-bits integer from serial port
**/
void putint(unsigned short int i)
{
  unsigned char c[2];
  c[0] = i & 0xFF;
  c[1] = (i>>8) & 0xFF;
  write_data(c, 2);
}

/**
*** Send start of frame, reset checksum
**/
void startframe(unsigned short int opcode, unsigned short int length)
{
  c_put('#');
  c_put('#');
  checksum = '#' + '#';
  putint(opcode);
  putint(length);
}


/**
*** Send end of frame, containing checksum
**/
void endframe()
{
  putint(checksum);
}


// Opcodes
enum HubOpcodes { 
  REQUEST_ID = 1,
  TAG_ID,
  PLAY_FRAME,
  LED_FRAME,
  POS_FRAME,
  REQUEST_STATUS,
  CONFIG_FRAME,
  RANDOM_FRAME,
  ID_REPORT = 128,
  STATUS_REPORT,
  RESET = 0xDEAD,
};


/**
*** Read and process data according to specified opcode and length
*** Process data on the fly. 
*** Only use this data if the checksum is correct (calculated after all data is recieved)
*** So execution is delayed to after reception of the checksum
**/
unsigned short int process_opcode(unsigned short int opcode, unsigned short int length)
{
   unsigned short bit, index;  
   switch ( opcode )
   {
     case REQUEST_ID: // no arguments
     case RESET: 
       break; // 4 bytes argument (serial #)
     case REQUEST_STATUS: 
     case TAG_ID: 
       if ( !getint32(&serial_nr_rx) ) return 0; 
       break;
     case PLAY_FRAME: // 1 bit per port
     case LED_FRAME: // 1 bit per port
     case POS_FRAME: // 2 bits per port
      debugstring("Receive frame.\n\r");
      debugint(length);
      index = 0;
      for(int i=0; i<length; i++)
       {
         unsigned char c;
         /* c = c_get();
         checksum += c; */
         if ( read_data(&c, 1) != 1 ) return 0;
        
         for(int j=0; j<8; j++) // for all bits in this byte
         {
           if ( config[index] == i*8 + j ) // check if this bit in the frame corresponds to configuration index
           {
             if ( c & (1<<j) )  // if bit is set: enable leds, otherwise, disable leds
               port_cmd[index] = 255;
             else
               port_cmd[index] = 0;
             port_cmd[index] += opcode<<8; // set the opcode in the MSB of the command
             if ( index < 24 ) 
               index++; // next one
           }
         }
       }
       break;
     case CONFIG_FRAME: // 4 bytes serial + 24 * 4 bytes
       debugstring("receive config\n\r");
       if ( !getint32(&serial_nr_rx) ) return 0; // we expect a serial nr!
       for( int i=0; i<24; i++)
         if ( getint(&arg[i]) == 0 ) return 0;
       debugstring("receive config done.\n\r");
       break;
     default: // unknown opcode, just absorb the data
       debugstring("Unknown opcode");
       debugint(opcode);
       debugint(length);
       if ( read_data(NULL, length) != length ) return 0;
   };
   
   return 1;
}
 
 /**
 *** execute_opcode()
 *** Execute the received opcode (this function is only called if the received checksum was correct)
 **/
 unsigned short int execute_opcode(unsigned short int opcode)
 {
    switch ( opcode )
   {
     case REQUEST_ID: // Send our ID on the bus if we where not tagged already, after a random time
       if ( !tagged ) 
       {
         wait_us( rand() & 0xFFFFF  ); // random 0..1 sec delay time
         startframe(ID_REPORT, 4);
         putint32(serial_nr);
         endframe();
       }
       break;
     case TAG_ID: // Indicate we are tagged and do not need to send our ID again
       if ( serial_nr_rx == serial_nr ) 
         tagged = 1; 
       break;
     case REQUEST_STATUS: // send status frame
       if ( serial_nr_rx == serial_nr )
       {
         startframe(STATUS_REPORT, 4 + 24*4 + 4); // status is [my id] [24 robot ids] [software version]
         putint32(serial_nr);
         for(int i=0; i<24; i++)
           putint32( request_id(i) ); //putint32( request_id(i) );
         putint32(VERSION_NR); // version
         endframe();
       }
       break;
     case CONFIG_FRAME: // 4 bytes serial + 24 * 4 bytes
       if ( serial_nr_rx == serial_nr )
       {
         debugstring("Config frame:");     
         for(int i=0; i<24; i++)
         {
           config[i] = arg[i];
           debugint(arg[i]);
         }
         write_config();
       }
       startframe(ID_REPORT, 4);
       putint32(serial_nr);
       endframe();
       break;
     case RESET: // Reset the tagged state
       tagged = 0;
       break;
     case PLAY_FRAME: // 1 bit per port
       debugstring("play frame!");
       for(int i=0; i<24; i++)
       {
         if ( port_cmd[i] & 0xFF )
           send_masked('p', 1<<i);
         else
           send_masked('s', 1<<i);
       }
       break;
     case LED_FRAME: // 1 bit per port
       debugstring("led frame!");
       for(int i=0; i<24; i++)
       {
        send_masked('L', 1<<i);
        send_masked(port_cmd[i] & 0xFF, 1<<i);
       }
       break;
     case POS_FRAME: // 1 bits per port (left/right)
       debugstring("pos frame!");
       for(int i=0; i<24; i++)
       {
        send_masked('A', 1<<i);
        if ( port_cmd[i] & 0xFF )
          send_masked(255, 1<<i);
        else
          send_masked(1, 1<<i);
       }
       for(int i=0; i<24; i++)
       {
        send_masked('L', 1<<i);
        if ( port_cmd[i] & 0xFF )         
          send_masked(255, 1<<i);
        else
          send_masked(0, 1<<i);
       }
       break;
     default: // unknown opcode, flash lights
       for(int i = 0; i< 4; i++)
       {
         led_b = i & 1;
         wait(0.1);
       }
       break;
   };
   return 1;
 }
 
 
/**
*** arm test
*** Move arms from left to right
**/
void arm_test(int n)
{
  unsigned long mask;
  unsigned char j;
  while( button && n )
  {
    j++;
    if ( n > 0 ) n--;
    if ( j > 7 ) j = 0;
    for(int i=0; i<24; i++)
    {
      led_b = i & 32;
      mask = 1 << i;
      send_masked('L', mask);
      send_masked(1<<j, mask );
     send_masked('A', mask);
      send_masked(32*j, mask );
    }
  }
}

 /** Go into random mode */
void random_mode()
{
  signed char dir[24];
  while ( !c_available() ) 
  {
    for(int i=0; i<24; i++)
    {
      switch ( rand() & 0x4F )
      {
      case 0: dir[i] = -10; break;
      case 1: dir[i] = 10; break;
      case 2: send_masked('p', 1<<i); break;
      case 3: 
       send_masked('A', 1<<i);
       send_masked(port_cmd[i], 1<<i); 
       break;
      case 4: arm_test(8); break;
      default: 
       send_masked('L', 1<<i);
       send_masked(rand() & 0xFF , 1<<i); 
       wait(0.2); 
       break;      
      }
      port_cmd[i] += dir[i];
      if ( port_cmd[i] > 255 ) port_cmd[i] = 255;
      if ( port_cmd[i] < 1 ) port_cmd[i] = 1;
      wait(0.01);
    }
  } 
}
 
/**
*** HUB Communication
*** Receive commands via serial port
*** Protocol: '#' '#' [Opcode LSB] [Opcode MSB] [Length LSB] [Length MSB] [DATA 0] .. [Data N] [Checksum LSB] [Checksum MSB]
*** Checksum is 16 bit unisgned sum of bytes [Opcode LSB] to [Data N]
*** Only execute opcode is checksum is correct
*** All data is received non blocking, with a timeout of 100 msec, if no data is received for 100 msec, the communication state is reset
**/
void hub_comm()
{
  static int i, error=0;
  char c=0, state=0;
  unsigned short int opcode, length, rx_checksum;
  while( button )
  {
    while ( !c_available() )  // wait for char 
    {    
      led_b = i & (tagged ? 0x10000 : 0x40000);
      led_a = (error > 0); 
      if ( error ) 
        error--;
      i++;
     if ( i > 0x100000 ) 
      {
        // random_mode();
        i = 0;
      } 
    }
    // debugstring("State: "); debugint(state); debugstring("\n");
    
    switch ( state )
    {
      case 0: // wait for '#'
        if ( c_get()  == '#' ) 
          state = 1;
        break;
      case 1:  // wait for start of frame '#'
        if ( c_get()  == '#' ) 
        {
          state = 2;      
          led_a = 1;
          checksum = '#' + '#'; 
          i = 0;          
        }
        else
          state = 0;
        break;
      case 2: // wait for opcode
        state = (getint(&opcode) ? 3 : 0); 
        break;
      case 3: // wait for length
        state = (getint(&length) ? 4 : 0);
        if ( length > 1024 ) state = 0; // assume maximum length       
        break;
      case 4: // receive data [length bytes]
        // debugstring("Opcode: ");
        // debugint(opcode);
        // debugstring("Length: ");
        // debugint(length);
        state = (process_opcode(opcode, length) ? 5 : 0); // If process is OK (!=0) then next state
        break;
      case 5: // check the checksum, restart comm. state if incorrect
        int c = checksum;
        debugstring("Checksum: ");
        debugint(checksum);
        getint(&rx_checksum);
        debugint(rx_checksum);
        if ( c == rx_checksum ) // execute function
        {
          error = 1000;
          debugstring("Execute opcode.\n\r"); 
          execute_opcode(opcode);
          debugstring("Done.\n\r");          
        }
        else
        {
          debugstring("Checksum error.\n\r");
          error = 20000; 
        }
        state = 0;
        break;
      default:
        error = 20000;
        state = 0;
        break;
    }
  }
}

#ifdef DEBUG
/**
*** Receive via USB virtual COM port and send to all ports
**/
void usb_comm()
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
#endif


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
void led_test(int n)
{
  unsigned long mask;
  unsigned char j;
  while( button && n )
  {
    if ( n > 0 ) n--;
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
  rs485_init();
  ringbuffer_init();
  led_test(10);
  arm_test(8);
  
  
  //ringbuffer_test();
  
#ifdef DEBUG_I2C
  i2c_init();
#endif
  
  while( 1 ) 
  {
    switch ( select_function(5) )
    {
      case 0: send_masked('p', 0xFF);  wait(5); break;// no selection
      case 1: hub_comm(); break; // protocol decoder
      case 2: led_test(-1); break;
      case 3: arm_test(-1); break;
      case 4: request_test(); break;

#ifdef DEBUG_I2C
      case 5: i2c_test(); break;
#endif

      default: break;

      /* 
      case 2: iotest(); break;
      case 4: rs485_test(); break;
      case 5: rs485_comm(); break;
      case 6: play_test(); break;
      case 7: servo_test(); break;
      */
      
    }
  }
}

