# Makefile for 'crawler' program

# Compiler and flags
CC = gcc
CFLAGS = -Wall -pedantic -std=c11 -ggdb -I../libcs50 -I../common

# Directories and files
CRAWLER = crawler
LIBS = ../libcs50/libcs50-given.a ../common/pagedir.o

# Source files
OBJS = crawler.o

# Default rule: build the crawler executable
$(CRAWLER): $(OBJS) $(LIBS)
	$(CC) $(CFLAGS) -o $(CRAWLER) $(OBJS) $(LIBS)

# Rule for compiling crawler.c into an object file
crawler.o: crawler.c ../common/pagedir.h ../libcs50/webpage.h ../libcs50/hashtable.h
	$(CC) $(CFLAGS) -c crawler.c

# Clean rule: remove built files
clean:
	rm -f $(OBJS) $(CRAWLER)

# Test rule: Run tests (you can customize this as needed)
test: testing.sh
	./testing.sh &> testing.out

