# Makefile for 'pagedir' module

# Compiler and flags
CC = gcc
CFLAGS = -Wall -pedantic -std=c11 -ggdb -I../libcs50

# Target library
LIB = common.a

# Source files
OBJS = pagedir.o word.o index.o

# Default rule: build the library
$(LIB): $(OBJS)
	ar cr $(LIB) $(OBJS)

word.o: word.h
index.o: index.h
pagedir.o: pagedir.h

# Clean rule: remove built files
clean:
	rm -f $(OBJS) $(LIB)

