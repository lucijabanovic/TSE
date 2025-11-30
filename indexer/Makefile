CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -ggdb

# Object files for indexer
OBJ_INDEXER = indexer.o ../common/common.a ../libcs50/libcs50.a

# Object files for indextest
OBJ_INDEXTEST = indextest.o ../common/common.a ../libcs50/libcs50.a

# Executable files
EXEC_INDEXER = indexer
EXEC_INDEXTEST = indextest

# All targets
all: $(EXEC_INDEXER) $(EXEC_INDEXTEST)

$(EXEC_INDEXER): $(OBJ_INDEXER)
	$(CC) $(CFLAGS) -o $@ $(OBJ_INDEXER)

$(EXEC_INDEXTEST): $(OBJ_INDEXTEST)
	$(CC) $(CFLAGS) -o $@ $(OBJ_INDEXTEST)

# Generic rule to compile .c files to .o files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(EXEC_INDEXER) $(EXEC_INDEXTEST) *.o *.index

# Test target
test:
	bash -v testing.sh &> testing.out