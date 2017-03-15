CC=gcc
CFLAGS=-g -Wall -pthread

all: procd

procd: procd.o
	$(CC) $(CFLAGS) procd.c -o procd

procd.o: procd.c
	$(CC) $(CFLAGS) procd.c -o procd

clean:
	rm *o procd
