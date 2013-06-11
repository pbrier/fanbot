/*
 * hidcom.cpp
 * send data to fanbot HID device
 * Peter Brier (peter.brier@kekketek.nl)
 * Command line parameters: data string, e.g. "1 2 3 4 5" sent as decimal data in a fixed length report 
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


// Get the numeric value of an argument (0..255)
// Can be decimal, number or character
int get_val(char *s, int *d)
{
  if ( !strncmp(s, "0x", 2) ) // Hex 
    return sscanf(s, "%X", d);
  else if (s[0] == '@' ) // character
  {
    *d = (int)s[1];
    return 1;   
  }
  else
    return sscanf(s, "%d", d);
}

/**
*** Main()
**/
int main(int argc, char* argv[])
{
  int res, verbose = 0, do_read = 0, do_write=1, do_interactive = 0, do_reconnect = 0, vid = FAN_VID, pid = FAN_PID, report_size=FAN_SIZE;
  unsigned char buf[256];
  #define MAX_STR 255
  wchar_t wstr[MAX_STR];
  hid_device *handle;
  int i;

  #ifdef WIN32
  UNREFERENCED_PARAMETER(argc);
  UNREFERENCED_PARAMETER(argv);
  #endif

  if ( argc == 1 )
  {
    printf("fanbot: control fanbot via USB HID communication\n" 
     "kekbot.org - rev 1.0 - " __DATE__ " " __TIME__ "\n"
     "USE: \n"
     "  hidcom[options] [data]\n"
     "  --vid=0xABCD      VID to use (default=0x%4.4X)\n"
     "  --pid=0xABCD      PID to use (default=0x%4.4X)\n"
     "  --verbose         Show more information\n"
     "  --size=65         Set the report size (default=%d bytes)\n"
     "  --read            Perform a read after write, and display the data\n"
     "  --nowrite         Only read, do not write data\n"
     "  --interactive     Read data from stdin and write to stdout\n",
     FAN_VID, FAN_PID, FAN_SIZE
    );
    exit(1);
  }
  
  for(int i=1; i<argc; i++)
  {
    if ( !strncmp(argv[i], "--vid=", 6) ) sscanf(argv[i]+6, "%X",  &vid);
    if ( !strncmp(argv[i], "--pid=", 6) ) sscanf(argv[i]+6, "%X",  &pid);
    if ( !strncmp(argv[i], "--size=", 7) ) sscanf(argv[i]+7, "%d",  &report_size);
    if ( !strcmp(argv[i], "--verbose") ) verbose = 1;
    if ( !strcmp(argv[i], "--read") ) do_read = 1;
    if ( !strcmp(argv[i], "--nowrite") ) do_write = 0;  
    if ( !strcmp(argv[i], "--reconnect") ) do_reconnect = 1;  
    if ( !strcmp(argv[i], "--interactive") ) do_interactive = 1;  
  }

  handle = hid_open(vid, pid, NULL);

  if (!handle) 
  {
    printf("unable to open device (0x%X, 0x%X)\n", vid, pid);
    return 1;
  }
  
  memset(buf, 0, sizeof(buf));
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
  
  // Write data to HID device (all numeric arguments)
  if ( do_write )
  {
    buf[0] = 0;
    int n=1;
    for(int j = 1; j<argc; j++)
    {
      int d;
      if ( get_val(argv[j], &d) )
        buf[n++] = d; 
    }

    res = hid_write(handle, buf, report_size);
    
    if (res < 0) 
    {
        printf("Unable to write(): %d\n", i);
        printf("Error: %ls\n", hid_error(handle));
    }
  }
  
  if ( do_read ) 
  {  
    buf[1] = 0;
    res = hid_read(handle, buf, sizeof(buf));
    printf("%d:", res);
    for(int i=0; i<res; i++) 
      printf(" %d", buf[i]);
    printf("\n");
  }
    
  if ( do_interactive ) 
  {  
    while ( 1 )
    {
      char str[MAX_STR], *pch;
      int n=1;
      buf[0] = 0;
      fgets(str , MAX_STR-1, stdin);
      if ( strlen(str) <= 1) 
        break;
      pch = strtok (str,",. ");
      while (pch != NULL)
      {
       int d;
       if ( get_val(pch, &d) == 1 )
         buf[n++] = d;
         pch = strtok (NULL, ",. ");
      }
      res = hid_write(handle, buf, report_size);
    
      if (res < 0) 
      {
        printf("Unable to write(): %d\n", i);
        printf("Error: %ls\n", hid_error(handle));
      }
  
      buf[1] = 0;
      res = hid_read(handle, buf, sizeof(buf));
      printf("%d:", res);
      for(int i=0; i<res; i++) 
        printf(" %d", buf[i]);
      printf("\n");
    }
  }  
  hid_close(handle); 
  hid_exit();/* Free static HIDAPI objects. */
  
  return res;
}
