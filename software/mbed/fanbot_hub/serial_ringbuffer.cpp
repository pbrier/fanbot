/* mbed Microcontroller Library
 * Copyright (c) 2006-2013 ARM Limited
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include "mbed.h"

#include "serial_api.h"
#include "serial_ringbuffer.h"


#define RINGBUFFER_SIZE 260
static uint8_t ringbuffer[RINGBUFFER_SIZE];
static uint16_t ringbuffer_in  = 0;
static uint16_t ringbuffer_out = 0;
extern serial_t stdio_uart;

void serial_handler(uint32_t id, SerialIrq event);

void ringbuffer_init(void){
  serial_irq_handler(&stdio_uart,&serial_handler,1);
  serial_irq_set(&stdio_uart, RxIrq, 1) ;

}
void serial_handler(uint32_t id, SerialIrq event){
	static int k;
	k++;
	//led_b = k & 11 ? 0:1;
	
	while(serial_readable(&stdio_uart) ) {
		int c = serial_getc(&stdio_uart);

		ringbuffer_putc( (uint8_t) c);
	}
}


void ringbuffer_putc(uint8_t c){
	ringbuffer[ringbuffer_in++] = c;
	ringbuffer_in %= RINGBUFFER_SIZE;
	if ( ringbuffer_out == ringbuffer_in ){
		// if after inserting one character the ringbuffer is full 
		// advance the outpointer, and forget the oldest character
		// Ringbuffer will never cointain more than RINGBUFFER_SIZE -1 characters
		ringbuffer_out++;
		ringbuffer_out %= RINGBUFFER_SIZE;
	}
}
/*
	Get one character from the ringbuffer. Blocking!
*/
int ringbuffer_waitc(void){
	while (ringbuffer_out == ringbuffer_in){
	}
	return ringbuffer_getc();
}


int ringbuffer_getc(void ){
	if (ringbuffer_out == ringbuffer_in){
		return -1;
	}
	int result = ringbuffer[ringbuffer_out++];
	ringbuffer_out %= RINGBUFFER_SIZE;
	return result; 
}
int ringbuffer_empty(void ){
	return ringbuffer_out == ringbuffer_in ? 1: 0;
}

int ringbuffer_full(void){
	if (ringbuffer_out > ringbuffer_in){
		return (ringbuffer_out - ringbuffer_in  == 1)  ? 1: 0;
	}else{
		return ( (ringbuffer_in - ringbuffer_out) == (RINGBUFFER_SIZE-1)  ) ? 1: 0 ;
	}
}

extern DigitalOut led_a,led_b;
/*
	Simple blocking test for ringbuffer. When called the function will:
	* block for 5 seconds while flashing led A very fast (20 hrz)
	* block for ever.
	* If no character is received led A will flash with 5 hrz
	* If character 32 (space) is received, led B is toggled.
*/

void ringbuffer_test(void){
  int la = 0, lb = 0, t = 0;
  while(t == 0){
	la++;
	led_a = la & 0x2;
	wait(0.02);
	if (la == 100) t = 1;
  } 
  led_b = 0;
  while(1){
	led_a = la & 0x1;
	led_b = lb & 0x1;
	wait(0.1);	
	
	int p = ringbuffer_getc();
	if (p == -1) 
		la++;
	if (p == 32) 
		lb++ ;
	
  }  
}
