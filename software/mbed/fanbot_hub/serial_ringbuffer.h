 
#ifndef        SERIAL_RINGBUFFER
#define        SERIAL_RINGBUFFER

void ringbuffer_init(void);
 
/*
	put one character in the ringbuffer ringbuffer.
	If the ringbuffer is full after inserting the character, advance also the  out pointer, 
	with the effect of  forgetting the oldest character
	Will return -1 if no character is available
*/
void ringbuffer_putc(uint8_t c);

/*
	Get one character from the ringbuffer. NON-Blocking!
	Will return -1 if no character is available
*/
int ringbuffer_getc(void);
/*
	Get one character from the ringbuffer. Blocking!
*/
int ringbuffer_waitc(void);
/*
	Return 1 if the ringbuffer is empty, else 0
*/
int ringbuffer_empty(void);
/*
	Return 1 if the ringbuffer is (almost) full  else 0
*/
int ringbuffer_full(void);

/*
	Simple blocking test for ringbuffer. When called the function will:
	* block for 5 seconds while flashing led A very fast (20 hrz)
	* block for ever.
	* If no character is received led A will flash with 5 hrz
	* If character 32 (space) is received, led B is toggled.
*/

void ringbuffer_test(void);
 
#endif    //  #ifndef  SERIAL_RINGBUFFER

