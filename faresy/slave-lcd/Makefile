all: lcd
	
lcd: main.c lcd.o SFont.o
	gcc -o $@ main.c lcd.o SFont.o -lm -lwiringPi

lcd.c main.c SFont.c: lcd.h SFont.h

clean:
	rm -f *.o lcd