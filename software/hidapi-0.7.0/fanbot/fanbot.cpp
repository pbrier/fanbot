/*
 * fanbot.cpp
 * send data to fanbot HID device
 * Peter Brier (peter.brier@kekketek.nl)
 * If started, it will wait for a fanbot to connect
 * Once a fanbot is connected, it will report 
 *
 * CONNECT [serial] [name]
 *
 * The following commands can be issued after that:
 *
 * SET [leds] [servo1] [servo2]
 * PROGRAM [n] [d1] [d2] .. [dn]
 * NAME [ASCII name]
 * PLAY
 *
 * The program remains active while the robot is connected. If the robot is disconnected, the program will respond with 
 *
 * DISCONNECT
 * 
 *
 * Based on HIDAPI library. (http://github.com/signal11/hidapi/commits/master)
 *
 *   This program is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */
#include <stdio.h>
#include <wchar.h>
#include <string.h>
#include <stdlib.h>
#include "hidapi.h"

#define FAN_VID  0x1234
#define FAN_PID 0x006
#define FAN_SIZE 65 // message size

// Headers needed for sleeping.
#ifdef _WIN32
  #include <windows.h>
#else
  #include <unistd.h>
#endif

#define CMD_SET 0
#define CMD_PROGRAM 1
#define CMD_PLAY 2
#define CMD_STOP 4
#define CMD_NAME 3
#define CMD_NONE 0xFF

#define NAME_LENGTH 32
#define FAN_STEPS 20


/**
*** Flush queue
**/
void hid_flush(hid_device* handle)
{
  unsigned char hid_buf[FAN_SIZE];
  // Flush the report queue
  hid_set_nonblocking(handle, 1);
  while( hid_read(handle, hid_buf, sizeof(hid_buf) > 0));
  hid_set_nonblocking(handle, 0);
}

/**
*** read_program()
*** Read the program memory content
**/
int read_program(hid_device* handle, unsigned char *program)
{
  unsigned char hid_buf[FAN_SIZE];
  int seen[FAN_STEPS], leds, servo, idx, res, count;
  memset(seen, 0, sizeof(seen));
  
  hid_flush(handle);
  
  
  for(int j=0;j<100; j++) 
  {
    res = hid_read_timeout(handle, hid_buf, sizeof(hid_buf), 100);
   // printf(".\n");
    if ( res < 0 )
    {
      return res;
    }
    idx = hid_buf[48];
    leds = hid_buf[49];
    servo = hid_buf[50];
    // printf("%d %d %d %d\n", count, idx, leds, servo);
    if ( idx >=0 && idx <= FAN_STEPS )
    {
      seen[idx] = 1;
      program[2*idx] = leds;
      program[2*idx+1] = servo;
    }
    count = 0;
    for(int i=0; i<FAN_STEPS; i++)
      count += seen[i];
    if ( count == FAN_STEPS )
    {    
      return 0;      
    }
  }  
  
}

void print_program(unsigned char *program)
{
  printf("PROGRAM ");
  for(int i=0; i<FAN_STEPS; i++)
    printf("%d %d ", (int)program[2*i], (int)program[2*i+1]);
  printf("\n");
}


/**
*** Main()
**/
int main(int argc, char* argv[])
{
  int res, verbose = 0, do_read = 0, do_write=1, do_interactive = 0, vid = FAN_VID, pid = FAN_PID, report_size=FAN_SIZE;
  int serial = 0;
  char *name;
  unsigned char buf[256], program[2 * FAN_STEPS];
  #define MAX_STR 255
  wchar_t wstr[MAX_STR];
  hid_device *handle;
  int i;

  #ifdef WIN32
  UNREFERENCED_PARAMETER(argc);
  UNREFERENCED_PARAMETER(argv);
  #endif


  // parse the command-line
  for(int i=1; i<argc; i++)
  {
    if ( !strncmp(argv[i], "--vid=", 6) ) sscanf(argv[i]+6, "%X",  &vid);
    if ( !strncmp(argv[i], "--pid=", 6) ) sscanf(argv[i]+6, "%X",  &pid);
    if ( !strcmp(argv[i], "--verbose") ) verbose = 1;
    if ( !strcmp(argv[i], "--read") ) do_read = 1; 
    if ( !strcmp(argv[i], "--help") ) 
    {
      printf("fanbot: control fanbot via USB HID communication\n" 
       "kekbot.org - rev 1.0 - " __DATE__ " " __TIME__ "\n"
       "USE: \n"
       "  fanbot [options] [data]\n"
       "  --vid=0xABCD      VID to use (default=0x%4.4X)\n"
       "  --pid=0xABCD      PID to use (default=0x%4.4X)\n"
       "  --verbose         Show more information\n",
       FAN_VID, FAN_PID, FAN_SIZE
      );
      exit(1);
    }
  }
  
  // Wait for connection
  while( 1 )
  {
    if ( verbose ) printf("Wait for connection...\n");
    handle = hid_open(vid, pid, NULL);
    if ( handle ) break; // we have a connection
    // Sleep(500);    
    usleep(1000*100);
  }
  hid_flush(handle);
  

  // Read serial# and name
  buf[1] = 0;
  res = hid_read(handle, buf, sizeof(buf));
  memcpy(&serial, &buf[8], sizeof(serial) );
  name = (char*)&buf[12];
  printf("CONNECT %X %s\n", serial, name);
  
  memset(buf, 0, sizeof(buf));
  
  read_program(handle, program);
  print_program(program);
  
  if ( verbose ) 
  {
    // Read the Manufacturer String
    wstr[0] = 0x0000;
    res = hid_get_manufacturer_string(handle, wstr, MAX_STR);
    if (res < 0)
      printf("Unable to read manufacturer string\n");
    else
      printf("Manufacturer String: %ls\n", wstr);
    // Read the Product String
    wstr[0] = 0x0000;
    res = hid_get_product_string(handle, wstr, MAX_STR);
    if (res < 0)
      printf("Unable to read product string\n");
    else
      printf("Product String: %ls\n", wstr);

    // Read the Serial Number String
    wstr[0] = 0x0000;
    res = hid_get_serial_number_string(handle, wstr, MAX_STR);
    if (res < 0)
      printf("Unable to read serial number string\n");
    else
      printf("Serial Number String: (%d) %ls", wstr[0], wstr);
    printf("\n");

    // Read Indexed String 1
    wstr[0] = 0x0000;
    res = hid_get_indexed_string(handle, 1, wstr, MAX_STR);
    if (res < 0)
      printf("Unable to read indexed string 1\n");
    else
      printf("Indexed String 1: %ls\n", wstr);
  }
  
#define BEGIN() pch = strtok (str,",. ")
#define NEXT() pch = strtok (NULL,",. ")
#define COMMAND(s) (!strncmp(pch, s, sizeof(s)-1))

  while ( 1 )
  {
    char str[MAX_STR], *pch;
    
    fgets(str , MAX_STR-1, stdin);
    if ( strlen(str) < 1 ) continue;
    BEGIN();
    
    if ( COMMAND("SET") )
    {
      int leds=0, s1=0, s2=0;
      if ( (NEXT()) != NULL ) sscanf(pch, "%d", &leds);
      if ( (NEXT()) != NULL ) sscanf(pch, "%d", &s1);
      if ( (NEXT()) != NULL ) sscanf(pch, "%d", &s2);    
      buf[1] = CMD_SET;
      buf[2] = leds;
      buf[3] = s1;
      buf[4] = s2;
      res = hid_write(handle, buf, report_size);
    }
    else if ( COMMAND("PLAY") )
    {
      int n = 1;
      buf[1] = CMD_PLAY;
      if ( (NEXT()) != NULL ) sscanf(pch, "%d", &n);
      buf[2] = n;
      res = hid_write(handle, buf, report_size);
    }
    else if ( COMMAND("PROGRAM") )
    {
      int i = 2, val = 0;
      memset(buf,0,sizeof(buf));
      buf[1] = CMD_PROGRAM;
      while ( (NEXT()) != NULL )
      {
        sscanf(pch, "%d", &val);
        buf[i++] = val;     
     }
     res = hid_write(handle, buf, report_size);
    }
    else if ( COMMAND("NAME") )
    {
      buf[1] = CMD_NAME;
      memset(&buf[2], 0, 32);
      for(i=0; i<32 && str[i+5] != 0 && str[i+5] != '\n' && str[i+5] != '\r'; i++)
        buf[i+2] = (unsigned char) str[i+5];
      res = hid_write(handle, buf, report_size);
    }
    else if ( COMMAND("STOP") )
    {
      buf[1] = CMD_STOP;
      res = hid_write(handle, buf, report_size);
    }
    else if ( COMMAND("READ") )  // read memory content
    {
      res = read_program(handle, program);
      if ( res < 0 )
        break;
      print_program(program);
    }
    else if ( COMMAND("QUIT") )
    {
      break; 
    }
    else
      buf[1] = CMD_NONE;
  
    if (res < 0) 
    {
      printf("ERROR: Unable to write(), '%ls'\n", hid_error(handle));
      exit(1);
    }
    
  }

  printf("DISCONNECT\n");  
  
  hid_close(handle); 
  hid_exit();/* Free static HIDAPI objects. */
  
  return res;
}
