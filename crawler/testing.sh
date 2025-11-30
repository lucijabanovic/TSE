#!/bin/bash

# Test invalid arguments
echo "Testing invalid arguments..."
./crawler invalid-url ../webpages/ 1
./crawler http://example.com ../webpages/ -1  # Invalid max depth
./crawler http://example.com ../webpages/ 11  # Invalid max depth
./crawler http://example.com  # Missing arguments

# Run with Valgrind
echo "Running with Valgrind..."
valgrind --leak-check=full --show-leak-kinds=all ./crawler http://toscrape.com ../webpages/ 1

# Run various valid tests
echo "Running valid tests..."
./crawler http://cs50tse.cs.dartmouth.edu/tse/ ../webpages/ 2
./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/ ../webpages/ 2
./crawler http://cs50tse.cs.dartmouth.edu/toscrape/ ../webpages/ 1
./crawler http://cs50tse.cs.dartmouth.edu/tse/wikipedia/ ../webpages/ 1

echo "Tests completed."
