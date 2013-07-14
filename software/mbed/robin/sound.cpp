/*

Copyright (c) 2013 Peter Brier

This file is part of the FanBot project.

Note: ringtone player function is copied from this file:

//
// rtttl.c
//
// Ringing Tones Text Transfer Language (RTTTL) player
//
// Copyright (C) 2002 Michael Ringgaard. All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions
// are met:
// 
// 1. Redistributions of source code must retain the above copyright 
//    notice, this list of conditions and the following disclaimer.  
// 2. Redistributions in binary form must reproduce the above copyright
//    notice, this list of conditions and the following disclaimer in the
//    documentation and/or other materials provided with the distribution.  
// 3. Neither the name of the project nor the names of its contributors
//    may be used to endorse or promote products derived from this software
//    without specific prior written permission. 
// 
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
// ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE
// FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
// DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
// OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
// HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
// LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
// OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF 
// SUCH DAMAGE.
// 

The various melodies came from various sources on the Internet, without a clear 
copyright statement.

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

#include "mbed.h"
#include "USBKeyboard.h"
 
//LED1: NUM_LOCK
//LED2: CAPS_LOCK
//LED3: SCROLL_LOCK
BusOut leds(P1_19, P1_25, P0_8, P0_9, P0_22, P0_13, P0_14); // Leds
 
//USBKeyboard
USBKeyboard keyboard;
PwmOut ir_pwm(P1_13);
PwmOut servo1(P0_19);
PwmOut servo2(P0_18);

#define SERVO_TIMEOUT 40000 // msec to switch servo off of no position change has occured [msec]
#define TICK_INTERVAL 50 // tick interval [msec]
#define SERVO_PERIOD 15 // period [mec]
#define SERVO_OFFSET 1000 // 1300
#define SERVO_GAIN 3

#define BARBIE "Barbie:d=4,o=5,b=125:8g#,8e,8g#,8c#6,a,p,8f#,8d#,8f#,8b,g#,8f#,8e,p,8e,8c#,f#,c#,p,8f#,8e,g#,f#"
#define POPCORN "Popcorn:d=4,o=5,b=160:8c6,8a#,8c6,8g,8d#,8g,c,8c6,8a#,8c6,8g,8d#,8g,c,8c6,8d6,8d#6,16c6,8d#6,16c6,8d#6,8d6,16a#,8d6,16a#,8d6,8c6,8a#,8g,8a#,c6"
#define MISSION "MI:d=4,o=6,b=100:32d,32d#,32d,32d#,32d,32d#,32d,32d#,32d,32d,32d#,32e,32f,32f#,32g,16g,8p,16g,8p,16a#,16p,16c,16p,16g,8p,16g,8p,16f,16p,16f#,16p,16g,8p,16g,8p,16a#,16p,16c,16p,16g,8p,16g,8p,16f,16p,16f#,16p,16a#,16g,2d,32p,16a#,16g,2c#,32p,16a#,16g,2c,16p,16a#5,16c"
#define SW "StWars:d=4,o=5,b=180:8f,8f,8f,2a#.,2f.6,8d#6,8d6,8c6,2a#.6,f.6,8d#6,8d6,8c6,2a#.6,f.6,8d#6,8d6,8d#6,2c6,p,8f,8f,8f,2a#.,2f.6,8d#6,8d6,8c6,2a#.6,f.6,8d#6,8d6,8c6,2a#.6,f.6,8d#6,8d6,8d#6,2c6"
//#define ST "Star Trek:d=4,o=5,b=63:8f.,16a#,d#.6,8d6,16a#.,16g.,16c.6,f6"  
#define ST "StarTrek:d=4,o=5,b=80:8g,16c6,f6,16e6,16c6,16a,16d6,8g6,8p,8g,16c6,f6,16e6,16c6,16a,16d6,8g6"
#define MONTY "P:d=4,o=5,b=200:f6,8e6,d6,8c#6,c6,8b,a#,8a,8g,8a,8a#,a,8g,2c6,8p,8c6,8a,8p,8a,8a,8g#,8a,8f6,8p,8c6,8c6,8p,8a,8a#,8p,8a#,8a#,8p,8c6,2d6,8p,8a#,8g,8p,8g,8g,8f#,8g,8e6,8p,8d6,8d6,8p,8a#,8a,8p,8a,8a,8p,8a#,2c6,8p,8c6"
#define KR "Knight Rider:d=32,o=5,b=63:16e,f,e,8b,16e6,f6,e6,8b,16e,f,e,16b,16e6,4d6,8p,4p,16e,f,e,8b,16e6,f6,e6,8b,16e,f,e,16b,16e6,4f6"
#define ATEAM "A-Team:d=8,o=5,b=125:4d#6,a#,2d#6,16p,g#,4a#,4d#.,p,16g,16a#,d#6,a#,f6,2d#6,16p,c#.6,16c6,16a#,g#.,2a#"
#define GATGET "Gadget:d=16,o=5,b=50:32d#,32f,32f#,32g#,a#,f#,a,f,g#,f#,32d#,32f,32f#,32g#,a#,d#6,4d6,32d#,32f,32f#,32g#,a#,f#,a,f,g#,f#,8d#"
// #define JB "007:d=4,o=5,b=320:c,8d,8d,d,2d,c,c,c,c,8d#,8d#,2d#,d,d,d,c,8d,8d,d,2d,c,c,c,c,8d#,8d#,d#,2d#,d,c#,c,c6,1b.,g,f,1g."
#define JB "Bond:d=4,o=6,b=56:16c#5,16e5,32c,8b5,32e5,32g.5,g#5,32e.5,32f#5,32e5,8d#5,32g#4,32c#.5,a#4,16c#5,16e5,32c,8b5,32e5,32g.5,g#5,32e.5,32f#5,32e5,8d#5,32g#4,32c.5,c#.5"

#define GB "Ghostbus:d=4,o=5,b=90:16g#.,16g#.,32b.,32c.6,16d#.6,16f#.6,16f#.6,16c#.6,16c#.6,16g#.,16g#.,32b.,32c.6,16d#.6,16f#.6,16f#.6,16c#.6,16c#.6,16g#.,16g#.,32b.,32c.6,16d#.6,16f#.6,16f#.6,16c#.6,16c#.6,16g#.,16g#.,32b.,32c.6,16d#.6,16f#.6,16f#.6,16c#.6,16c#.6"
#define NC "Nut cracker Suite:d=16,o=5,b=76:16g6,e6,8g6,8p,f#6,p,d#6,p,e6.,p.,d6,d6,d6,8p,c#6,c#6,c#6,8p,c6,c6,c6,8p,b,e6,c6,e6,b,4p,g,e,8g,p,f#,p,c6,p,8b,8p,g6,g6,g6,8p,f#6,f#6,f#6,8p,e6,e6,e6,8p,d#6,f#6,e6,f#6,d#6,4p.,g6,e6,g6.,32p,f#6,p,d#6,p,e6,p,d6,d6,d6,p,c#6,c#6,c#6,p,c6,c6,c6,p,b,e6,c6,e6,b,2p,"
#define TF "Transformers:d=16,o=6,b=285:e7,f7,e7,d#7,4d7,4p,d,d,d,d,d,d,d,d,e,e,e,e,f,f,f,f,f,f,f,f,8a7,8a#7,8a7,8p,4d7,2p,d,d7,d,d7,d,d7,d,d7,e,e7,e,e7,f,f7,f,f7,f,f7,f,f7,a5,a5,a5,a5,a#5,a#,a#5,a#,a#5,a#,a#5,a#,a#5,a#,a#5,a#,4p,8d,8p,4e,4f,4p,4f,4p,2g,4a,4a#,4p,g,g7,g,g7,g,g7,g,g7,4e,4g,4a,4p,f,f7,f,f7,f,f7,f,f7,4e,4f,4g,4p,e,e7,e,e7,e,e7,e,e7,e,e7,e,e7,4p,4d,4c#,8e,8p,4d,2d,d,d7,d,d7,d,d7,d,d7"
#define AF "32p,8g,8p,16a#.,8p,16g,16p,16g,8c6,8g,8f,8g,8p,16d.6,8p,16g,16p,16g,8d#6,8d6,8a#,8g,8d6,8g6,16g,16f,16p,16f,8d,8a#,2g,4p,16f6,8d6,8c6,8a#,4g,8a#.,16g,16p,16g,8c6,8g,8f,4g,8d.6,16g,16p,16g,8d#6,86,8a#,8g,8d6,8g6,16g,16f,16p,16f,8d,8a#,2g"
#define TW "Twilight:d=16,o=7,b=285:a#,g#,8p,a#,a,8p,a#,g#,8p,a#,e,8p,a#,g#,8p,a#,a,8p,a#,g#,8p,a#,e,8p,a#,g#,8p,a#,a,8p,a#,g#,8p,a#,e,8p,a#,g#,8p,a#,a,8p,a#,g#,8p,a#,e,8p,a#,g#,8p,a#,a,8p,a#,g#,8p,a#,e,8p,a#,g#,8p,a#,a,8p,a#,g#,8p,a#,e,8p,a#,g#,8p,a#,a,8p,a#,g#,8p,a#,e,8p,a#,g#,8p,a#,a,8p,a#,g#,8p,a#,e,8p,a#,g#,b5,c#6,b5,c#6,b5,c#6,b5,c#6,b5,c#6,b5,d6,a#5,d6,a#5,d#6,a5,2p.,g6,a#6,d#,e,c#,e,d#,a#6,g#6,a#6,e,g#,e,a#"

static const char *songs[] = 
{
  MISSION, BARBIE, POPCORN, SW, ST, KR, ATEAM, GB, TF, TW, KR, 
};

#define Y1 1
#define Y2 2
#define R1 4
#define G1 8
#define R2 16
#define G2 32
#define R3 64

int const notefreq[12] = {4186, 4434, 4698, 4978, 5274, 5587, 5919, 6271, 6644, 7040, 7458, 7902};
int const led_note[12] = {R1, R2, R3, R1+R2+R3, Y1+Y2, G1+G2, Y1+Y2+G1+G2, R1+R3, Y1+Y2+R2, Y1+Y2+R1+R2+R3, R1+R2+R3+G1+G2, 255}; 

int note2freq(int note) {
  return notefreq[note % 12] / (1 << (9 - (note / 12)));
}



// Set servo position (clamped to range for Fanbot)
void set_servo(char n, volatile int val)
{
  PwmOut *p = (n == 'A' ? &servo1 : &servo2 );
  int a = (val ? SERVO_OFFSET : 0);  
  p->pulsewidth_us(a + SERVO_GAIN*val);
}


void play(const char *song) {
  char *p = (char*)song;
  int defdur = 4;
  int defscale = 6;
  int bpm = 63;
  
  // Skip name
  while (*p && *p != ':') p++;
  if (!*p) return;
  p++;

  // Parse defaults
  while (*p) {
    char param;
    int value;

    while (*p == ' ') p++;
    if (!*p) return;
    if (*p == ':') break;

    param = *p++;
    if (*p != '=') return;
    
    p++;
    value = 0;
    while (*p >= '0' && *p <= '9') value = value * 10 + (*p++ - '0');

    switch (param) {
      case 'd': defdur = 32 / value; break;
      case 'o': defscale = value; break;
      case 'b': bpm = value; break;
    }

    while (*p == ' ') p++;
    if (*p == ',') p++;
  }
  p++;

  while (*p) {
    int note = -1;
    int scale = defscale;
    int dur = defdur;
    int ms;
    int freq;

    // Skip whitespace
    while (*p == ' ') p++;
    if (!*p) return;

    // Parse duration
    if (*p >= '0' && *p <= '9') {
      int value = 0;
      while (*p >= '0' && *p <= '9') value = value * 10 + (*p++ - '0');

      dur = 32 / value;
    }

    // Parse note
    switch (*p) {
      case 0: return;
      case 'C': case 'c': note = 0; break;
      case 'D': case 'd': note = 2; break;
      case 'E': case 'e': note = 4; break;
      case 'F': case 'f': note = 5; break;
      case 'G': case 'g': note = 7; break;
      case 'A': case 'a': note = 9; break;
      case 'H': case 'h': note = 11; break;
      case 'B': case 'b': note = 11; break;
      case 'P': case 'p': note = -1; break;
    }
    p++;
    if (*p == '#') {
      note++;
      p++;
    }
    if (*p == 'b') {
      note--;
      p++;
    }

    // Parse special duration
    if (*p == '.') {
      dur += dur / 2;
      p++;
    }

    // Parse scale
    if (*p >= '0' && *p <= '9') scale = (*p++ - '0');

    // Parse special duration (again...)
    if (*p == '.') {
      dur += dur / 2;
      p++;
    }

    // Skip delimiter
    while (*p == ' ') p++;
    if (*p == ',') p++;

    // Play note
    ms = dur * 60000 / (bpm * 8);
    if (note == -1) {
      freq = 0;
    } else {
      set_servo('A', 10 + 10*note );
      set_servo('B', 10 + 10*note );
      leds = led_note[note];
      freq = note2freq((scale + 1) * 12 + note);
    }

    if (freq) {
      ir_pwm.period(1.0/freq);
      ir_pwm.pulsewidth(1.0/(2.0*freq) );
      wait_ms(ms * 7 / 8);
      ir_pwm.pulsewidth(0);
      wait_ms(ms / 8);
    } else {
      wait_ms(ms);
    }
  }
}


 
int main(void) 
{
  servo1.period_ms(SERVO_PERIOD);
  servo2.period_ms(SERVO_PERIOD);
	set_servo('A', 128);
  set_servo('B', 128);
  bool state = false, last_state = false;
  int timer, song = 0;
  
  for(int i=100; i<12288; i=(float)i*1.1)
  {
    ir_pwm.period(1.0/(float)i);
    ir_pwm.pulsewidth(1.0/(2.0*i) );
    wait(0.02);
    leds = 1<< (i>>11);
  }
  ir_pwm.pulsewidth(0.0);
  
  while (1) {
      //  keyboard.mediaControl(KEY_VOLUME_DOWN);
   //     keyboard.printf("Hello World from Mbed\r\n");
      //  keyboard.keyCode('s', KEY_CTRL);
     //   keyboard._putc('a');
        //keyboard.keyCode(KEY_CAPS_LOCK);
         wait(0.01);
         state = (keyboard.lockStatus() & 2) > 0 ;
         leds = (state ? 255 : 0 );
         
         if ( state != last_state ) 
         {
           set_servo('A', (state ? 128 : 128-120));
           set_servo('B', (state ? 128 : 128+120));
           if ( timer < 20)
           {
             //char *p = "Hello World!";
             //while(*p) keyboard._putc(*p++);
             play(songs[song++]);             
             if ( song >= (sizeof(songs)/sizeof(songs[0])) ) 
               song = 0;
           }
           timer=0;
         }
         timer++;
         last_state = state;
    }
}