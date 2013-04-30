/*
 * fanbot.cpp
 * send data to fanbot HID device
 * Peter Brier (peter.brier@kekketek.nl)
 * Command line paramerers: data string, e.g. "1 2 3 4 5" 
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

#define FAN_VID	0x1234
#define FAN_PID 0x006
#define FAN_SIZE 64 // message size

// Headers needed for sleeping.
#ifdef _WIN32
	#include <windows.h>
#else
	#include <unistd.h>
#endif



int main(int argc, char* argv[])
{
	int res, verbose = 0, do_read = 0, do_write=1, do_interactive = 0;
	unsigned char buf[256];
	#define MAX_STR 255
	wchar_t wstr[MAX_STR];
	hid_device *handle;
	int i;

	#ifdef WIN32
	UNREFERENCED_PARAMETER(argc);
	UNREFERENCED_PARAMETER(argv);
	#endif

	handle = hid_open(FAN_VID, FAN_PID, NULL);

	if (!handle) 
	{
		printf("unable to open device (%X, %X)\n", FAN_VID, FAN_PID);
 		return 1;
	}
	
	for(int i=1; i<argc; i++)
	{
	  if ( !strcmp(argv[i], "--verbose") ) verbose = 1;
	  if ( !strcmp(argv[i], "--read") ) do_read = 1;
	  if ( !strcmp(argv[i], "--nowrite") ) do_write = 0;	
	  if ( !strcmp(argv[i], "--interactive") ) do_interactive = 1;	
	}

  memset(buf, 0, sizeof(buf));
  if ( verbose ) 
	{
	// Read the Manufacturer String
	wstr[0] = 0x0000;
	res = hid_get_manufacturer_string(handle, wstr, MAX_STR);
	if (res < 0)
		printf("Unable to read manufacturer string\n");
	printf("Manufacturer String: %ls\n", wstr);

	// Read the Product String
	wstr[0] = 0x0000;
	res = hid_get_product_string(handle, wstr, MAX_STR);
	if (res < 0)
		printf("Unable to read product string\n");
	printf("Product String: %ls\n", wstr);

	// Read the Serial Number String
	wstr[0] = 0x0000;
	res = hid_get_serial_number_string(handle, wstr, MAX_STR);
	if (res < 0)
		printf("Unable to read serial number string\n");
	printf("Serial Number String: (%d) %ls", wstr[0], wstr);
	printf("\n");

	// Read Indexed String 1
	wstr[0] = 0x0000;
	res = hid_get_indexed_string(handle, 1, wstr, MAX_STR);
	if (res < 0)
		printf("Unable to read indexed string 1\n");
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
	  if ( sscanf(argv[j], "%d", &d) == 1 )
	    buf[n++] = d; 
	}

  	res = hid_write(handle, buf, 65);
	  
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
	     if ( sscanf(pch, "%d", &d) == 1 )
	       buf[n++] = d;
         pch = strtok (NULL, ",. ");
      }
  	  res = hid_write(handle, buf, 65);
	  
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
