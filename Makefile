CC = gcc
CLFAGS = -ansi -pedantic-errors -Wall -Werror
PROGS = mergetest public01 public02 public03 public04 public05

all: $(PROGS)

public01: public01.o mergesort.o
	$(CC) -o public01 public01.o mergesort.o -lpthread

public02: public02.o mergesort.o
	$(CC) -o public02 public02.o mergesort.o -lpthread

public03: public03.o mergesort.o
	$(CC) -o public03 public03.o mergesort.o -lpthread

public04: public04.o mergesort.o
	$(CC) -o public04 public04.o mergesort.o -lpthread

public05: public05.o mergesort.o mergesort.c
	$(CC) -o public05 public05.o mergesort.o -lpthread

public01.o: public01.c mergesort.c mergesort.h
	$(CC) $(CFLAGS) -c public01.c -lpthread

public02.o: public02.c mergesort.c mergesort.h
	$(CC) $(CFLAGS) -c public02.c -lpthread

public03.o: public03.c mergesort.c mergesort.h
	$(CC) $(CFLAGS) -c public03.c -lpthread

public04.o: public04.c mergesort.c mergesort.h
	$(CC) $(CFLAGS) -c public04.c -lpthread

public05.o: public05.c mergesort.c mergesort.h
	$(CC) $(CFLAGS) -c public05.c -lpthread

mergetest: mergesort.o mergetest.o
	$(CC) -o mergetest mergesort.o mergetest.o -lpthread

mergetest.o: mergetest.c mergesort.h
	$(CC) $(CFLAGS) -c mergetest.c -lpthread

mergesort.o: mergesort.c mergesort.h
	$(CC) $(CFLAGS) -c mergesort.c -lpthread

clean:
	rm -f *.o $(PROGS)
	