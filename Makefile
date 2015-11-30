CC=gcc
CFLAGS =-O3 -march=native -mtune=native -fopenmp
LIBS=-lgsl

all:	BLB_serial

BLB_serial:	BLB_serial.c
	$(CC) $(CFLAGS)	$(LIBS) -o BLB_serial BLB_serial.c