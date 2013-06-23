/*
    Fanbot Interactive client. 
    Allows you to make waves and control many fanbots
    (c) 2013 Peter Brier (peter.brier@kekketek.nl)
 
    This file is part of the Fanbot project

    Fanbot is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Fanbot is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Fanbot.  If not, see <http://www.gnu.org/licenses/>.
*/
  

import processing.net.*;    
Client thisClient;
int dataIn; 

int w = 42; // width
int h = 24; // height
int s =30; // dot size
int port = 2013; // TCP/IP port number
float decay = 0.8;
int diameter = 2;

int[] screen = new int[w*h];

int x = 0;
int y = 0;
int count=0;

void setup() { 
  frameRate(60);
  size((w+0)*s, (h+0)*s); 
  thisClient = new Client(this, "127.0.0.1", port); 
  for(int y=0; y<h; y++)
      for(int x=0; x<w; x++)
         screen[y*w+x] = ( ( (x+y) & 1) == 0 ? 0 : 255 );
} 


void set(int x, int y, int val)
{
  if ( x < 0 || x >= w ) return;
  if ( y < 0 || y >= h ) return;
  screen[y*w + x] += val; 
}

void paint(int x, int y, boolean state)
{
  int r = 20;
  float base = diameter * ( state ? -128 : 128);
  for(int i = -r; i<=r; i++)
    for(int j = -r; j<=r; j++)
    {
       set(x+i,y+j, (int)(base / sqrt(1.0F + i*i + j*j)) ); 
    }
}

void draw()
{ 
  if ( thisClient != null )
    thisClient.write('\f'); // use '^' in the future
  
  
  // Handle mouse event
  if (mousePressed == true) {
    int mx = mouseX/s;
    int my = mouseY/s;
    paint(mx,my,mouseButton == RIGHT );
  }
  
  
  // Display the screen
  for(int y=0; y<h; y++)
  {
    for(int x=0; x<w; x++)
    {
      int v = (int)(decay * screen[w*y + x ]);
      if ( v < 0 ) v = 0; 
      screen[w*y + x ] = v;
      if ( screen[w*y + x ] < 128  )
      {
        fill(color(0));
        thisClient.write(' ');
      }
      else
      {
        fill(color(220 + 35 * sin(count/20.0),0,0));
        thisClient.write('*');
      }
      rect(x*s, y*s, (x+1)*s, (y+1)*s);
    }
    thisClient.write('\n');
  } 
    count++;
} 





