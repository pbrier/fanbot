/*
    Fanbot proxy controller
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
Server myServer;   
Client thisClient;
int dataIn; 

int w = 42; // width
int h = 24; // height
int s =20; // dot size
int port = 2013; // TCP/IP port number
byte[] byteBuffer = new byte[10*w*h];
char[] screen = new char[w*h];

int x = 0;
int y = 0;
int count=0;

void setup() { 
    frameRate(60);
  size((w+0)*s, (h+0)*s); 
  myServer = new Server(this, port);
  for(int y=0; y<h; y++)
      for(int x=0; x<w; x++)
         screen[y*w+x] = ( ( (x+y) & 1) == 0 ? ' ' : '*' );
} 


void draw()
{ 
  if ( thisClient != null )
  {
    int len = thisClient.available();
    if ( len > 0 )
      println("client len: " +  len );
    
    for(int i = 0; i < len; i++)
    { 
      int b =thisClient.read();
      println("data: " +  b );
      if ( b < 0 ) 
      {
         thisClient = null;
         return;
      }
      if ( b == '\f' )
      {
        x = 0;
        y = 0; 
        return;
      }
      if ( b == '\r' ) return;
      if ( b == '\n' ) 
      {
         x = 0;
         y++;
      }
      else
      {
        if ( y >= h ) y = 0;
        if ( x < w ) screen[y*w + x] = (char)b;
        x++;
      }     
    }
  }
  
  // Handle mouse event
  if (mousePressed == true) {
    int mx = mouseX/s;
    int my = mouseY/s;
    screen[my*w + mx] = (mouseButton == LEFT ? ' ' : '*' );
  }
  
  
  // Display the screen
  for(int y=0; y<h; y++)
    for(int x=0; x<w; x++)
    {
      if ( screen[w*y + x ] != '*' )
        fill(color(0));
      else
        fill(color(220 + 35 * sin(count/20.0),0,0));
      rect(x*s, y*s, (x+1)*s, (y+1)*s);
    } 
    count++;
} 

// ServerEvent message is generated when a new client connects 
// to an existing server.
void serverEvent(Server someServer, Client someClient) {
  println("We have a new client: " + someClient.ip());
    thisClient = someClient;
    x=0; y=0;
}



