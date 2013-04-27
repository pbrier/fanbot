###########################################
# Simple Makefile for HIDAPI test program
#
# Alan Ott
# Signal 11 Software
# 2010-06-01
###########################################

all: hidtest

CC=gcc
CXX=g++
COBJS=hid.o
CPPOBJS=../fanbot/fanbot.o
OBJS=$(COBJS) $(CPPOBJS)
CFLAGS=-I../hidapi -g -c
LIBS= -lsetupapi


hidtest: $(OBJS)
	g++ -g $^ $(LIBS) -o fanbot

$(COBJS): %.o: %.c
	$(CC) $(CFLAGS) $< -o $@

$(CPPOBJS): %.o: %.cpp
	$(CXX) $(CFLAGS) $< -o $@

clean:
	rm *.o ../fanbot/*.o fanbot.exe

.PHONY: clean
