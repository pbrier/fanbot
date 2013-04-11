  
/*
  Fanbot proxy
  Server program for the fanbot project
  See: www.fanbot.org
  Writting for processing 2.0 (see http://www.processing.org)
  
    Copyright (C) 2013 Peter Brier

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
    
    
    The server protocol is quite simple, all single byte commands:
    
    Char.   Hex.    Function
    ~~~~~~  ~~~~~~  ~~~~~~~~~~~~~~~~~
     '^'    0x5E    Start of frame. Go to cell (0,0) the upper left corner 
     '*'    0x2A    Turn cell ON
     ' '    0x20    Advance pointer, do not change anything
     '\n'   0x0A    Goto beginning of next line
     '\r'   0x10    Do nothing (Ignore)
     other          Turn cell OFF 
    
*/
    
import processing.net.*; 


// TCP Server
int port = 2013; // TCP/IP port number
Server myServer;   
Client thisClient;

// Screen size, cursor pointer and buffer
int w = 42; // width
int h = 24; // height
int x = 0; // cursor x (0..w-1)
int y = 0; // cursor y (0..h-1)
char[] screen = new char[w*h]; // screen buffer


// Display size:
int s =20; // dot size
int count = 0;


/*
 * Initial setup
 */
void setup() { 
  size((w+0)*s, (h+0)*s); 
  myServer = new Server(this, port);
  for(int y=0; y<h; y++)
      for(int x=0; x<w; x++)
        screen[y*w+x] = ( (y & 1) == 0 ? ' ' : '*' );
} 
 

/*
 * Continously called
 */
void draw()
{ 
  int base = 150;
  if ( thisClient != null )
  {
    int len = thisClient.available();
    if ( len > 0 )
      println("client len: " +  len );

    for(int i = 0; i < len; i++)
    { 
      base = 200;
      int b =thisClient.read();
      // println("data: " +  b );
      if ( b < 0 ) 
      {
         thisClient = null;
         return;
      }
      switch( b )
      {
        case '\n': 
          x = 0;
          y++;
          break;
        case ' ':
          x++;
          break;
        case '^':
          x = 0;
          y = 0;
          break;
        case '\r':
          break;
        default:
          if ( y >= h ) y = 0;
          if ( x < w ) screen[y*w + x] = (char)b;
          x++;
          break;
      }     
    }
  }
  
  // Display the screen
  for(int y=0; y<h; y++)
    for(int x=0; x<w; x++)
    {
      if ( screen[w*y + x ] != '*' )
        fill(color(0));
      else
        fill(color(base + 50 * sin(count/10.0),0,0));
      rect(x*s, y*s, (x+1)*s, (y+1)*s);
    } 
  count++;
} 

// ServerEvent message is generated when a new client connects 
// to an existing server.
void serverEvent(Server someServer, Client someClient) {
  // println("We have a new client: " + someClient.ip());
    thisClient = someClient;
    x=0; y=0;
}



