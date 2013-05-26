###########################################
# Simple Makefile for HIDAPI test program
#
# Alan Ott
# Signal 11 Software
# 2010-06-01
###########################################

all: fanbot hidcom

CC=gcc
CXX=g++
COBJS=hid.o
CPPOBJS= ../fanbot/hidcom.o  ../fanbot/fanbot.o
OBJS=$(COBJS) $(CPPOBJS)
CFLAGS=-I../hidapi -g -c 
LIBS= -lsetupapi

fanbot: ../fanbot/fanbot.o hid.o
	g++ -g $^ $(LIBS) --static -o fanbot

hidcom: ../fanbot/hidcom.o hid.o
	g++ -g $^ $(LIBS) --static -o hidcom

  $(COBJS): %.o: %.c
	$(CC) $(CFLAGS) $< -o $@

$(CPPOBJS): %.o: %.cpp
	$(CXX) $(CFLAGS) $< -o $@

clean:
	rm *.o ../fanbot/*.o ../fanbot/*.exe

.PHONY: clean
