CFLAGS += -O2 -I/usr/include/libusb-1.0
LIBS = -lusb-1.0 -ludev

all: usbot
	
usbot: main.cpp hid-libusb.o
	g++ -o $@ $(CFLAGS) main.cpp hid-libusb.o $(LIBS)

clean:
	rm -f *.o usbot
