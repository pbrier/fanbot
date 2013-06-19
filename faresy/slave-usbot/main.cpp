// usbot - read FanBot info
// -jcw, 2013-06-19

// based almost verbatim on fanbot.cpp by Peter Brier (peter.brier@kekketek.nl)
// Based on HIDAPI library. (http://github.com/signal11/hidapi/commits/master)
// License GPL3

#include <stdio.h>
#include <wchar.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "hidapi.h"

#define FAN_VID  0x1234
#define FAN_PID 0x006
#define FAN_SIZE 65 // message size

#define NAME_LENGTH 32
#define FAN_STEPS 20

void hid_flush(hid_device* handle)
{
  unsigned char hid_buf[FAN_SIZE];
  hid_set_nonblocking(handle, 1);
  while( hid_read(handle, hid_buf, sizeof(hid_buf) > 0))
    ;
  hid_set_nonblocking(handle, 0);
}

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
  printf("CODE=");
  for(int i=0; i<FAN_STEPS; i++)
    printf("%02X%02X", (int)program[2*i], (int)program[2*i+1]);
  printf("\n");
}


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
    if ( !strcmp(argv[i], "--help") ) 
    {
      printf("fanbot: control fanbot via USB HID communication\n" 
       "kekbot.org - rev 1.0 - " __DATE__ " " __TIME__ "\n"
       "USE: \n"
       "  fanbot [options]\n"
       "  --vid=0xABCD      VID to use (default=0x%4.4X)\n"
       "  --pid=0xABCD      PID to use (default=0x%4.4X)\n"
       "  --verbose         Show more information\n",
       FAN_VID, FAN_PID
      );
      exit(1);
    }
  }
  
  // Wait for connection
  if ( verbose ) printf("Wait for connection...\n");
  while ((handle = hid_open(vid, pid, NULL)) == NULL)
    usleep(1000*100);
  hid_flush(handle);
  
  // Read serial# and name
  buf[1] = 0;
  res = hid_read(handle, buf, sizeof(buf));
  if (res != 64) {
    fprintf(stderr, "got %d bytes, expected %d\n", res, 64);
    hid_close(handle); 
    hid_exit();
    return 1;
  }
  
  printf("FBID=%08X\nNAME='%s'\n", *(int*) buf, buf + 12);
  
  memset(buf, 0, sizeof buf);
  read_program(handle, program);
  print_program(program);
  
  hid_close(handle); 
  hid_exit();
  return 0;
}
