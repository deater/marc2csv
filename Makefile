CC = gcc
CFLAGS = -O2 -Wall -g
LFLAGS =

all:	dump_record marc2csv print_header

###

print_header:	print_header.o
	$(CC) $(LFLAGS) -o print_header print_header.o

print_header.o:	print_header.c
	$(CC) $(CFLAGS) -c print_header.c

###

dump_record:	dump_record.o
	$(CC) $(LFLAGS) -o dump_record dump_record.o

dump_record.o:	dump_record.c
	$(CC) $(CFLAGS) -c dump_record.c
###

marc2csv:	marc2csv.o
	$(CC) $(LFLAGS) -o marc2csv marc2csv.o

marc2csv.o:	marc2csv.c
	$(CC) $(CFLAGS) -c marc2csv.c

###

clean:	
	rm -rf *~ *.o dump_record marc2csv print_header
