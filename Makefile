CC = gcc
CFLAGS = -O2 -Wall -g
LFLAGS =

all:	dump_record

dump_record:	dump_record.o
	$(CC) $(LFLAGS) -o dump_record dump_record.o

dump_record.o:	dump_record.c
	$(CC) $(CFLAGS) -c dump_record.c

clean:	
	rm -rf *~ *.o dump_record
