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
#include <unistd.h>

using namespace std;


bool fexists(const char *filename)
{
  std::ifstream ifile(filename);
  return ifile;
}


int copyfile2(const char *from, const char *to)
{
  std::ifstream  src(from);
  std::ofstream  dst(to);
  dst << src.rdbuf();
}

int copyfile(const char *from, const char *to)
{
    char buf[BUFSIZ];
    size_t size;

    FILE* source = fopen(from, "rb");
    FILE* dest = fopen(to, "wb");

    while (size = fread(buf, 1, BUFSIZ, source)) {
        fwrite(buf, 1, size, dest);
    }

    fclose(source);
    fclose(dest);
    printf("\n%s copied to %s...\n", from, to);
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
  while(1)
  {
    waitfor(argv[2], false);
    copyfile(argv[1], argv[2]);
    waitfor(argv[2], true);
  }
  return 0;
}