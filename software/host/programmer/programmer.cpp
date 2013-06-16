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

#include <iostream>
#include <fstream> 
#include <cstdio>    // fopen, fclose, fread, fwrite, BUFSIZ
#include <ctime>
#include <cstring>
#include <unistd.h>

using namespace std;


bool fexists(const char *filename)
{
  std::ifstream ifile(filename);
  return ifile;
}


int copyfile(const char *from, const char *to)
{
    char buf[BUFSIZ];
    size_t size, total=0;

    printf("\nCopy '%s' to '%s'... ", from, to);
    
    FILE* source = fopen(from, "rb");
    FILE* dest = fopen(to, "wb");

    while (size = fread(buf, 1, BUFSIZ, source)) {
        fwrite(buf, 1, size, dest);
        total += size;
    }

    fclose(source);
    fclose(dest);
    printf("Done (%d bytes).\n\n", (int) total);
}

int comparefile(const char *a, const char *b)
{
    char buf_a[BUFSIZ], buf_b[BUFSIZ];
    size_t size;
    int diff=0;

    printf("\nCompare '%s' to '%s'... ", a, b);
    
    FILE* fa = fopen(a, "rb");
    FILE* fb = fopen(b, "rb");

    while ( (size = fread(buf_a, 1, BUFSIZ, fa)) && (!diff) ) 
    {
      if ( fread(buf_b, 1, size, fb) != size ) 
      {
        diff = 1;
        printf("Size");
      }
      if ( memcmp(buf_a, buf_b, size) ) 
        diff = 1;
    }

    fclose(fa);
    fclose(fb);
    if ( diff ) 
      printf("\nFILES DIFFER!.\n\n");
    else
      printf("\nFILES ARE THE SAME!.\n\n");
    return diff;
}



void waitfor(const char *name, int state)
{
 while( fexists(name) == state ) 
  {
    usleep(1000*100);
    printf( (state ? "." : "-") );
  }
}


/**
*** Wait for destination file to exist
*** Copy source file, and wait for it to disappear
**/
int main(int argc, char *argv[]) 
{
  char *to=argv[2], *from=argv[1];
  if ( argc != 3 )
  {
    printf("%s: [source] [dest]\n", argv[0]);
    return 1;
  }
  while(1)
  {
    waitfor(to, false);
    if ( comparefile(from,to) )
      copyfile(from, to);
    comparefile(from,to);
    waitfor(to, true);
  }
  return 0;
}