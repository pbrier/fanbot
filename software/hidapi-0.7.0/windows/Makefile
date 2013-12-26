###########################################
# Simple Makefile for HIDAPI test program
#
# Alan Ott
# Signal 11 Software
# 2010-06-01
###########################################

all: fanbot hidcom fanbot_nr

CC=gcc
CXX=g++
COBJS=hid.o
CPPOBJS= ../fanbot/hidcom.o  ../fanbot/fanbot.o ../fanbot/fanbot_nr.o
OBJS=$(COBJS) $(CPPOBJS)
CFLAGS=-I../hidapi -g -c 
LIBS= -lsetupapi

fanbot: ../fanbot/fanbot.o hid.o
	g++ -g $^ $(LIBS) --static -o fanbot

fanbot_nr: ../fanbot/fanbot_nr.o hid.o
	g++ -g $^ $(LIBS) --static -o fanbot_nr

hidcom: ../fanbot/hidcom.o hid.o
	g++ -g $^ $(LIBS) --static -o hidcom

  $(COBJS): %.o: %.c
	$(CC) $(CFLAGS) $< -o $@

$(CPPOBJS): %.o: %.cpp
	$(CXX) $(CFLAGS) $< -o $@

clean:
	rm *.o ../fanbot/*.o ../fanbot/*.exe

.PHONY: clean
