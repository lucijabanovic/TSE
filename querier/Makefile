CC = gcc
CFLAGS = -Wall -pedantic -std=c11 -ggdb -I../libcs50 -I../common

# Object files for querier
OBJ_QUERIER = querier.o ../common/common.a ../libcs50/libcs50.a

# Executable files
EXEC_QUERIER = querier

# All targets
all: $(EXEC_QUERIER)

$(EXEC_QUERIER): $(OBJ_QUERIER)
	$(CC) $(CFLAGS) -o $@ $(OBJ_QUERIER)

# Generic rule to compile .c files to .o files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(EXEC_QUERIER) *.o

# Test target
test:
	bash -v testing.sh &> testing.out