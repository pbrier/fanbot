// LCD utility app -jcw, 2013-06-12

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include "lcd.h"
#include "SFont.h"

#define WIDTH 240
#define HEIGHT 320
uint8_t rgbIn24 [WIDTH*HEIGHT*3];
unsigned rgbOut16 [WIDTH][HEIGHT];

static void initLcd () {
  wiringPiSetup();
  SetPinNU(0,1,2,3,4,5,6,7,8,10,9,11,15,16,14,12);
  SetLCDSize(LCD_28);
  LCDInit();
}

static void displayBitMap (const char* filename) {
	// read entire rgb image into memory
  int fd = open(filename, O_RDONLY);
  if (fd < 0) {
    perror(filename);
    exit(1);
  }
  read(fd, rgbIn24, sizeof rgbIn24);
  if (lseek(fd, 0, SEEK_CUR) != sizeof rgbIn24) {
    fprintf(stderr, "bad file size, expected %ld\n", sizeof rgbIn24);
    exit(1);
  }
  close(fd);
  
	// convert 24-bit to 16-bit (5+6+5) colour, and rotate + flip image by 90 deg
  int x, y;
	const uint8_t *p = rgbIn24;
  for (y = 0; y < HEIGHT; ++y) {
    for (x = WIDTH; --x >= 0;) {
      uint8_t r = *p++ >> 3;
      uint8_t g = *p++ >> 2;
      uint8_t b = *p++ >> 3;
      rgbOut16[x][y] = (r << 11) | (g << 5) | b;
    }
	}

	// init the LCD display now that the hard work is done and display the image
  initLcd();
  drawBitmapP(0, 0, HEIGHT, WIDTH, rgbOut16[0], 1);
}

static void displayMessage (const char* msg) {
  initLcd();
  setFont(16,16,32);

  setColorRGB(255, 0, 0);
  fillRect(0, 0, 319, 239);
  setColorRGB(255, 255, 255);
  setBackColorRGB(255, 0, 0);
  print((char*) msg, CENTER, 119,0);
}

static void waitForTouch (const char* fileCheck) {
  initLcd();
  Touch_Init();
  Touch_Read();
  for (;;) {
  	if (Touch_DataAvailable()) {
  		Touch_Read();
  		int x = Touch_GetX();
  		int y = Touch_GetY();
  		printf("%d,%d\n", x, y);
      break;
  	}
  }
}

int main (int argc, const char* argv[]) {
  char mode = '?';
  if (argc > 1) {
    if (argv[1][0] == '-') {
      mode = argv[1][1];
      ++argv;
      --argc;
    } else
      mode = 'b';
  }
    
  if (mode == 'b' && argc == 2) {
    displayBitMap(argv[1]);
		return 0;
	}
    
  if (mode == 'm' && argc == 2) {
    displayMessage(argv[1]);
    return 0;
  }
      
  if (mode == 'w') {
    waitForTouch(argv[1]);
    return 0;
  }
  
  if (mode == 't' && argc == 1) {
    wiringPiSetup();
    pinMode(1, 1);
    int i;
    for (i = 0; i < 100000000; ++i) {
      digitalWrite(1, 1);
      digitalWrite(1, 0);
    }
    return 0;
  }
      
  puts("Usage:\n");
  puts("  lcd -b <file>     - display a 24-bit 320x240 RGB bitmap on LCD");
  puts("  lcd -m <msg> ...  - display one or more text messages on LCD");
  puts("  lcd -w [<file>]   - wait for screan touch or until file exists");
  puts("  lcd -t            - timing test: 100,000,000 pulses to GPIO #18");
  puts("");
  return 1;
}
