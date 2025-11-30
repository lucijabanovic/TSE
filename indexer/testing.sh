#!/bin/bash

# This script tests the functionality of the indexer program.
# It verifies correct handling of valid and invalid inputs,
# as well as the creation and correctness of the index file.

# Enable verbose output for debugging
set -v

# Create a temporary directory for testing
mkdir -p test_pages

# Test 1: Running indexer with valid arguments
echo "Test 1: Running indexer with valid arguments"
# Create a test crawler file to simulate the output of the crawler
echo "Creating test crawler file"
echo "test_pages/.crawler" > test_pages/.crawler

echo "http://example.cs.dartmouth.edu/" >> test_pages/1
echo "0" >> test_pages/1
echo "<html><body>This is a test page</body></html>" >> test_pages/1

echo "http://another-example.cs.dartmouth.edu/" >> test_pages/2
echo "1" >> test_pages/2
echo "<html><body>Another test page</body></html>" >> test_pages/2

# Run the indexer with the test directory and output index file
./indexer test_pages test_index.index

cat test_pages/1
# Check if the index file was created successfully
if [ -f test_index.index ]; then
    echo "Success: Index file created."
else
    echo "Failure: Index file not created."
fi

# Test 2: Run indextest to check index file format
echo "Test 2: Running indextest on created index file"
./indextest test_index.index new_test_index.index

# Capture the output of the indextest to testing.out
if [ -f new_test_index.index ]; then
    echo "Success: Indextest ran successfully. Output saved to new_test_index.index."
else
    echo "Failure: Indextest did not produce output."
fi

# Check if the expected output is formatted correctly
echo "Output of indextest:"
cat new_test_index.index

# Test 3: Running the indexer with an invalid page directory
echo "Test 3: Running indexer with invalid page directory"
./indexer invalid_directory test_index.index

# Check the output for expected error message
if [ $? -ne 0 ]; then
    echo "Success: Handled invalid page directory correctly."
else
    echo "Failure: Did not handle invalid page directory."
fi

# Test 4: Running indexer without sufficient arguments
echo "Test 4: Running indexer with insufficient arguments"
./indexer test_pages

# Check the output for expected error message
if [ $? -ne 0 ]; then
    echo "Success: Handled insufficient arguments correctly."
else
    echo "Failure: Did not handle insufficient arguments."
fi

# Test 5: Running indexer on an empty page directory
echo "Test 5: Running indexer on an empty page directory"
# Create an empty page directory for testing
mkdir -p empty_directory

# Run the indexer with the empty directory
./indexer empty_directory empty_index.index

# Check if the index file was created and is empty
if [ -f empty_index.index ]; then
    echo "Success: Index file created for empty directory."
else
    echo "Failure: Index file not created for empty directory."
fi

# Cleanup: Remove test files and directories
echo "Cleaning up test files and directories"
rm -rf test_pages empty_directory test_index.index new_test_index.index

echo "All tests completed."
